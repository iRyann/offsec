#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

static void die(const char *msg) {
  perror(msg);
  exit(1);
}

static int hexval(char c) {
  if ('0' <= c && c <= '9') return c - '0';
  if ('a' <= c && c <= 'f') return c - 'a' + 10;
  if ('A' <= c && c <= 'F') return c - 'A' + 10;
  return -1;
}

static size_t parse_hex(const char *s, uint8_t *out, size_t out_max) {
  size_t L = strlen(s);
  if (L % 2 != 0) {
    fprintf(stderr, "hex len must be even\n");
    exit(2);
  }
  size_t n = L / 2;
  if (n > out_max) {
    fprintf(stderr, "max %zu bytes\n", out_max);
    exit(2);
  }
  for (size_t i = 0; i < n; i++) {
    int hi = hexval(s[2 * i]);
    int lo = hexval(s[2 * i + 1]);
    if (hi < 0 || lo < 0) {
      fprintf(stderr, "invalid hex at pos %zu\n", 2 * i);
      exit(2);
    }
    out[i] = (uint8_t)((hi << 4) | lo);
  }
  return n;
}

static void hexdump(const uint8_t *p, size_t n) {
  for (size_t i = 0; i < n; i++) fprintf(stderr, "%02x", p[i]);
  fprintf(stderr, "\n");
}

static void segv_handler(int sig, siginfo_t *si, void *uctx_) {
  (void)sig;
  ucontext_t *uc = (ucontext_t *)uctx_;
#if defined(__x86_64__)
  greg_t *g = uc->uc_mcontext.gregs;
  fprintf(stderr, "\n[!] SIGSEGV @ %p\n", si->si_addr);
  fprintf(stderr, "RIP=%#llx RSP=%#llx RAX=%#llx RBX=%#llx RCX=%#llx RDX=%#llx\n",
          (unsigned long long)g[REG_RIP],
          (unsigned long long)g[REG_RSP],
          (unsigned long long)g[REG_RAX],
          (unsigned long long)g[REG_RBX],
          (unsigned long long)g[REG_RCX],
          (unsigned long long)g[REG_RDX]);
  fprintf(stderr, "RSI=%#llx RDI=%#llx RBP=%#llx R8=%#llx R9=%#llx R10=%#llx\n",
          (unsigned long long)g[REG_RSI],
          (unsigned long long)g[REG_RDI],
          (unsigned long long)g[REG_RBP],
          (unsigned long long)g[REG_R8],
          (unsigned long long)g[REG_R9],
          (unsigned long long)g[REG_R10]);
  fprintf(stderr, "R11=%#llx R12=%#llx R13=%#llx R14=%#llx R15=%#llx EFL=%#llx\n",
          (unsigned long long)g[REG_R11],
          (unsigned long long)g[REG_R12],
          (unsigned long long)g[REG_R13],
          (unsigned long long)g[REG_R14],
          (unsigned long long)g[REG_R15],
          (unsigned long long)g[REG_EFL]);
#endif
  _exit(139);
}

static void install_segv_oracle(void) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_sigaction = segv_handler;
  sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGSEGV, &sa, NULL) < 0) die("sigaction");
}

__attribute__((noreturn, naked))
static void jump_with_rsp(void *new_rsp, void *entry) {
  __asm__ volatile(
    "mov %rdi, %rsp \n"
    "jmp *%rsi      \n"
  );
}

int main(int argc, char **argv) {
  install_segv_oracle();

  if (argc < 2) {
    fprintf(stderr, "usage: %s <hexbytes>\n", argv[0]);
    fprintf(stderr, "example: %s 4c0a4c29\n", argv[0]);
    return 2;
  }

  // ---- input bytes (<= 16) ----
  uint8_t *in = (uint8_t *)calloc(17, 1); // +1 for trailing 0x00
  if (!in) die("calloc");
  size_t in_len = parse_hex(argv[1], in, 16);
  // ensure 0-terminated (already due to calloc)

  fprintf(stderr, "IN = ");
  hexdump(in, in_len);

  // ---- load blob ./exam (128 bytes) ----
  int fd = open("./exam", O_RDONLY);
  if (fd < 0) die("open ./exam");

  uint8_t code[128];
  ssize_t r = read(fd, code, sizeof(code));
  if (r != (ssize_t)sizeof(code)) {
    fprintf(stderr, "read ./exam: got %zd bytes, expected 128\n", r);
    return 1;
  }
  close(fd);

  // ---- map at fixed address ----
  const uintptr_t BASE = 0x100000;
  void *buf = mmap((void *)BASE, 0x1000,
                   PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (buf == MAP_FAILED) die("mmap(MAP_FIXED 0x100000)");

  memcpy(buf, code, sizeof(code));

  // ---- fake stack as expected by stub: pop rsi ; pop rbp ; pop rsi ----
  static char fake_argv0[] = "oracle";
  // Layout: [0]=argc (junk), [1]=argv0 ptr, [2]=argv1 ptr, [3]=argv2 (NULL)
  // stub will do: pop rsi (argc), pop rbp (argv0), pop rsi (argv1=password)
  uint64_t *fake_stack = (uint64_t *)aligned_alloc(16, 0x100);
  if (!fake_stack) die("aligned_alloc");
  memset(fake_stack, 0, 0x100);
  fake_stack[0] = 2;                         // argc
  fake_stack[1] = (uint64_t)(uintptr_t)fake_argv0; // argv[0]
  fake_stack[2] = (uint64_t)(uintptr_t)in;         // argv[1] = password bytes
  fake_stack[3] = 0;                         // argv[2] = NULL

  // ---- jump into blob at base+0x55 ----
  void *entry = (void *)(BASE + 0x55);
  jump_with_rsp(fake_stack, entry);
  return 0;
}

