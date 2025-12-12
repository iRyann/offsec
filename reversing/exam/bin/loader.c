#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    const char *path = "exam";
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return 1;
    }

    size_t sz = st.st_size;
    void *buf = mmap(NULL, sz,
                     PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    ssize_t n = read(fd, buf, sz);
    if (n < 0) {
        perror("read");
        return 1;
    }
    close(fd);

    // On cast en fonction sans arguments et on saute dedans
    void (*code)(void) = buf;
    code();

    return 0;
}

