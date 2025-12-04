diff --git a/analysis/crackme1_notes.md b/analysis/crackme1_notes.md
new file mode 100644
index 0000000000000000000000000000000000000000..d9468c339cc7fda79a7f7a87eba373a7e13fd11c
--- /dev/null
+++ b/analysis/crackme1_notes.md
@@ -0,0 +1,836 @@
+# Crackme1 : Sans_debugger_fixe
+
+## 1. Identification du binaire
+
+- Binaire ciblé : `/bin/Sans_debugger_fixe` (copie locale dans repo).
+- Contexte : instructions AGENTS.md (analyse détaillée, documentation exhaustive).
+
+## Journal d'analyse
+
+### Étape 1 : fingerprint minimal
+bin/Sans_debugger_fixe: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, stripped
+RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
+[31mNo RELRO     [m   [31mNo canary found[m   [31mNX disabled[m   [31mNo PIE       [m   [32mNo RPATH [m  [32mNo RUNPATH [m  [32mNo Symbols	[m  [31mNo[m	0		0		bin/Sans_debugger_fixe
+
+- Commande `file` -> ELF 64-bit, statically linked, stripped.
+- `checksec` : pas de protections (No RELRO, pas de canary, NX désactivé, No PIE, binaire stripé).
+- Remarque : installation préalable de `file` et `checksec` via apt pour respecter le protocole.
+
+### Étape 2 : strings et rabin2
+.shstrtab
+.shstrtab
+.text
+arch     x86
+baddr    0x400000
+binsz    1253
+bintype  elf
+bits     64
+canary   false
+class    ELF64
+crypto   false
+endian   little
+havecode true
+laddr    0x0
+lang     c
+linenum  false
+lsyms    false
+machine  AMD x86-64 architecture
+nx       false
+os       linux
+pic      false
+relocs   false
+rpath    NONE
+sanitize false
+static   true
+stripped true
+subsys   linux
+va       true
+
+- `strings`: quasi aucune chaîne (seulement `.shstrtab` et `.text` visibles). Binaire très épuré/obfusqué.
+- `rabin2 -I`: ELF64 statique, stripped, NX false, pas de relocs, baddr 0x400000, taille 1253 bytes.
+
+### Étape 3 : désassemblage initial (objdump -d)
+
+bin/Sans_debugger_fixe:     file format elf64-x86-64
+
+
+Disassembly of section .text:
+
+0000000000400080 <.text>:
+  400080:	48 31 ed             	xor    %rbp,%rbp
+  400083:	48 8b 3c 24          	mov    (%rsp),%rdi
+  400087:	48 8d 74 24 08       	lea    0x8(%rsp),%rsi
+  40008c:	e8 67 01 00 00       	call   0x4001f8
+  400091:	89 c7                	mov    %eax,%edi
+  400093:	b8 3c 00 00 00       	mov    $0x3c,%eax
+  400098:	0f 05                	syscall
+  40009a:	48 89 f8             	mov    %rdi,%rax
+  40009d:	48 89 f9             	mov    %rdi,%rcx
+  4000a0:	8a 10                	mov    (%rax),%dl
+  4000a2:	84 d2                	test   %dl,%dl
+  4000a4:	74 05                	je     0x4000ab
+  4000a6:	48 ff c0             	inc    %rax
+  4000a9:	eb f5                	jmp    0x4000a0
+  4000ab:	48 29 c8             	sub    %rcx,%rax
+  4000ae:	c3                   	ret
+  4000af:	0f 31                	rdtsc
+  4000b1:	48 c1 e2 20          	shl    $0x20,%rdx
+  4000b5:	48 09 d0             	or     %rdx,%rax
+  4000b8:	c3                   	ret
+  4000b9:	89 f8                	mov    %edi,%eax
+  4000bb:	0f af c7             	imul   %edi,%eax
+  4000be:	05 34 12 00 00       	add    $0x1234,%eax
+  4000c3:	99                   	cltd
+  4000c4:	b9 01 01 00 00       	mov    $0x101,%ecx
+  4000c9:	f7 f9                	idiv   %ecx
+  4000cb:	89 d6                	mov    %edx,%esi
+  4000cd:	89 f0                	mov    %esi,%eax
+  4000cf:	69 c0 b7 7a 00 00    	imul   $0x7ab7,%eax,%eax
+  4000d5:	05 9e 0a 00 00       	add    $0xa9e,%eax
+  4000da:	99                   	cltd
+  4000db:	f7 f9                	idiv   %ecx
+  4000dd:	89 d6                	mov    %edx,%esi
+  4000df:	89 f8                	mov    %edi,%eax
+  4000e1:	0f af c7             	imul   %edi,%eax
+  4000e4:	99                   	cltd
+  4000e5:	f7 f9                	idiv   %ecx
+  4000e7:	39 d6                	cmp    %edx,%esi
+  4000e9:	0f 94 c0             	sete   %al
+  4000ec:	0f b6 c0             	movzbl %al,%eax
+  4000ef:	c3                   	ret
+  4000f0:	bf e2 01 00 00       	mov    $0x1e2,%edi
+  4000f5:	e8 bf ff ff ff       	call   0x4000b9
+  4000fa:	89 c6                	mov    %eax,%esi
+  4000fc:	31 ff                	xor    %edi,%edi
+  4000fe:	31 d2                	xor    %edx,%edx
+  400100:	45 31 d2             	xor    %r10d,%r10d
+  400103:	b8 65 00 00 00       	mov    $0x65,%eax
+  400108:	0f 05                	syscall
+  40010a:	c3                   	ret
+  40010b:	b8 a6 29 62 07       	mov    $0x76229a6,%eax
+  400110:	0f af c7             	imul   %edi,%eax
+  400113:	99                   	cltd
+  400114:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  400119:	f7 f9                	idiv   %ecx
+  40011b:	89 d6                	mov    %edx,%esi
+  40011d:	b8 9a 3f 2a 07       	mov    $0x72a3f9a,%eax
+  400122:	0f af c7             	imul   %edi,%eax
+  400125:	99                   	cltd
+  400126:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  40012b:	f7 f9                	idiv   %ecx
+  40012d:	41 89 d0             	mov    %edx,%r8d
+  400130:	b8 fe 28 d0 06       	mov    $0x6d028fe,%eax
+  400135:	0f af c7             	imul   %edi,%eax
+  400138:	99                   	cltd
+  400139:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  40013e:	f7 f9                	idiv   %ecx
+  400140:	41 89 d1             	mov    %edx,%r9d
+  400143:	b8 6c 39 b6 06       	mov    $0x6b6396c,%eax
+  400148:	0f af c7             	imul   %edi,%eax
+  40014b:	99                   	cltd
+  40014c:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  400151:	f7 f9                	idiv   %ecx
+  400153:	41 89 d2             	mov    %edx,%r10d
+  400156:	b8 18 a1 27 06       	mov    $0x627a118,%eax
+  40015b:	0f af c7             	imul   %edi,%eax
+  40015e:	99                   	cltd
+  40015f:	b9 09 00 00 00       	mov    $0x9,%ecx
+  400164:	f7 f9                	idiv   %ecx
+  400166:	41 89 d3             	mov    %edx,%r11d
+  400169:	89 f0                	mov    %esi,%eax
+  40016b:	44 89 c2             	mov    %r8d,%edx
+  40016e:	6b d2 0a             	imul   $0xa,%edx,%edx
+  400171:	01 d0                	add    %edx,%eax
+  400173:	44 89 ca             	mov    %r9d,%edx
+  400176:	6b d2 64             	imul   $0x64,%edx,%edx
+  400179:	01 d0                	add    %edx,%eax
+  40017b:	44 89 d2             	mov    %r10d,%edx
+  40017e:	69 d2 e8 03 00 00    	imul   $0x3e8,%edx,%edx
+  400184:	01 d0                	add    %edx,%eax
+  400186:	44 89 da             	mov    %r11d,%edx
+  400189:	69 d2 10 27 00 00    	imul   $0x2710,%edx,%edx
+  40018f:	01 d0                	add    %edx,%eax
+  400191:	c3                   	ret
+  400192:	53                   	push   %rbx
+  400193:	48 89 fb             	mov    %rdi,%rbx
+  400196:	48 89 df             	mov    %rbx,%rdi
+  400199:	e8 fc fe ff ff       	call   0x40009a
+  40019e:	48 89 c1             	mov    %rax,%rcx
+  4001a1:	48 0f af c1          	imul   %rcx,%rax
+  4001a5:	48 0f af c1          	imul   %rcx,%rax
+  4001a9:	48 83 c0 04          	add    $0x4,%rax
+  4001ad:	49 89 c0             	mov    %rax,%r8
+  4001b0:	4d 31 c9             	xor    %r9,%r9
+  4001b3:	31 c9                	xor    %ecx,%ecx
+  4001b5:	83 f9 10             	cmp    $0x10,%ecx
+  4001b8:	7d 19                	jge    0x4001d3
+  4001ba:	0f be 04 0b          	movsbl (%rbx,%rcx,1),%eax
+  4001be:	48 63 d0             	movslq %eax,%rdx
+  4001c1:	48 83 ea 10          	sub    $0x10,%rdx
+  4001c5:	48 63 c1             	movslq %ecx,%rax
+  4001c8:	48 0f af d0          	imul   %rax,%rdx
+  4001cc:	49 01 d1             	add    %rdx,%r9
+  4001cf:	ff c1                	inc    %ecx
+  4001d1:	eb e2                	jmp    0x4001b5
+  4001d3:	b8 01 00 00 00       	mov    $0x1,%eax
+  4001d8:	4d 39 c1             	cmp    %r8,%r9
+  4001db:	75 02                	jne    0x4001df
+  4001dd:	31 c0                	xor    %eax,%eax
+  4001df:	5b                   	pop    %rbx
+  4001e0:	c3                   	ret
+  4001e1:	48 83 ec 08          	sub    $0x8,%rsp
+  4001e5:	48 89 e7             	mov    %rsp,%rdi
+  4001e8:	b8 c9 00 00 00       	mov    $0xc9,%eax
+  4001ed:	0f 05                	syscall
+  4001ef:	48 8b 04 24          	mov    (%rsp),%rax
+  4001f3:	48 83 c4 08          	add    $0x8,%rsp
+  4001f7:	c3                   	ret
+  4001f8:	55                   	push   %rbp
+  4001f9:	48 89 e5             	mov    %rsp,%rbp
+  4001fc:	48 81 ec a0 04 00 00 	sub    $0x4a0,%rsp
+  400203:	48 89 7d c8          	mov    %rdi,-0x38(%rbp)
+  400207:	48 89 75 c0          	mov    %rsi,-0x40(%rbp)
+  40020b:	48 c7 45 a8 00 00 00 	movq   $0x0,-0x58(%rbp)
+  400212:	00 
+  400213:	48 c7 85 70 ff ff ff 	movq   $0x0,-0x90(%rbp)
+  40021a:	00 00 00 00 
+  40021e:	48 c7 85 78 ff ff ff 	movq   $0x0,-0x88(%rbp)
+  400225:	00 00 00 00 
+  400229:	48 c7 45 80 00 00 00 	movq   $0x0,-0x80(%rbp)
+  400230:	00 
+  400231:	48 c7 45 88 00 00 00 	movq   $0x0,-0x78(%rbp)
+  400238:	00 
+  400239:	48 c7 45 90 00 00 00 	movq   $0x0,-0x70(%rbp)
+  400240:	00 
+  400241:	e8 9b ff ff ff       	call   0x4001e1
+  400246:	48 89 45 98          	mov    %rax,-0x68(%rbp)
+  40024a:	e8 60 fe ff ff       	call   0x4000af
+  40024f:	b9 a0 86 01 00       	mov    $0x186a0,%ecx
+  400254:	48 31 d2             	xor    %rdx,%rdx
+  400257:	48 f7 f1             	div    %rcx
+  40025a:	48 89 55 a0          	mov    %rdx,-0x60(%rbp)
+  40025e:	48 83 7d c8 02       	cmpq   $0x2,-0x38(%rbp)
+  400263:	0f 8c 64 02 00 00    	jl     0x4004cd
+  400269:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
+  40026d:	48 8b 40 08          	mov    0x8(%rax),%rax
+  400271:	48 89 45 b0          	mov    %rax,-0x50(%rbp)
+  400275:	48 89 c7             	mov    %rax,%rdi
+  400278:	e8 1d fe ff ff       	call   0x40009a
+  40027d:	48 83 f8 06          	cmp    $0x6,%rax
+  400281:	0f 8e 46 02 00 00    	jle    0x4004cd
+  400287:	48 83 f8 10          	cmp    $0x10,%rax
+  40028b:	0f 8f 3c 02 00 00    	jg     0x4004cd
+  400291:	48 8b 7d b0          	mov    -0x50(%rbp),%rdi
+  400295:	e8 f8 fe ff ff       	call   0x400192
+  40029a:	85 c0                	test   %eax,%eax
+  40029c:	0f 85 2b 02 00 00    	jne    0x4004cd
+  4002a2:	c7 45 a8 02 00 00 00 	movl   $0x2,-0x58(%rbp)
+  4002a9:	bf 1b 00 00 00       	mov    $0x1b,%edi
+  4002ae:	e8 06 fe ff ff       	call   0x4000b9
+  4002b3:	41 89 c0             	mov    %eax,%r8d
+  4002b6:	4c 8d 95 60 ff ff ff 	lea    -0xa0(%rbp),%r10
+  4002bd:	41 83 f8 05          	cmp    $0x5,%r8d
+  4002c1:	7d 2b                	jge    0x4002ee
+  4002c3:	48 8b 45 98          	mov    -0x68(%rbp),%rax
+  4002c7:	48 99                	cqto
+  4002c9:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  4002ce:	48 f7 f9             	idiv   %rcx
+  4002d1:	48 89 45 98          	mov    %rax,-0x68(%rbp)
+  4002d5:	b8 04 00 00 00       	mov    $0x4,%eax
+  4002da:	44 29 c0             	sub    %r8d,%eax
+  4002dd:	89 c1                	mov    %eax,%ecx
+  4002df:	89 d0                	mov    %edx,%eax
+  4002e1:	83 c0 30             	add    $0x30,%eax
+  4002e4:	41 88 04 0a          	mov    %al,(%r10,%rcx,1)
+  4002e8:	41 83 c0 01          	add    $0x1,%r8d
+  4002ec:	eb cf                	jmp    0x4002bd
+  4002ee:	e8 fd fd ff ff       	call   0x4000f0
+  4002f3:	83 f8 ff             	cmp    $0xffffffff,%eax
+  4002f6:	74 02                	je     0x4002fa
+  4002f8:	eb 33                	jmp    0x40032d
+  4002fa:	4c 8d 95 60 fb ff ff 	lea    -0x4a0(%rbp),%r10
+  400301:	45 31 c0             	xor    %r8d,%r8d
+  400304:	41 81 f8 ff 00 00 00 	cmp    $0xff,%r8d
+  40030b:	7d 11                	jge    0x40031e
+  40030d:	44 89 c7             	mov    %r8d,%edi
+  400310:	e8 a4 fd ff ff       	call   0x4000b9
+  400315:	43 89 04 82          	mov    %eax,(%r10,%r8,4)
+  400319:	41 ff c0             	inc    %r8d
+  40031c:	eb e6                	jmp    0x400304
+  40031e:	8b 45 a0             	mov    -0x60(%rbp),%eax
+  400321:	89 c7                	mov    %eax,%edi
+  400323:	e8 e3 fd ff ff       	call   0x40010b
+  400328:	41 89 c1             	mov    %eax,%r9d
+  40032b:	eb 0d                	jmp    0x40033a
+  40032d:	bf 01 00 00 00       	mov    $0x1,%edi
+  400332:	e8 d4 fd ff ff       	call   0x40010b
+  400337:	41 89 c1             	mov    %eax,%r9d
+  40033a:	bf 1b 00 00 00       	mov    $0x1b,%edi
+  40033f:	e8 75 fd ff ff       	call   0x4000b9
+  400344:	41 89 c0             	mov    %eax,%r8d
+  400347:	4c 8d 95 60 ff ff ff 	lea    -0xa0(%rbp),%r10
+  40034e:	41 83 f8 05          	cmp    $0x5,%r8d
+  400352:	7d 27                	jge    0x40037b
+  400354:	44 89 c8             	mov    %r9d,%eax
+  400357:	99                   	cltd
+  400358:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  40035d:	f7 f9                	idiv   %ecx
+  40035f:	41 89 c1             	mov    %eax,%r9d
+  400362:	b8 09 00 00 00       	mov    $0x9,%eax
+  400367:	44 29 c0             	sub    %r8d,%eax
+  40036a:	89 c1                	mov    %eax,%ecx
+  40036c:	89 d0                	mov    %edx,%eax
+  40036e:	83 c0 30             	add    $0x30,%eax
+  400371:	41 88 04 0a          	mov    %al,(%r10,%rcx,1)
+  400375:	41 83 c0 01          	add    $0x1,%r8d
+  400379:	eb d3                	jmp    0x40034e
+  40037b:	48 c7 45 80 00 00 00 	movq   $0x0,-0x80(%rbp)
+  400382:	00 
+  400383:	48 c7 45 88 00 00 00 	movq   $0x0,-0x78(%rbp)
+  40038a:	00 
+  40038b:	48 c7 45 90 01 00 00 	movq   $0x1,-0x70(%rbp)
+  400392:	00 
+  400393:	bf 1b 00 00 00       	mov    $0x1b,%edi
+  400398:	e8 1c fd ff ff       	call   0x4000b9
+  40039d:	41 89 c0             	mov    %eax,%r8d
+  4003a0:	4c 8d 8d 60 ff ff ff 	lea    -0xa0(%rbp),%r9
+  4003a7:	4c 8d 95 70 ff ff ff 	lea    -0x90(%rbp),%r10
+  4003ae:	4c 8d 9d 78 ff ff ff 	lea    -0x88(%rbp),%r11
+  4003b5:	41 83 f8 05          	cmp    $0x5,%r8d
+  4003b9:	7d 19                	jge    0x4003d4
+  4003bb:	43 0f b6 04 01       	movzbl (%r9,%r8,1),%eax
+  4003c0:	43 88 04 02          	mov    %al,(%r10,%r8,1)
+  4003c4:	43 0f b6 44 01 05    	movzbl 0x5(%r9,%r8,1),%eax
+  4003ca:	43 88 04 03          	mov    %al,(%r11,%r8,1)
+  4003ce:	41 83 c0 01          	add    $0x1,%r8d
+  4003d2:	eb e1                	jmp    0x4003b5
+  4003d4:	bf 1b 00 00 00       	mov    $0x1b,%edi
+  4003d9:	e8 db fc ff ff       	call   0x4000b9
+  4003de:	41 89 c0             	mov    %eax,%r8d
+  4003e1:	4c 8d 95 70 ff ff ff 	lea    -0x90(%rbp),%r10
+  4003e8:	4c 8d 9d 78 ff ff ff 	lea    -0x88(%rbp),%r11
+  4003ef:	41 83 f8 05          	cmp    $0x5,%r8d
+  4003f3:	7d 3b                	jge    0x400430
+  4003f5:	b8 04 00 00 00       	mov    $0x4,%eax
+  4003fa:	44 29 c0             	sub    %r8d,%eax
+  4003fd:	89 c1                	mov    %eax,%ecx
+  4003ff:	41 0f b6 04 0a       	movzbl (%r10,%rcx,1),%eax
+  400404:	83 e8 30             	sub    $0x30,%eax
+  400407:	8b 55 90             	mov    -0x70(%rbp),%edx
+  40040a:	0f af c2             	imul   %edx,%eax
+  40040d:	01 45 80             	add    %eax,-0x80(%rbp)
+  400410:	41 0f b6 04 0b       	movzbl (%r11,%rcx,1),%eax
+  400415:	83 e8 30             	sub    $0x30,%eax
+  400418:	8b 55 90             	mov    -0x70(%rbp),%edx
+  40041b:	0f af c2             	imul   %edx,%eax
+  40041e:	01 45 88             	add    %eax,-0x78(%rbp)
+  400421:	8b 45 90             	mov    -0x70(%rbp),%eax
+  400424:	6b c0 0a             	imul   $0xa,%eax,%eax
+  400427:	89 45 90             	mov    %eax,-0x70(%rbp)
+  40042a:	41 83 c0 01          	add    $0x1,%r8d
+  40042e:	eb bf                	jmp    0x4003ef
+  400430:	8b 45 80             	mov    -0x80(%rbp),%eax
+  400433:	33 45 88             	xor    -0x78(%rbp),%eax
+  400436:	41 89 c1             	mov    %eax,%r9d
+  400439:	bf 1b 00 00 00       	mov    $0x1b,%edi
+  40043e:	e8 76 fc ff ff       	call   0x4000b9
+  400443:	41 89 c0             	mov    %eax,%r8d
+  400446:	4c 8d 95 60 ff ff ff 	lea    -0xa0(%rbp),%r10
+  40044d:	41 83 f8 05          	cmp    $0x5,%r8d
+  400451:	7d 27                	jge    0x40047a
+  400453:	44 89 c8             	mov    %r9d,%eax
+  400456:	99                   	cltd
+  400457:	b9 0a 00 00 00       	mov    $0xa,%ecx
+  40045c:	f7 f9                	idiv   %ecx
+  40045e:	41 89 c1             	mov    %eax,%r9d
+  400461:	b8 0e 00 00 00       	mov    $0xe,%eax
+  400466:	44 29 c0             	sub    %r8d,%eax
+  400469:	89 c1                	mov    %eax,%ecx
+  40046b:	89 d0                	mov    %edx,%eax
+  40046d:	83 c0 30             	add    $0x30,%eax
+  400470:	41 88 04 0a          	mov    %al,(%r10,%rcx,1)
+  400474:	41 83 c0 01          	add    $0x1,%r8d
+  400478:	eb d3                	jmp    0x40044d
+  40047a:	48 83 7d c8 02       	cmpq   $0x2,-0x38(%rbp)
+  40047f:	7e 36                	jle    0x4004b7
+  400481:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
+  400485:	48 8b 40 10          	mov    0x10(%rax),%rax
+  400489:	48 89 45 b8          	mov    %rax,-0x48(%rbp)
+  40048d:	31 c9                	xor    %ecx,%ecx
+  40048f:	4c 8d 95 60 ff ff ff 	lea    -0xa0(%rbp),%r10
+  400496:	4c 8b 5d b8          	mov    -0x48(%rbp),%r11
+  40049a:	83 f9 0f             	cmp    $0xf,%ecx
+  40049d:	7d 10                	jge    0x4004af
+  40049f:	41 8a 04 0a          	mov    (%r10,%rcx,1),%al
+  4004a3:	41 8a 14 0b          	mov    (%r11,%rcx,1),%dl
+  4004a7:	38 d0                	cmp    %dl,%al
+  4004a9:	75 0c                	jne    0x4004b7
+  4004ab:	ff c1                	inc    %ecx
+  4004ad:	eb eb                	jmp    0x40049a
+  4004af:	8b 45 a8             	mov    -0x58(%rbp),%eax
+  4004b2:	d1 e8                	shr    $1,%eax
+  4004b4:	89 45 a8             	mov    %eax,-0x58(%rbp)
+  4004b7:	8b 7d a8             	mov    -0x58(%rbp),%edi
+  4004ba:	48 8d 35 10 00 00 00 	lea    0x10(%rip),%rsi        # 0x4004d1
+  4004c1:	ba 03 00 00 00       	mov    $0x3,%edx
+  4004c6:	b8 01 00 00 00       	mov    $0x1,%eax
+  4004cb:	0f 05                	syscall
+  4004cd:	31 c0                	xor    %eax,%eax
+  4004cf:	c9                   	leave
+  4004d0:	c3                   	ret
+  4004d1:	4f                   	rex.WRXB
+  4004d2:	4b                   	rex.WXB
+  4004d3:	0a                   	.byte 0xa
+bash: command not found: strace
+execve("./bin/Sans_debugger_fixe", ["./bin/Sans_debugger_fixe"], 0x7ffd538f1ff0 /* 63 vars */) = 0
+time([1764690061 /* 2025-12-02T15:41:01+0000 */]) = 1764690061 (2025-12-02T15:41:01+0000)
+exit(0)                                 = ?
++++ exited with 0 +++
+Couldn't find .dynsym or .dynstr in "/proc/5029/exe"
+0x00400080    1 26           entry0
+0x004001f8   34 729          fcn.004001f8
+0x004000b9    1 55           fcn.004000b9
+0x0040009a    4 21           fcn.0040009a
+0x004001e1    1 23           fcn.004001e1
+0x004000af    1 10           fcn.004000af
+0x00400192    6 79           fcn.00400192
+0x004000f0    1 27           fcn.004000f0
+0x0040010b    1 135          fcn.0040010b
+
+### Étape 4 : désassemblage avancé (r2)
+- `r2 -q -c "aaa; afl"`: fonctions détectées : entry0 (0x400080), fcn.004001f8 (probable main), fcn.004000b9, fcn.0040009a (strlen maison), fcn.004001e1, fcn.004000af (rdtsc), fcn.00400192, fcn.004000f0 (syscall write?), fcn.0040010b (grosse routine calc).
+            [31m[31m; CALL XREF from entry0 @ [31m0x40008c[31m[0m
+[36m┌[0m 729: [31mfcn.004001f8[0m (signed int64_t arg1, int64_t arg2);
+[36m│[0m           [37m; [37mvar [34mint64_t var_4a0h [36m@ rbp-0x4a0[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_a0h [36m@ rbp-0xa0[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_90h [36m@ rbp-0x90[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_88h [36m@ rbp-0x88[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_80h [36m@ rbp-0x80[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_78h [36m@ rbp-0x78[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_70h [36m@ rbp-0x70[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_68h [36m@ rbp-0x68[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_60h [36m@ rbp-0x60[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_58h [36m@ rbp-0x58[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_50h [36m@ rbp-0x50[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_48h [36m@ rbp-0x48[0m
+[36m│[0m           [37m; [37mvar [34mint64_t var_40h [36m@ rbp-0x40[0m
+[36m│[0m           [37m; [37mvar [34msigned int64_t var_38h [36m@ rbp-0x38[0m
+[36m│[0m           [37m; [37marg [34msigned int64_t arg1 [36m@ rdi[0m
+[36m│[0m           [37m; [37marg [34mint64_t arg2 [36m@ rsi[0m
+[36m│[0m           [32m0x004001f8[0m      [33m55[0m             [35mpush[36m rbp[0m[0m[0m
+[36m│[0m           [32m0x004001f9[0m      [33m48[37m89[37me5[0m         [37mmov[36m rbp[0m,[36m[36m rsp[0m[0m[0m
+[36m│[0m           [32m0x004001fc[0m      [33m48[37m81[37mec[37ma0[37m04[32m00[37m.[0m  [33msub[36m rsp[0m,[36m[36m [33m0x4a0[0m[0m[0m
+[36m│[0m           [32m0x00400203[0m      [33m48[37m89[33m7d[37mc8[0m       [37mmov qword[36m [0m[[34mvar_38h[0m][36m[0m,[36m[36m rdi[0m[0m[0m    [34m; arg1[0m
+[36m│[0m           [32m0x00400207[0m      [33m48[37m89[33m75[37mc0[0m       [37mmov qword[36m [0m[[34mvar_40h[0m][36m[0m,[36m[36m rsi[0m[0m[0m    [34m; arg2[0m
+[36m│[0m           [32m0x0040020b[0m      [33m48[37mc7[33m45[37ma8[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_58h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x00400213[0m      [33m48[37mc7[37m85[33m70[31mff[31mff[37m.[0m  [37mmov qword[36m [0m[[34mvar_90h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x0040021e[0m      [33m48[37mc7[37m85[33m78[31mff[31mff[37m.[0m  [37mmov qword[36m [0m[[34mvar_88h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x00400229[0m      [33m48[37mc7[33m45[37m80[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_80h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x00400231[0m      [33m48[37mc7[33m45[37m88[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_78h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x00400239[0m      [33m48[37mc7[33m45[37m90[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_70h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m           [32m0x00400241[0m      [37me8[37m9b[31mff[31mff[31mff[0m     [1;92mcall fcn.004001e1[0m[0m
+[36m│[0m           [32m0x00400246[0m      [33m48[37m89[33m45[37m98[0m       [37mmov qword[36m [0m[[34mvar_68h[0m][36m[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m           [32m0x0040024a[0m      [37me8[33m60[37mfe[31mff[31mff[0m     [1;92mcall fcn.004000af[0m[0m
+[36m│[0m           [32m0x0040024f[0m      [37mb9[37ma0[37m86[37m01[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0x186a0[0m[0m[0m
+[36m│[0m           [32m0x00400254[0m      [33m48[33m31[37md2[0m         [33mxor[36m rdx[0m,[36m[36m rdx[0m[0m[0m
+[36m│[0m           [32m0x00400257[0m      [33m48[37mf7[37mf1[0m         [33mdiv[36m rcx[0m[0m[0m
+[36m│[0m           [32m0x0040025a[0m      [33m48[37m89[33m55[37ma0[0m       [37mmov qword[36m [0m[[34mvar_60h[0m][36m[0m,[36m[36m rdx[0m[0m[0m
+[36m│[0m           [32m0x0040025e[0m      [33m48[37m83[33m7d[37mc8[37m02[0m     [33mcmp qword[36m [0m[[34mvar_38h[0m][36m[0m,[36m[36m [33m2[0m[0m[0m
+[36m│[0m       [36m┌[0m[36m─[0m[36m<[0m [32m0x00400263[0m      [37m0f[37m8c[33m64[37m02[32m00[32m00[0m   [32mjl 0x4004cd[0m[0m
+[36m│[0m       [36m│[0m   [32m0x00400269[0m      [33m48[37m8b[33m45[37mc0[0m       [37mmov[36m rax[0m,[36m qword[36m [0m[[34mvar_40h[0m][36m[0m[0m[0m
+[36m│[0m       [36m│[0m   [32m0x0040026d[0m      [33m48[37m8b[33m40[37m08[0m       [37mmov[36m rax[0m,[36m qword [0m[[36mrax [0m+[36m[36m [33m8[0m][36m[0m[0m[0m
+[36m│[0m       [36m│[0m   [32m0x00400271[0m      [33m48[37m89[33m45[37mb0[0m       [37mmov qword[36m [0m[[34mvar_50h[0m][36m[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m       [36m│[0m   [32m0x00400275[0m      [33m48[37m89[37mc7[0m         [37mmov[36m rdi[0m,[36m[36m rax[0m[0m[0m                [34m; int64_t arg1[0m
+[36m│[0m       [36m│[0m   [32m0x00400278[0m      [37me8[37m1d[37mfe[31mff[31mff[0m     [1;92mcall fcn.0040009a[0m[0m
+[36m│[0m       [36m│[0m   [32m0x0040027d[0m      [33m48[37m83[37mf8[37m06[0m       [33mcmp[36m rax[0m,[36m[36m [33m6[0m[0m[31m                  [31m; 6[0m
+[36m│[0m      [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400281[0m      [37m0f[37m8e[33m46[37m02[32m00[32m00[0m   [32mjle 0x4004cd[0m[0m
+[36m│[0m      [36m│[0m[36m│[0m   [32m0x00400287[0m      [33m48[37m83[37mf8[37m10[0m       [33mcmp[36m rax[0m,[36m[36m [33m0x10[0m[0m[31m               [31m; 16[0m
+[36m│[0m     [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040028b[0m      [37m0f[37m8f[33m3c[37m02[32m00[32m00[0m   [32mjg 0x4004cd[0m[0m
+[36m│[0m     [36m│[0m[36m│[0m[36m│[0m   [32m0x00400291[0m      [33m48[37m8b[33m7d[37mb0[0m       [37mmov[36m rdi[0m,[36m qword[36m [0m[[34mvar_50h[0m][36m[0m[0m[0m    [34m; int64_t arg1[0m
+[36m│[0m     [36m│[0m[36m│[0m[36m│[0m   [32m0x00400295[0m      [37me8[37mf8[37mfe[31mff[31mff[0m     [1;92mcall fcn.00400192[0m[0m
+[36m│[0m     [36m│[0m[36m│[0m[36m│[0m   [32m0x0040029a[0m      [37m85[37mc0[0m           [33mtest[36m eax[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040029c[0m      [37m0f[37m85[33m2b[37m02[32m00[32m00[0m   [32mjne 0x4004cd[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002a2[0m      [37mc7[33m45[37ma8[37m02[32m00[32m00[37m.[0m  [37mmov dword[36m [0m[[34mvar_58h[0m][36m[0m,[36m[36m [33m2[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002a9[0m      [37mbf[37m1b[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m0x1b[0m[0m[31m               [31m; 27[0m [34m; int64_t arg1[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002ae[0m      [37me8[37m06[37mfe[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002b3[0m      [33m41[37m89[37mc0[0m         [37mmov[36m r8d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002b6[0m      [33m4c[37m8d[37m95[33m60[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_a0h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4002ec[31m[0m
+[36m│[0m   [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x004002bd[0m      [33m41[37m83[37mf8[37m05[0m       [33mcmp[36m r8d[0m,[36m[36m [33m5[0m[0m[31m                  [31m; 5[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004002c1[0m      [33m7d[33m2b[0m           [32mjge 0x4002ee[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002c3[0m      [33m48[37m8b[33m45[37m98[0m       [37mmov[36m rax[0m,[36m qword[36m [0m[[34mvar_68h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002c7[0m      [33m48[37m99[0m           [37mcqo[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002c9[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002ce[0m      [33m48[37mf7[37mf9[0m         [33midiv[36m rcx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002d1[0m      [33m48[37m89[33m45[37m98[0m       [37mmov qword[36m [0m[[34mvar_68h[0m][36m[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002d5[0m      [37mb8[37m04[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m4[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002da[0m      [33m44[33m29[37mc0[0m         [33msub[36m eax[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002dd[0m      [37m89[37mc1[0m           [37mmov[36m ecx[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002df[0m      [37m89[37md0[0m           [37mmov[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002e1[0m      [37m83[37mc0[33m30[0m         [33madd[36m eax[0m,[36m[36m [33m0x30[0m[0m[31m               [31m; 48[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002e4[0m      [33m41[37m88[37m04[37m0a[0m       [37mmov byte [0m[[36mr10 [0m+[36m[36m rcx[0m][36m[0m,[36m[36m al[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002e8[0m      [33m41[37m83[37mc0[37m01[0m       [33madd[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m  [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x004002ec[0m      [37meb[37mcf[0m           [32mjmp 0x4002bd[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4002c1[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004002ee[0m      [37me8[37mfd[37mfd[31mff[31mff[0m     [1;92mcall fcn.004000f0[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004002f3[0m      [37m83[37mf8[31mff[0m         [33mcmp[36m eax[0m,[36m[36m [33m0xffffffff[0m[0m[0m
+[36m│[0m   [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004002f6[0m      [33m74[37m02[0m           [32mje 0x4002fa[0m[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004002f8[0m      [37meb[33m33[0m           [32mjmp 0x40032d[0m[0m
+[36m│[0m  [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4002f6[31m[0m
+[36m│[0m  [36m│[0m[36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004002fa[0m      [33m4c[37m8d[37m95[33m60[37mfb[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_4a0h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400301[0m      [33m45[33m31[37mc0[0m         [33mxor[36m r8d[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x40031c[31m[0m
+[36m│[0m  [36m│[0m[34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x00400304[0m      [33m41[37m81[37mf8[31mff[32m00[32m00[37m.[0m  [33mcmp[36m r8d[0m,[36m[36m [33m0xff[0m[0m[31m               [31m; 255[0m
+[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040030b[0m      [33m7d[37m11[0m           [32mjge 0x40031e[0m[0m
+[36m│[0m [36m│[0m[36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040030d[0m      [33m44[37m89[37mc7[0m         [37mmov[36m edi[0m,[36m[36m r8d[0m[0m[0m                [34m; int64_t arg1[0m
+[36m│[0m [36m│[0m[36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400310[0m      [37me8[37ma4[37mfd[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m [36m│[0m[36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400315[0m      [33m43[37m89[37m04[37m82[0m       [37mmov dword [0m[[36mr10 [0m+[36m[36m r8[0m*[33m4[0m][36m[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m [36m│[0m[36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400319[0m      [33m41[31mff[37mc0[0m         [33minc[36m r8d[0m[0m[0m
+[36m│[0m [36m│[0m[36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x0040031c[0m      [37meb[37me6[0m           [32mjmp 0x400304[0m[0m
+[36m│[0m [36m│[0m[36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x40030b[31m[0m
+[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040031e[0m      [37m8b[33m45[37ma0[0m         [37mmov[36m eax[0m,[36m dword[36m [0m[[34mvar_60h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400321[0m      [37m89[37mc7[0m           [37mmov[36m edi[0m,[36m[36m eax[0m[0m[0m                [34m; int64_t arg3[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400323[0m      [37me8[37me3[37mfd[31mff[31mff[0m     [1;92mcall fcn.0040010b[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400328[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040032b[0m      [37meb[37m0d[0m           [32mjmp 0x40033a[0m[0m
+[36m│[0m  [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4002f8[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040032d[0m      [37mbf[37m01[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m1[0m[0m[0m                  [34m; int64_t arg3[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400332[0m      [37me8[37md4[37mfd[31mff[31mff[0m     [1;92mcall fcn.0040010b[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400337[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x40032b[31m[0m
+[36m│[0m   [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040033a[0m      [37mbf[37m1b[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m0x1b[0m[0m[31m               [31m; 27[0m [34m; int64_t arg1[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040033f[0m      [37me8[33m75[37mfd[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400344[0m      [33m41[37m89[37mc0[0m         [37mmov[36m r8d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400347[0m      [33m4c[37m8d[37m95[33m60[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_a0h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x400379[31m[0m
+[36m│[0m   [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x0040034e[0m      [33m41[37m83[37mf8[37m05[0m       [33mcmp[36m r8d[0m,[36m[36m [33m5[0m[0m[31m                  [31m; 5[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400352[0m      [33m7d[33m27[0m           [32mjge 0x40037b[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400354[0m      [33m44[37m89[37mc8[0m         [37mmov[36m eax[0m,[36m[36m r9d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400357[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400358[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040035d[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040035f[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400362[0m      [37mb8[37m09[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m9[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400367[0m      [33m44[33m29[37mc0[0m         [33msub[36m eax[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040036a[0m      [37m89[37mc1[0m           [37mmov[36m ecx[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040036c[0m      [37m89[37md0[0m           [37mmov[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040036e[0m      [37m83[37mc0[33m30[0m         [33madd[36m eax[0m,[36m[36m [33m0x30[0m[0m[31m               [31m; 48[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400371[0m      [33m41[37m88[37m04[37m0a[0m       [37mmov byte [0m[[36mr10 [0m+[36m[36m rcx[0m][36m[0m,[36m[36m al[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400375[0m      [33m41[37m83[37mc0[37m01[0m       [33madd[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m  [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x00400379[0m      [37meb[37md3[0m           [32mjmp 0x40034e[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x400352[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040037b[0m      [33m48[37mc7[33m45[37m80[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_80h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400383[0m      [33m48[37mc7[33m45[37m88[32m00[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_78h[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040038b[0m      [33m48[37mc7[33m45[37m90[37m01[32m00[37m.[0m  [37mmov qword[36m [0m[[34mvar_70h[0m][36m[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400393[0m      [37mbf[37m1b[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m0x1b[0m[0m[31m               [31m; 27[0m [34m; int64_t arg1[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400398[0m      [37me8[37m1c[37mfd[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040039d[0m      [33m41[37m89[37mc0[0m         [37mmov[36m r8d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003a0[0m      [33m4c[37m8d[37m8d[33m60[31mff[31mff[37m.[0m  [37mlea[36m r9[0m,[36m[36m [0m[[34mvar_a0h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003a7[0m      [33m4c[37m8d[37m95[33m70[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_90h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003ae[0m      [33m4c[37m8d[37m9d[33m78[31mff[31mff[37m.[0m  [37mlea[36m r11[0m,[36m[36m [0m[[34mvar_88h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4003d2[31m[0m
+[36m│[0m   [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x004003b5[0m      [33m41[37m83[37mf8[37m05[0m       [33mcmp[36m r8d[0m,[36m[36m [33m5[0m[0m[31m                  [31m; 5[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004003b9[0m      [33m7d[37m19[0m           [32mjge 0x4003d4[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003bb[0m      [33m43[37m0f[37mb6[37m04[37m01[0m     [37mmovzx[36m eax[0m,[36m byte [0m[[36mr9 [0m+[36m[36m r8[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003c0[0m      [33m43[37m88[37m04[37m02[0m       [37mmov byte [0m[[36mr10 [0m+[36m[36m r8[0m][36m[0m,[36m[36m al[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003c4[0m      [33m43[37m0f[37mb6[33m44[37m01[37m05[0m   [37mmovzx[36m eax[0m,[36m byte [0m[[36mr9 [0m+[36m r8 [0m+[36m[36m [33m5[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003ca[0m      [33m43[37m88[37m04[37m03[0m       [37mmov byte [0m[[36mr11 [0m+[36m[36m r8[0m][36m[0m,[36m[36m al[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003ce[0m      [33m41[37m83[37mc0[37m01[0m       [33madd[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m  [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x004003d2[0m      [37meb[37me1[0m           [32mjmp 0x4003b5[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4003b9[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004003d4[0m      [37mbf[37m1b[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m0x1b[0m[0m[31m               [31m; 27[0m [34m; int64_t arg1[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003d9[0m      [37me8[37mdb[37mfc[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003de[0m      [33m41[37m89[37mc0[0m         [37mmov[36m r8d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003e1[0m      [33m4c[37m8d[37m95[33m70[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_90h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003e8[0m      [33m4c[37m8d[37m9d[33m78[31mff[31mff[37m.[0m  [37mlea[36m r11[0m,[36m[36m [0m[[34mvar_88h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x40042e[31m[0m
+[36m│[0m   [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x004003ef[0m      [33m41[37m83[37mf8[37m05[0m       [33mcmp[36m r8d[0m,[36m[36m [33m5[0m[0m[31m                  [31m; 5[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004003f3[0m      [33m7d[33m3b[0m           [32mjge 0x400430[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003f5[0m      [37mb8[37m04[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m4[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003fa[0m      [33m44[33m29[37mc0[0m         [33msub[36m eax[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003fd[0m      [37m89[37mc1[0m           [37mmov[36m ecx[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004003ff[0m      [33m41[37m0f[37mb6[37m04[37m0a[0m     [37mmovzx[36m eax[0m,[36m byte [0m[[36mr10 [0m+[36m[36m rcx[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400404[0m      [37m83[37me8[33m30[0m         [33msub[36m eax[0m,[36m[36m [33m0x30[0m[0m[31m               [31m; 48[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400407[0m      [37m8b[33m55[37m90[0m         [37mmov[36m edx[0m,[36m dword[36m [0m[[34mvar_70h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040040a[0m      [37m0f[37maf[37mc2[0m         [33mimul[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040040d[0m      [37m01[33m45[37m80[0m         [33madd dword[36m [0m[[34mvar_80h[0m][36m[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400410[0m      [33m41[37m0f[37mb6[37m04[37m0b[0m     [37mmovzx[36m eax[0m,[36m byte [0m[[36mr11 [0m+[36m[36m rcx[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400415[0m      [37m83[37me8[33m30[0m         [33msub[36m eax[0m,[36m[36m [33m0x30[0m[0m[31m               [31m; 48[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400418[0m      [37m8b[33m55[37m90[0m         [37mmov[36m edx[0m,[36m dword[36m [0m[[34mvar_70h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040041b[0m      [37m0f[37maf[37mc2[0m         [33mimul[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040041e[0m      [37m01[33m45[37m88[0m         [33madd dword[36m [0m[[34mvar_78h[0m][36m[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400421[0m      [37m8b[33m45[37m90[0m         [37mmov[36m eax[0m,[36m dword[36m [0m[[34mvar_70h[0m][36m[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400424[0m      [33m6b[37mc0[37m0a[0m         [33mimul[36m eax[0m,[36m[36m eax[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400427[0m      [37m89[33m45[37m90[0m         [37mmov dword[36m [0m[[34mvar_70h[0m][36m[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040042a[0m      [33m41[37m83[37mc0[37m01[0m       [33madd[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m  [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x0040042e[0m      [37meb[37mbf[0m           [32mjmp 0x4003ef[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4003f3[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x00400430[0m      [37m8b[33m45[37m80[0m         [37mmov[36m eax[0m,[36m dword[36m [0m[[34mvar_80h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400433[0m      [33m33[33m45[37m88[0m         [33mxor[36m eax[0m,[36m dword[36m [0m[[34mvar_78h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400436[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400439[0m      [37mbf[37m1b[32m00[32m00[32m00[0m     [37mmov[36m edi[0m,[36m[36m [33m0x1b[0m[0m[31m               [31m; 27[0m [34m; int64_t arg1[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040043e[0m      [37me8[33m76[37mfc[31mff[31mff[0m     [1;92mcall fcn.004000b9[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400443[0m      [33m41[37m89[37mc0[0m         [37mmov[36m r8d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400446[0m      [33m4c[37m8d[37m95[33m60[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_a0h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x400478[31m[0m
+[36m│[0m   [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x0040044d[0m      [33m41[37m83[37mf8[37m05[0m       [33mcmp[36m r8d[0m,[36m[36m [33m5[0m[0m[31m                  [31m; 5[0m
+[36m│[0m  [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400451[0m      [33m7d[33m27[0m           [32mjge 0x40047a[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400453[0m      [33m44[37m89[37mc8[0m         [37mmov[36m eax[0m,[36m[36m r9d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400456[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400457[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040045c[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040045e[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400461[0m      [37mb8[37m0e[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m0xe[0m[0m[31m                [31m; 14[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400466[0m      [33m44[33m29[37mc0[0m         [33msub[36m eax[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400469[0m      [37m89[37mc1[0m           [37mmov[36m ecx[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040046b[0m      [37m89[37md0[0m           [37mmov[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040046d[0m      [37m83[37mc0[33m30[0m         [33madd[36m eax[0m,[36m[36m [33m0x30[0m[0m[31m               [31m; 48[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400470[0m      [33m41[37m88[37m04[37m0a[0m       [37mmov byte [0m[[36mr10 [0m+[36m[36m rcx[0m][36m[0m,[36m[36m al[0m[0m[0m
+[36m│[0m  [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400474[0m      [33m41[37m83[37mc0[37m01[0m       [33madd[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m  [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x00400478[0m      [37meb[37md3[0m           [32mjmp 0x40044d[0m[0m
+[36m│[0m  [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x400451[31m[0m
+[36m│[0m  [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040047a[0m      [33m48[37m83[33m7d[37mc8[37m02[0m     [33mcmp qword[36m [0m[[34mvar_38h[0m][36m[0m,[36m[36m [33m2[0m[0m[0m
+[36m│[0m   [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040047f[0m      [33m7e[33m36[0m           [32mjle 0x4004b7[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400481[0m      [33m48[37m8b[33m45[37mc0[0m       [37mmov[36m rax[0m,[36m qword[36m [0m[[34mvar_40h[0m][36m[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400485[0m      [33m48[37m8b[33m40[37m10[0m       [37mmov[36m rax[0m,[36m qword [0m[[36mrax [0m+[36m[36m [33m0x10[0m][36m[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400489[0m      [33m48[37m89[33m45[37mb8[0m       [37mmov qword[36m [0m[[34mvar_48h[0m][36m[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040048d[0m      [33m31[37mc9[0m           [33mxor[36m ecx[0m,[36m[36m ecx[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040048f[0m      [33m4c[37m8d[37m95[33m60[31mff[31mff[37m.[0m  [37mlea[36m r10[0m,[36m[36m [0m[[34mvar_a0h[0m][36m[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400496[0m      [33m4c[37m8b[33m5d[37mb8[0m       [37mmov[36m r11[0m,[36m qword[36m [0m[[34mvar_48h[0m][36m[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x4004ad[31m[0m
+[36m│[0m  [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x0040049a[0m      [37m83[37mf9[37m0f[0m         [33mcmp[36m ecx[0m,[36m[36m [33m0xf[0m[0m[31m                [31m; 15[0m
+[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040049d[0m      [33m7d[37m10[0m           [32mjge 0x4004af[0m[0m
+[36m│[0m [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x0040049f[0m      [33m41[37m8a[37m04[37m0a[0m       [37mmov[36m al[0m,[36m byte [0m[[36mr10 [0m+[36m[36m rcx[0m][36m[0m[0m[0m
+[36m│[0m [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004a3[0m      [33m41[37m8a[37m14[37m0b[0m       [37mmov[36m dl[0m,[36m byte [0m[[36mr11 [0m+[36m[36m rcx[0m][36m[0m[0m[0m
+[36m│[0m [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004a7[0m      [33m38[37md0[0m           [33mcmp[36m al[0m,[36m[36m dl[0m[0m[0m
+[36m│[0m [36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004004a9[0m      [33m75[37m0c[0m           [32mjne 0x4004b7[0m[0m
+[36m│[0m [36m│[0m[34m╎[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004ab[0m      [31mff[37mc1[0m           [33minc[36m ecx[0m[0m[0m
+[36m│[0m [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x004004ad[0m      [37meb[37meb[0m           [32mjmp 0x40049a[0m[0m
+[36m│[0m [36m│[0m [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREF from fcn.004001f8 @ [31m0x40049d[31m[0m
+[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004004af[0m      [37m8b[33m45[37ma8[0m         [37mmov[36m eax[0m,[36m dword[36m [0m[[34mvar_58h[0m][36m[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004b2[0m      [37md1[37me8[0m           [33mshr[36m eax[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004b4[0m      [37m89[33m45[37ma8[0m         [37mmov dword[36m [0m[[34mvar_58h[0m][36m[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m   [36m│[0m[36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREFS from fcn.004001f8 @ [31m0x40047f[31m, 0x4004a9[31m[0m
+[36m│[0m [36m─[0m[36m─[0m[36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004004b7[0m      [37m8b[33m7d[37ma8[0m         [37mmov[36m edi[0m,[36m dword[36m [0m[[34mvar_58h[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004ba[0m      [33m48[37m8d[33m35[37m10[32m00[32m00[37m.[0m  [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x004004d1[0m][36m[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004c1[0m      [37mba[37m03[32m00[32m00[32m00[0m     [37mmov[36m edx[0m,[36m[36m [33m3[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004c6[0m      [37mb8[37m01[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x004004cb[0m      [37m0f[37m05[0m           [35msyscall[0m[0m[0m
+[36m│[0m    [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [31m[31m; CODE XREFS from fcn.004001f8 @ [31m0x400263[31m, 0x400281[31m, 0x40028b[31m, 0x40029c[31m[0m
+[36m│[0m    [36m└[0m[36m└[0m[36m└[0m[36m└[0m[36m─[0m[36m>[0m [32m0x004004cd[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m           [32m0x004004cf[0m      [37mc9[0m             [35mleave[0m[0m[0m
+[36m└[0m           [32m0x004004d0[0m      [37mc3[0m             [31mret[0m[0m[0m
+            [31m[31m; CALL XREFS from fcn.004001f8 @ [31m0x400323[31m, 0x400332[31m[0m
+[36m┌[0m 135: [31mfcn.0040010b[0m (int64_t arg3);
+[36m│[0m           [37m; [37marg [34mint64_t arg3 [36m@ rdx[0m
+[36m│[0m           [32m0x0040010b[0m      [37mb8[37ma6[33m29[33m62[37m07[0m     [37mmov[36m eax[0m,[36m[36m [33m0x76229a6[0m[0m[0m
+[36m│[0m           [32m0x00400110[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x00400113[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x00400114[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m           [32m0x00400119[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x0040011b[0m      [37m89[37md6[0m           [37mmov[36m esi[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x0040011d[0m      [37mb8[37m9a[33m3f[33m2a[37m07[0m     [37mmov[36m eax[0m,[36m[36m [33m0x72a3f9a[0m[0m[0m
+[36m│[0m           [32m0x00400122[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x00400125[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x00400126[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m           [32m0x0040012b[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x0040012d[0m      [33m41[37m89[37md0[0m         [37mmov[36m r8d[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x00400130[0m      [37mb8[37mfe[33m28[37md0[37m06[0m     [37mmov[36m eax[0m,[36m[36m [33m0x6d028fe[0m[0m[0m
+[36m│[0m           [32m0x00400135[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x00400138[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x00400139[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m           [32m0x0040013e[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x00400140[0m      [33m41[37m89[37md1[0m         [37mmov[36m r9d[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x00400143[0m      [37mb8[33m6c[33m39[37mb6[37m06[0m     [37mmov[36m eax[0m,[36m[36m [33m0x6b6396c[0m[0m[0m
+[36m│[0m           [32m0x00400148[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x0040014b[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x0040014c[0m      [37mb9[37m0a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m           [32m0x00400151[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x00400153[0m      [33m41[37m89[37md2[0m         [37mmov[36m r10d[0m,[36m[36m edx[0m[0m[0m               [34m; arg3[0m
+[36m│[0m           [32m0x00400156[0m      [37mb8[37m18[37ma1[33m27[37m06[0m     [37mmov[36m eax[0m,[36m[36m [33m0x627a118[0m[0m[0m
+[36m│[0m           [32m0x0040015b[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x0040015e[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x0040015f[0m      [37mb9[37m09[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m9[0m[0m[0m
+[36m│[0m           [32m0x00400164[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x00400166[0m      [33m41[37m89[37md3[0m         [37mmov[36m r11d[0m,[36m[36m edx[0m[0m[0m               [34m; arg3[0m
+[36m│[0m           [32m0x00400169[0m      [37m89[37mf0[0m           [37mmov[36m eax[0m,[36m[36m esi[0m[0m[0m
+[36m│[0m           [32m0x0040016b[0m      [33m44[37m89[37mc2[0m         [37mmov[36m edx[0m,[36m[36m r8d[0m[0m[0m
+[36m│[0m           [32m0x0040016e[0m      [33m6b[37md2[37m0a[0m         [33mimul[36m edx[0m,[36m[36m edx[0m,[36m[36m [33m0xa[0m[0m[0m
+[36m│[0m           [32m0x00400171[0m      [37m01[37md0[0m           [33madd[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m           [32m0x00400173[0m      [33m44[37m89[37mca[0m         [37mmov[36m edx[0m,[36m[36m r9d[0m[0m[0m
+[36m│[0m           [32m0x00400176[0m      [33m6b[37md2[33m64[0m         [33mimul[36m edx[0m,[36m[36m edx[0m,[36m[36m [33m0x64[0m[0m[0m
+[36m│[0m           [32m0x00400179[0m      [37m01[37md0[0m           [33madd[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m           [32m0x0040017b[0m      [33m44[37m89[37md2[0m         [37mmov[36m edx[0m,[36m[36m r10d[0m[0m[0m
+[36m│[0m           [32m0x0040017e[0m      [33m69[37md2[37me8[37m03[32m00[32m00[0m   [33mimul[36m edx[0m,[36m[36m edx[0m,[36m[36m [33m0x3e8[0m[0m[0m
+[36m│[0m           [32m0x00400184[0m      [37m01[37md0[0m           [33madd[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m│[0m           [32m0x00400186[0m      [33m44[37m89[37mda[0m         [37mmov[36m edx[0m,[36m[36m r11d[0m[0m[0m
+[36m│[0m           [32m0x00400189[0m      [33m69[37md2[37m10[33m27[32m00[32m00[0m   [33mimul[36m edx[0m,[36m[36m edx[0m,[36m[36m [33m0x2710[0m[0m[0m
+[36m│[0m           [32m0x0040018f[0m      [37m01[37md0[0m           [33madd[36m eax[0m,[36m[36m edx[0m[0m[0m
+[36m└[0m           [32m0x00400191[0m      [37mc3[0m             [31mret[0m[0m[0m
+            [31m[31m; CALL XREF from fcn.004001f8 @ [31m0x400295[31m[0m
+[36m┌[0m 79: [31mfcn.00400192[0m (int64_t arg1);
+[36m│[0m           [37m; [37marg [34mint64_t arg1 [36m@ rdi[0m
+[36m│[0m           [32m0x00400192[0m      [33m53[0m             [35mpush[36m rbx[0m[0m[0m
+[36m│[0m           [32m0x00400193[0m      [33m48[37m89[37mfb[0m         [37mmov[36m rbx[0m,[36m[36m rdi[0m[0m[0m                [34m; arg1[0m
+[36m│[0m           [32m0x00400196[0m      [33m48[37m89[37mdf[0m         [37mmov[36m rdi[0m,[36m[36m rbx[0m[0m[0m
+[36m│[0m           [32m0x00400199[0m      [37me8[37mfc[37mfe[31mff[31mff[0m     [1;92mcall fcn.0040009a[0m[0m
+[36m│[0m           [32m0x0040019e[0m      [33m48[37m89[37mc1[0m         [37mmov[36m rcx[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m           [32m0x004001a1[0m      [33m48[37m0f[37maf[37mc1[0m       [33mimul[36m rax[0m,[36m[36m rcx[0m[0m[0m
+[36m│[0m           [32m0x004001a5[0m      [33m48[37m0f[37maf[37mc1[0m       [33mimul[36m rax[0m,[36m[36m rcx[0m[0m[0m
+[36m│[0m           [32m0x004001a9[0m      [33m48[37m83[37mc0[37m04[0m       [33madd[36m rax[0m,[36m[36m [33m4[0m[0m[0m
+[36m│[0m           [32m0x004001ad[0m      [33m49[37m89[37mc0[0m         [37mmov[36m r8[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m           [32m0x004001b0[0m      [33m4d[33m31[37mc9[0m         [33mxor[36m r9[0m,[36m[36m r9[0m[0m[0m
+[36m│[0m           [32m0x004001b3[0m      [33m31[37mc9[0m           [33mxor[36m ecx[0m,[36m[36m ecx[0m[0m[0m
+[36m│[0m           [31m[31m; CODE XREF from fcn.00400192 @ [31m0x4001d1[31m[0m
+[36m│[0m       [34m┌[0m[34m─[0m[34m>[0m [32m0x004001b5[0m      [37m83[37mf9[37m10[0m         [33mcmp[36m ecx[0m,[36m[36m [33m0x10[0m[0m[31m               [31m; 16[0m
+[36m│[0m      [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004001b8[0m      [33m7d[37m19[0m           [32mjge 0x4001d3[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001ba[0m      [37m0f[37mbe[37m04[37m0b[0m       [37mmovsx[36m eax[0m,[36m byte [0m[[36mrbx [0m+[36m[36m rcx[0m][36m[0m[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001be[0m      [33m48[33m63[37md0[0m         [37mmovsxd[36m rdx[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001c1[0m      [33m48[37m83[37mea[37m10[0m       [33msub[36m rdx[0m,[36m[36m [33m0x10[0m[0m[31m               [31m; 16[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001c5[0m      [33m48[33m63[37mc1[0m         [37mmovsxd[36m rax[0m,[36m[36m ecx[0m[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001c8[0m      [33m48[37m0f[37maf[37md0[0m       [33mimul[36m rdx[0m,[36m[36m rax[0m[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001cc[0m      [33m49[37m01[37md1[0m         [33madd[36m r9[0m,[36m[36m rdx[0m[0m[0m
+[36m│[0m      [36m│[0m[34m╎[0m   [32m0x004001cf[0m      [31mff[37mc1[0m           [33minc[36m ecx[0m[0m[0m
+[36m│[0m      [36m│[0m[34m└[0m[34m─[0m[34m<[0m [32m0x004001d1[0m      [37meb[37me2[0m           [32mjmp 0x4001b5[0m[0m
+[36m│[0m      [36m│[0m    [31m[31m; CODE XREF from fcn.00400192 @ [31m0x4001b8[31m[0m
+[36m│[0m      [36m└[0m[36m─[0m[36m─[0m[36m>[0m [32m0x004001d3[0m      [37mb8[37m01[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m1[0m[0m[0m
+[36m│[0m           [32m0x004001d8[0m      [33m4d[33m39[37mc1[0m         [33mcmp[36m r9[0m,[36m[36m r8[0m[0m[0m
+[36m│[0m       [36m┌[0m[36m─[0m[36m<[0m [32m0x004001db[0m      [33m75[37m02[0m           [32mjne 0x4001df[0m[0m
+[36m│[0m       [36m│[0m   [32m0x004001dd[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
+[36m│[0m       [36m│[0m   [31m[31m; CODE XREF from fcn.00400192 @ [31m0x4001db[31m[0m
+[36m│[0m       [36m└[0m[36m─[0m[36m>[0m [32m0x004001df[0m      [33m5b[0m             [35mpop[36m rbx[0m[0m[0m
+[36m└[0m           [32m0x004001e0[0m      [37mc3[0m             [31mret[0m[0m[0m
+            [31m[31m; XREFS: [31mCALL 0x004000f5  [31mCALL 0x004002ae  [31mCALL 0x00400310  [0m
+            [31m[31m; XREFS: [31mCALL 0x0040033f  [31mCALL 0x00400398  [31mCALL 0x004003d9  [0m
+            [31m[31m; XREFS: [31mCALL 0x0040043e  [0m
+[36m┌[0m 55: [31mfcn.004000b9[0m (int64_t arg1, uint32_t arg3);
+[36m│[0m           [37m; [37marg [34mint64_t arg1 [36m@ rdi[0m
+[36m│[0m           [37m; [37marg [34muint32_t arg3 [36m@ rdx[0m
+[36m│[0m           [32m0x004000b9[0m      [37m89[37mf8[0m           [37mmov[36m eax[0m,[36m[36m edi[0m[0m[0m                [34m; arg1[0m
+[36m│[0m           [32m0x004000bb[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x004000be[0m      [37m05[33m34[37m12[32m00[32m00[0m     [33madd[36m eax[0m,[36m[36m [33m0x1234[0m[0m[0m
+[36m│[0m           [32m0x004000c3[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x004000c4[0m      [37mb9[37m01[37m01[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0x101[0m[0m[31m              [31m; 257[0m
+[36m│[0m           [32m0x004000c9[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x004000cb[0m      [37m89[37md6[0m           [37mmov[36m esi[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x004000cd[0m      [37m89[37mf0[0m           [37mmov[36m eax[0m,[36m[36m esi[0m[0m[0m
+[36m│[0m           [32m0x004000cf[0m      [33m69[37mc0[37mb7[33m7a[32m00[32m00[0m   [33mimul[36m eax[0m,[36m[36m eax[0m,[36m[36m [33m0x7ab7[0m[0m[0m
+[36m│[0m           [32m0x004000d5[0m      [37m05[37m9e[37m0a[32m00[32m00[0m     [33madd[36m eax[0m,[36m[36m [33m0xa9e[0m[0m[31m              [31m; 2718[0m
+[36m│[0m           [32m0x004000da[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x004000db[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x004000dd[0m      [37m89[37md6[0m           [37mmov[36m esi[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x004000df[0m      [37m89[37mf8[0m           [37mmov[36m eax[0m,[36m[36m edi[0m[0m[0m                [34m; arg1[0m
+[36m│[0m           [32m0x004000e1[0m      [37m0f[37maf[37mc7[0m         [33mimul[36m eax[0m,[36m[36m edi[0m[0m[0m
+[36m│[0m           [32m0x004000e4[0m      [37m99[0m             [37mcdq[0m[0m[0m
+[36m│[0m           [32m0x004000e5[0m      [37mf7[37mf9[0m           [33midiv[36m ecx[0m[0m[0m
+[36m│[0m           [32m0x004000e7[0m      [33m39[37md6[0m           [33mcmp[36m esi[0m,[36m[36m edx[0m[0m[0m                [34m; arg3[0m
+[36m│[0m           [32m0x004000e9[0m      [37m0f[37m94[37mc0[0m         [37msete[36m al[0m[0m[0m
+[36m│[0m           [32m0x004000ec[0m      [37m0f[37mb6[37mc0[0m         [37mmovzx[36m eax[0m,[36m[36m al[0m[0m[0m
+[36m└[0m           [32m0x004000ef[0m      [37mc3[0m             [31mret[0m[0m[0m
+            [31m[31m; CALL XREF from fcn.004001f8 @ [31m0x400241[31m[0m
+[36m┌[0m 23: [31mfcn.004001e1[0m ();
+[36m│[0m           [32m0x004001e1[0m      [33m48[37m83[37mec[37m08[0m       [33msub[36m rsp[0m,[36m[36m [33m8[0m[0m[0m
+[36m│[0m           [32m0x004001e5[0m      [33m48[37m89[37me7[0m         [37mmov[36m rdi[0m,[36m[36m rsp[0m[0m[0m
+[36m│[0m           [32m0x004001e8[0m      [37mb8[37mc9[32m00[32m00[32m00[0m     [37mmov[36m eax[0m,[36m[36m [33m0xc9[0m[0m[31m               [31m; 201[0m
+[36m│[0m           [32m0x004001ed[0m      [37m0f[37m05[0m           [35msyscall[0m[0m[0m
+[36m│[0m           [32m0x004001ef[0m      [33m48[37m8b[37m04[33m24[0m       [37mmov[36m rax[0m,[36m qword[36m [0m[[36mrsp[0m][36m[0m[0m[0m
+[36m│[0m           [32m0x004001f3[0m      [33m48[37m83[37mc4[37m08[0m       [33madd[36m rsp[0m,[36m[36m [33m8[0m[0m[0m
+[36m└[0m           [32m0x004001f7[0m      [37mc3[0m             [31mret[0m[0m[0m
+
+### Étape 5 : Analyse dynamique (strace / ltrace)
+- `strace ./bin/Sans_debugger_fixe` : uniquement `time()` puis exit(0). Aucune lecture entrée standard → le binaire attend des arguments, pas d'I/O interactive.
+- `ltrace` échoue (binaire statique sans dynsym), conforme à l'attente.
+
+### Étape 6 : Compréhension des fonctions clés (r2)
+- `fcn.0040009a` : strlen maison (compte jusqu'au 0x00).
+- `fcn.004000af` : `rdtsc` 64 bits (rdx<<32 | rax).
+- `fcn.004000b9` : transformée modulaire sur `n`, renvoie 1 si `(n^2 mod 0x101)` est préservé après une série d'affines, sinon 0. Pour les valeurs testées (0,1,27,0x1e2) → 0.
+- `fcn.004000f0` : anti-debug `ptrace(PTRACE_TRACEME, …)` après un appel inutile à `fcn.004000b9(0x1e2)`. Retour -1 si tracé.
+- `fcn.0040010b` : combine cinq restes `const*n mod {10,10,10,10,9}` en un entier décimal `E = r0 + 10*r1 + 100*r2 + 1000*r3 + 10000*r4`.
+- `fcn.00400192` : test sur l'argument 1 (len = strlen(arg)). Calcule `target = len^3 + 4`. Boucle i=0..15 : accumule `sum += (s[i]-0x10)*i`. Retourne 0 si `sum == target`, sinon 1. Exige typiquement `len==16` pour éviter lecture hors borne.
+- `fcn.004001e1` : wrapper `time()` (syscall 201) → renvoie `time(NULL)`.
+
+### Étape 7 : Reconstitution du `main` (fcn.004001f8)
+1. Initialise plusieurs buffers et récupère `t = time()` + `rdx = rdtsc % 100000`.
+2. Vérifie `argc >= 2` et `6 < len(arg1) <= 16` et `fcn.00400192(arg1) == 0`. Sinon sortie silencieuse.
+3. Si OK → `fd = 2` (var_58=2) pour l’écriture finale.
+4. Construit une chaîne `buf[15]` en trois blocs de 5 chiffres :
+   - Bloc A (indices 0..4) = les 5 derniers digits décimaux de `time()` (loop div/10).
+   - Anti-debug : si `ptrace` retourne -1 (sous debugger) alors pré-remplit une table de 256 valeurs via `fcn.004000b9(i)` et fixe `val = fcn.0040010b(rdx_mod)`; sinon `val = fcn.0040010b(1)`.
+   - Bloc B (indices 5..9) = 5 digits décimaux de `val` (div/10 successifs, avec padding).
+   - Copie Bloc A → tmp1, Bloc B → tmp2, reconstruit deux entiers `x=int(tmp1)` et `y=int(tmp2)` puis calcule `z = x XOR y`.
+   - Bloc C (indices 10..14) = digits décimaux de `z` (div/10 successifs).
+5. Si `argc > 2` et `argv[2]` == `buf` (15 chars) alors `fd >>= 1` (passe de 2 à 1 pour écrire sur stdout). Sinon reste à 2 (stderr).
+6. Écrit "OK\n" via `write(fd, "OK\n", 3)` puis exit.
+
+### Étape 8 : Construction d'un argument valide pour l’étape 2
+- Equation `sum((s[i]-0x10)*i) = len^3 + 4` avec `len=16` → cible 4100.
+- Choix base `s[i]='0'` (xi=32) donne somme 3840, manque 260. Ajouter +20 sur l’indice 13 (car 13*20=260) ⇒ caractère `0x44 ('D')`.
+- Chaîne finale argument1 : `0000000000000D00` (16 chars). Vérifié via script Python (somme = 4100).
+
+### Étape 9 : Génération de l’argument 2 (facultatif, pour basculer la sortie sur stdout)
+- Sans debugger, `val = fcn.0040010b(1) = 28862` (constante). Bloc B = "28862".
+- Bloc A dépend de `time()` au lancement : `time() % 100000` en digits.
+- Bloc C = `(int(BlocA) XOR int(BlocB))` rendu en 5 digits.
+- Script helper (Python) pour construire `argv2` en temps réel :
+```bash
+python - <<'PY'
+import time
+
+def fcn_b9(n):
+    ecx=0x101
+    edx=(n*n+0x1234)%ecx
+    esi=(edx*0x7ab7+0xa9e)%ecx
+    return int(esi==((n*n)%ecx))
+
+def fcn_10b(n):
+    e1=(0x76229a6*n)%10
+    e2=(0x72a3f9a*n)%10
+    e3=(0x6d028fe*n)%10
+    e4=(0x6b6396c*n)%10
+    e5=(0x627a118*n)%9
+    return e1 + e2*10 + e3*100 + e4*1000 + e5*10000
+
+def build(debug=False, t=None, rd=None):
+    if t is None:
+        t=int(time.time())
+    if rd is None:
+        rd=0
+    buf=['?']*15
+    var=t
+    r8=fcn_b9(0x1b)
+    for _ in range(5):
+        var,rem=divmod(var,10)
+        buf[4-r8]=str(rem); r8+=1
+    val=fcn_10b(rd if debug else 1)
+    r8=fcn_b9(0x1b)
+    for _ in range(5):
+        val,rem=divmod(val,10)
+        buf[9-r8]=str(rem); r8+=1
+    v1=int(''.join(buf[:5])); v2=int(''.join(buf[5:10])); z=v1^v2
+    r8=fcn_b9(0x1b)
+    for _ in range(5):
+        z,rem=divmod(z,10)
+        buf[14-r8]=str(rem); r8+=1
+    print(''.join(buf))
+
+build()
+PY
+```
+- Fournir ce résultat comme `argv2` juste après l’avoir calculé (éviter changement de seconde).
+
+### Étape 10 : Validation
+- Commande minimale qui affiche "OK" (fd=stderr si pas d’argv2) :
+  ```
+  ./bin/Sans_debugger_fixe 0000000000000D00
+  ```
+- Variante avec `argv2` calculé dynamiquement pour forcer `fd=stdout` :
+  ```
+  code=$(python helper_ci-dessus)
+  ./bin/Sans_debugger_fixe 0000000000000D00 "$code"
+  ```
+- Tests effectués : exécution simple → affiche OK, aucune interaction supplémentaire.
+
+### Réflexions / pistes inutilisées
+- GDB non requis : logique reconstruite statiquement + scripts.
+- Anti-debug `ptrace` inversé : si debugger actif, le calcul change (`val = fcn_0040010b(rdtsc%100000)`), donc éviter debugger pour solution stable.

