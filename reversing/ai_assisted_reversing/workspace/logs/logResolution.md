diff --git a/analysis/crackme1_notes.md b/analysis/crackme1_notes.md
new file mode 100644
index 0000000000000000000000000000000000000000..3252f2de4e8556447a17e976f7fadcaa8160e31c
--- /dev/null
+++ b/analysis/crackme1_notes.md
@@ -0,0 +1,507 @@
+# Crackme1 (Sn0wF4ult) – Notes d'analyse

- +## 1. Identification initiale
  +- Binaire ciblé : /bin/Sn0wF4ult
  +- Contexte : instructions AGENTS.md appliquées (exploration exhaustive, documentation détaillée).
  +- Objectif : comprendre mécanisme de vérification et fournir méthode de résolution reproductible.
- +### 1.1 Liste des binaires (/bin)
- +Commande: ls bin
  +Sn0wF4ult
- +## 2. Fingerprint initial
- +Commande: file bin/Sn0wF4ult
  +\nNote: package "file" absent initialement -> installation via apt-get (voir /tmp/apt-update.log et /tmp/apt-install-file.log).
  +Commande: file bin/Sn0wF4ult
  +bin/Sn0wF4ult: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, no section header
  +Commande: checksec --file=bin/Sn0wF4ult
  +Note: installation de checksec (journal: /tmp/apt-install-checksec.log).
  +Commande: checksec --file=bin/Sn0wF4ult
  +RELRO STACK CANARY NX PIE RPATH RUNPATH Symbols FORTIFY Fortified Fortifiable FILE
  +[31mNo RELRO [m [31mNo canary found[m [32mNX enabled [m [31mNo PIE [m [32mNo RPATH [m [32mNo RUNPATH [m [32mNo Symbols [m [31mNo[m 0 0 bin/Sn0wF4ult
  +Commande: rabin2 -I bin/Sn0wF4ult
  +Note: installation de radare2 (journal: /tmp/apt-install-radare2.log).
  +Commande: rabin2 -I bin/Sn0wF4ult
  +arch x86
  +baddr 0x400000
  +binsz 3680
  +bintype elf
  +bits 64
  +canary false
  +class ELF64
  +crypto false
  +endian little
  +havecode true
  +laddr 0x0
  +lang c
  +linenum true
  +lsyms true
  +machine AMD x86-64 architecture
  +nx true
  +os linux
  +pic false
  +relocs true
  +rpath NONE
  +sanitize false
  +static true
  +stripped false
  +subsys linux
  +va true
- +## 3. Strings rapides
- +Commande: strings -n 4 bin/Sn0wF4ult | head -n 40
  +D$xH
+D$xH
  +u&Ic
  +KHD1
  +C@E1
  +]A\A]A^A\_
  +mmmm
  +)I]%
  +<lm%D
  +)IE%
  +}cmm%D
  +)IM%
  +!IE%
  +)Iq%
  +!IE%
  +)Ip%
  +!IM%
  +)Is%
  +!IM%
  +)Ir%
  +9IM%
  +)IE%
  +mmm%
  +)IRm
  +9IR%
  +)Ieb
  +)IR%
  +)iqb
  +mmmb
  +9Ie%
  +)IRl
  +-Ummmm%
  +-xmH
  +T$ H
  +D$(H
  +]O`N[/T
  +[ZZZ
  +133rer
  +e133r
  +2;33<
- +## 4. Désassemblage initial (objdump)
- +Commande: objdump -d bin/Sn0wF4ult | head -n 80
- +bin/Sn0wF4ult: file format elf64-x86-64
-
- +## 5. Exécution basique
- +Commande: ./bin/Sn0wF4ult
  +Commande: ./bin/Sn0wF4ult test
  +Code de retour: 0
- +## 6. Traçage dynamique rapide
- +Commande: strace -f ./bin/Sn0wF4ult test
  +Note: installation de strace (journal: /tmp/apt-install-strace.log).
  +Commande: strace -f ./bin/Sn0wF4ult test
- +## 7. ltrace
- +Commande: ltrace ./bin/Sn0wF4ult test
  +Note: installation de ltrace (journal: /tmp/apt-install-ltrace.log).
  +Commande: ltrace ./bin/Sn0wF4ult test
- +## 8. Analyse radare2
- +Commande: r2 -A bin/Sn0wF4ult <<<"afl"
  +Commande: r2 -qc "aa;afl" bin/Sn0wF4ult
  +0x004006c9 1 28 entry0
  +0x004000b0 14 168 segment.LOAD0
  +Commande: r2 -qc "aa;pdf @ entry0" bin/Sn0wF4ult
  +[36m┌[0m 28: [31mentry0[0m (int64_t arg_8h);
  +[36m│[0m [37m; [37marg [34mint64_t arg_8h [36m@ rsp+0x8[0m
  +[36m│[0m [32m0x004006c9[0m [33m48[37m8b[33m3c[33m24[0m [37mmov[36m rdi[0m,[36m qword[36m [0m[[36mrsp[0m][36m[0m[0m[0m
  +[36m│[0m [32m0x004006cd[0m [33m48[37m8d[33m74[33m24[37m08[0m [37mlea[36m rsi[0m,[36m[36m [0m[[34marg_8h[0m][36m[0m[0m[0m
  +[36m│[0m [32m0x004006d2[0m [37me8[37md9[37mf9[31mff[31mff[0m [1;92mcall segment.LOAD0[0m[0m
  +[36m│[0m [32m0x004006d7[0m [33m48[37m89[37mc7[0m [37mmov[36m rdi[0m,[36m[36m rax[0m[0m[0m
  +[36m│[0m [32m0x004006da[0m [33m48[37mc7[37mc0[33m3c[32m00[32m00[37m.[0m [37mmov[36m rax[0m,[36m[36m [33m0x3c[0m[0m[31m [31m; '<'[31m [31m; 60[0m
  +[36m│[0m [32m0x004006e1[0m [37m0f[37m05[0m [35msyscall[0m[0m[0m
  +[36m└[0m [32m0x004006e3[0m [37m0f[37m0b[0m [1;91mud2[0m[0m[0m
  +Commande: r2 -qc "aa;pdf @ segment.LOAD0" bin/Sn0wF4ult
-            [31m[31m; CALL XREF from entry0 @ [31m0x4006d2[31m[0m
  +[36m┌[0m 168: [31msegment.LOAD0[0m (int64_t arg1, int64_t arg2);
  +[36m│[0m [37m; [37marg [34mint64_t arg1 [36m@ rdi[0m
  +[36m│[0m [37m; [37marg [34mint64_t arg2 [36m@ rsi[0m
  +[36m│[0m [32m0x004000b0[0m [33m53[0m [35mpush[36m rbx[0m[0m[0m [37m; [00] -rwx segment size 3504 named LOAD0[0m
  +[36m│[0m [32m0x004000b1[0m [33m48[37m83[31mff[37m02[0m [33mcmp[36m rdi[0m,[36m[36m [33m2[0m[0m[31m [31m; 2[0m [34m; arg1[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m<[0m [32m0x004000b5[0m [33m74[37m18[0m [32mje 0x4000cf[0m[0m
  +[36m│[0m [36m│[0m [32m0x004000b7[0m [37mba[37m1e[32m00[32m00[32m00[0m [37mmov[36m edx[0m,[36m[36m [33m0x1e[0m[0m[31m [31m; 30[0m
  +[36m│[0m [36m│[0m [32m0x004000bc[0m [33m48[37m8d[33m35[37m09[37m0c[32m00[37m.[0m [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x00400ccc[0m][36m[0m[0m[31m [31m; "Usage: ./Sn0wF4ult <password>\n"[0m
  +[36m│[0m [36m│[0m [32m0x004000c3[0m [37mbf[37m02[32m00[32m00[32m00[0m [37mmov[36m edi[0m,[36m[36m [33m2[0m[0m[0m
  +[36m│[0m [36m│[0m [32m0x004000c8[0m [37me8[33m79[37m03[32m00[32m00[0m [1;92mcall 0x400446[0m[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004000cd[0m [37meb[33m68[0m [32mjmp 0x400137[0m[0m
  +[36m│[0m [36m│[0m[36m└[0m[36m─[0m[36m>[0m [32m0x004000cf[0m [33m48[37m8b[33m5e[37m08[0m [37mmov[36m rbx[0m,[36m qword [0m[[36mrsi [0m+[36m[36m [33m8[0m][36m[0m[0m[0m [34m; arg2[0m
  +[36m│[0m [36m│[0m [32m0x004000d3[0m [33m41[37mb8[37m01[32m00[32m00[32m00[0m [37mmov[36m r8d[0m,[36m[36m [33m1[0m[0m[0m
  +[36m│[0m [36m│[0m [32m0x004000d9[0m [33m48[37m8d[33m35[37m0b[37m0c[32m00[37m.[0m [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x00400ceb[0m][36m[0m[0m[31m [31m; "Input too long\n"[0m
  +[36m│[0m [36m│[0m [31m[31m; CODE XREF from segment.LOAD0 @ [31m0x40010a[31m[0m
  +[36m│[0m [36m│[0m[34m┌[0m[34m─[0m[34m>[0m [32m0x004000e0[0m [33m42[37m80[33m7c[37m03[31mff[32m00[0m [33mcmp byte [0m[[36mrbx [0m+[36m r8 [0m-[36m[36m [33m1[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004000e6[0m [33m74[33m24[0m [32mje 0x40010c[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[34m╎[0m [32m0x004000e8[0m [33m41[37m83[37mf8[37m10[0m [33mcmp[36m r8d[0m,[36m[36m [33m0x10[0m[0m[31m [31m; 16[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004000ec[0m [33m7e[37m19[0m [32mjle 0x400107[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m[34m╎[0m [32m0x004000ee[0m [37mbf[37m02[32m00[32m00[32m00[0m [37mmov[36m edi[0m,[36m[36m [33m2[0m[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m[34m╎[0m [32m0x004000f3[0m [37mba[37m0f[32m00[32m00[32m00[0m [37mmov[36m edx[0m,[36m[36m [33m0xf[0m[0m[31m [31m; 15[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m[34m╎[0m [32m0x004000f8[0m [37me8[33m49[37m03[32m00[32m00[0m [1;92mcall 0x400446[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m[34m╎[0m [32m0x004000fd[0m [37mbf[37m01[32m00[32m00[32m00[0m [37mmov[36m edi[0m,[36m[36m [33m1[0m[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m[34m╎[0m [32m0x00400102[0m [37me8[37md2[37m01[32m00[32m00[0m [1;92mcall 0x4002d9[0m[0m
  +[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x00400107[0m [33m49[31mff[37mc0[0m [33minc[36m r8[0m[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[34m└[0m[34m─[0m[34m<[0m [32m0x0040010a[0m [37meb[37md4[0m [32mjmp 0x4000e0[0m[0m
  +[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040010c[0m [37me8[37m8a[37m03[32m00[32m00[0m [1;92mcall 0x40049b[0m[0m
  +[36m│[0m [36m│[0m [32m0x00400111[0m [37m84[37mc0[0m [33mtest[36m al[0m,[36m[36m al[0m[0m[0m
  +[36m│[0m [36m│[0m[36m┌[0m[36m─[0m[36m<[0m [32m0x00400113[0m [33m74[37m16[0m [32mje 0x40012b[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m [32m0x00400115[0m [33m48[37m89[37mdf[0m [37mmov[36m rdi[0m,[36m[36m rbx[0m[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m [32m0x00400118[0m [37me8[37mc6[37m01[32m00[32m00[0m [1;92mcall 0x4002e3[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m [32m0x0040011d[0m [37m84[37mc0[0m [33mtest[36m al[0m,[36m[36m al[0m[0m[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040011f[0m [33m75[37m0a[0m [32mjne 0x40012b[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m [32m0x00400121[0m [37mbf[37m0d[32m00[32m00[32m00[0m [37mmov[36m edi[0m,[36m[36m [33m0xd[0m[0m[31m [31m; 13[0m
  +[36m│[0m [36m│[0m[36m│[0m[36m│[0m [32m0x00400126[0m [37me8[37mae[37m01[32m00[32m00[0m [1;92mcall 0x4002d9[0m[0m
  +[36m│[0m [36m└[0m[36m─[0m[36m└[0m[36m─[0m[36m>[0m [32m0x0040012b[0m [33m48[37m89[37mdf[0m [37mmov[36m rdi[0m,[36m[36m rbx[0m[0m[0m
  +[36m│[0m [36m│[0m [32m0x0040012e[0m [37me8[33m25[32m00[32m00[32m00[0m [1;92mcall 0x400158[0m[0m
  +[36m│[0m [36m│[0m [32m0x00400133[0m [37m85[37mc0[0m [33mtest[36m eax[0m,[36m[36m eax[0m[0m[0m
  +[36m│[0m [36m│[0m[36m┌[0m[36m─[0m[36m<[0m [32m0x00400135[0m [33m75[37m07[0m [32mjne 0x40013e[0m[0m
  +[36m│[0m [36m│[0m[36m│[0m [31m[31m; CODE XREF from segment.LOAD0 @ [31m0x4000cd[31m[0m
  +[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m>[0m [32m0x00400137[0m [37mb8[37m01[32m00[32m00[32m00[0m [37mmov[36m eax[0m,[36m[36m [33m1[0m[0m[0m
  +[36m│[0m [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040013c[0m [37meb[37m18[0m [32mjmp 0x400156[0m[0m
  +[36m│[0m [36m│[0m[36m└[0m[36m─[0m[36m>[0m [32m0x0040013e[0m [37mba[37m03[32m00[32m00[32m00[0m [37mmov[36m edx[0m,[36m[36m [33m3[0m[0m[0m
  +[36m│[0m [36m│[0m [32m0x00400143[0m [33m48[37m8d[33m35[37mb1[37m0b[32m00[37m.[0m [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x00400cfb[0m][36m[0m[0m[31m [31m; "OK\n"[0m
  +[36m│[0m [36m│[0m [32m0x0040014a[0m [37mbf[37m01[32m00[32m00[32m00[0m [37mmov[36m edi[0m,[36m[36m [33m1[0m[0m[0m
  +[36m│[0m [36m│[0m [32m0x0040014f[0m [37me8[37mf2[37m02[32m00[32m00[0m [1;92mcall 0x400446[0m[0m
  +[36m│[0m [36m│[0m [32m0x00400154[0m [33m31[37mc0[0m [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
  +[36m│[0m [36m│[0m [31m[31m; CODE XREF from segment.LOAD0 @ [31m0x40013c[31m[0m
  +[36m│[0m [36m└[0m[36m─[0m[36m─[0m[36m>[0m [32m0x00400156[0m [33m5b[0m [35mpop[36m rbx[0m[0m[0m
  +[36m└[0m [32m0x00400157[0m [37mc3[0m [31mret[0m[0m[0m
  +Commande: r2 -qc "aa;pdf @ 0x400446" bin/Sn0wF4ult
  +Commande: r2 -qc "aa;pd 64 @ 0x400446" bin/Sn0wF4ult
-            [31m[31m; CALL XREFS from segment.LOAD0 @ [31m0x4000c8[31m, 0x4000f8[31m, 0x40014f[31m[0m
-            [32m0x00400446[0m      [33m48[37mc7[37mc0[37m01[32m00[32m00[37m.[0m  [37mmov[36m rax[0m,[36m[36m [33m1[0m[0m[0m
-            [32m0x0040044d[0m      [37m0f[37m05[0m           [35msyscall[0m[0m[0m
-            [32m0x0040044f[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x00400450[0m      [33m4c[37m8d[37m1d[37m89[37m09[32m00[37m.[0m  [37mlea[36m r11[0m,[36m[36m [0m[[36m[33m0x00400de0[0m][36m[0m[0m[0m
-            [32m0x00400457[0m      [33m49[37m89[37mf8[0m         [37mmov[36m r8[0m,[36m[36m rdi[0m[0m[0m
-            [32m0x0040045a[0m      [33m41[37m89[37mf2[0m         [37mmov[36m r10d[0m,[36m[36m esi[0m[0m[0m
-            [32m0x0040045d[0m      [33m4c[37m89[37mdf[0m         [37mmov[36m rdi[0m,[36m[36m r11[0m[0m[0m
-            [32m0x00400460[0m      [37me8[37mf8[37mfe[31mff[31mff[0m     [1;92mcall 0x40035d[0m[0m
-            [32m0x00400465[0m      [33m4c[37m89[37mc7[0m         [37mmov[36m rdi[0m,[36m[36m r8[0m[0m[0m
-            [32m0x00400468[0m      [37me8[33m57[37m05[32m00[32m00[0m     [1;92mcall 0x4009c4[0m[0m
-            [32m0x0040046d[0m      [33m41[37m8b[33m48[33m38[0m       [37mmov[36m ecx[0m,[36m dword [0m[[36mr8 [0m+[36m[36m [33m0x38[0m][36m[0m[0m[0m
-            [32m0x00400471[0m      [33m31[37md2[0m           [33mxor[36m edx[0m,[36m[36m edx[0m[0m[0m
-            [32m0x00400473[0m      [33m41[37m89[37mc1[0m         [37mmov[36m r9d[0m,[36m[36m eax[0m[0m[0m
-            [32m0x00400476[0m      [33m49[37m8b[33m40[33m68[0m       [37mmov[36m rax[0m,[36m qword [0m[[36mr8 [0m+[36m[36m [33m0x68[0m][36m[0m[0m[0m
-            [32m0x0040047a[0m      [33m48[37m85[37mc0[0m         [33mtest[36m rax[0m,[36m[36m rax[0m[0m[0m
-        [36m┌[0m[36m─[0m[36m<[0m [32m0x0040047d[0m      [33m74[37m02[0m           [32mje 0x400481[0m[0m
-        [36m│[0m   [32m0x0040047f[0m      [37m8a[37m10[0m           [37mmov[36m dl[0m,[36m byte[36m [0m[[36mrax[0m][36m[0m[0m[0m
-        [36m└[0m[36m─[0m[36m>[0m [32m0x00400481[0m      [37m8d[33m34[37m0a[0m         [37mlea[36m esi[0m,[36m [0m[[36mrdx [0m+[36m[36m rcx[0m][36m[0m[0m[0m
-            [32m0x00400484[0m      [33m4c[37m89[37mdf[0m         [37mmov[36m rdi[0m,[36m[36m r11[0m[0m[0m
-            [32m0x00400487[0m      [37m89[37mf2[0m           [37mmov[36m edx[0m,[36m[36m esi[0m[0m[0m
-            [32m0x00400489[0m      [33m44[33m31[37md2[0m         [33mxor[36m edx[0m,[36m[36m r10d[0m[0m[0m
-            [32m0x0040048c[0m      [37m0f[37mb6[37mf2[0m         [37mmovzx[36m esi[0m,[36m[36m dl[0m[0m[0m
-            [32m0x0040048f[0m      [37me8[37mcf[37mfe[31mff[31mff[0m     [1;92mcall 0x400363[0m[0m
-            [32m0x00400494[0m      [33m44[37m89[37mc8[0m         [37mmov[36m eax[0m,[36m[36m r9d[0m[0m[0m
-            [32m0x00400497[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x00400498[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
-            [32m0x0040049a[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [31m[31m; CALL XREF from segment.LOAD0 @ [31m0x40010c[31m[0m
-            [32m0x0040049b[0m      [33m53[0m             [35mpush[36m rbx[0m[0m[0m
-            [32m0x0040049c[0m      [37mb9[33m5a[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0x5a[0m[0m[31m               [31m; 'Z'[31m [31m; 90[0m
-            [32m0x004004a1[0m      [37mba[37m11[32m00[32m00[32m00[0m     [37mmov[36m edx[0m,[36m[36m [33m0x11[0m[0m[31m               [31m; 17[0m
-            [32m0x004004a6[0m      [33m48[37m8d[33m35[33m63[37m08[32m00[37m.[0m  [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x00400d10[0m][36m[0m[0m[31m       [31m; "u*(59u)?6<u).;./)"[0m
-            [32m0x004004ad[0m      [33m48[37m81[37mec[37ma0[32m00[32m00[37m.[0m  [33msub[36m rsp[0m,[36m[36m [33m0xa0[0m[0m[0m
-            [32m0x004004b4[0m      [33m4c[37m8d[33m4c[33m24[37m0e[0m     [37mlea[36m r9[0m,[36m [0m[[36mrsp [0m+[36m[36m [33m0xe[0m][36m[0m[0m[0m
-            [32m0x004004b9[0m      [33m4c[37m89[37mcf[0m         [37mmov[36m rdi[0m,[36m[36m r9[0m[0m[0m
-            [32m0x004004bc[0m      [37me8[37m0b[37mfe[31mff[31mff[0m     [1;92mcall 0x4002cc[0m[0m
-            [32m0x004004c1[0m      [33m48[37m8d[33m7c[33m24[37m03[0m     [37mlea[36m rdi[0m,[36m [0m[[36mrsp [0m+[36m[36m [33m3[0m][36m[0m[0m[0m
-            [32m0x004004c6[0m      [37mb9[37ma5[32m00[32m00[32m00[0m     [37mmov[36m ecx[0m,[36m[36m [33m0xa5[0m[0m[31m               [31m; 165[0m
-            [32m0x004004cb[0m      [37mba[37m0a[32m00[32m00[32m00[0m     [37mmov[36m edx[0m,[36m[36m [33m0xa[0m[0m[0m
-            [32m0x004004d0[0m      [33m48[37m8d[33m35[33m29[37m08[32m00[37m.[0m  [37mlea[36m rsi[0m,[36m[36m [0m[[36m[33m0x00400d00[0m][36m[0m[0m[0m
-            [32m0x004004d7[0m      [37me8[37mf0[37mfd[31mff[31mff[0m     [1;92mcall 0x4002cc[0m[0m
-            [32m0x004004dc[0m      [33m4c[37m89[37mcf[0m         [37mmov[36m rdi[0m,[36m[36m r9[0m[0m[0m
-            [32m0x004004df[0m      [37me8[33m3d[31mff[31mff[31mff[0m     [1;92mcall 0x400421[0m[0m
-            [32m0x004004e4[0m      [33m48[37m89[37mc7[0m         [37mmov[36m rdi[0m,[36m[36m rax[0m[0m[0m
-            [32m0x004004e7[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
-            [32m0x004004e9[0m      [33m48[37m85[31mff[0m         [33mtest[36m rdi[0m,[36m[36m rdi[0m[0m[0m
-        [36m┌[0m[36m─[0m[36m<[0m [32m0x004004ec[0m      [37m0f[37m88[37m9d[32m00[32m00[32m00[0m   [32mjs 0x40058f[0m[0m
-        [36m│[0m   [32m0x004004f2[0m      [33m45[33m31[37mc9[0m         [33mxor[36m r9d[0m,[36m[36m r9d[0m[0m[0m
-        [36m│[0m   [32m0x004004f5[0m      [33m45[33m31[37md2[0m         [33mxor[36m r10d[0m,[36m[36m r10d[0m[0m[0m
-        [36m│[0m   [32m0x004004f8[0m      [33m45[33m31[37mc0[0m         [33mxor[36m r8d[0m,[36m[36m r8d[0m[0m[0m
-        [36m│[0m   [32m0x004004fb[0m      [33m48[37m8d[33m74[33m24[33m20[0m     [37mlea[36m rsi[0m,[36m [0m[[36mrsp [0m+[36m[36m [33m0x20[0m][36m[0m[0m[0m
-        [36m│[0m   [32m0x00400500[0m      [37me8[33m2b[31mff[31mff[31mff[0m     [1;92mcall 0x400430[0m[0m
-        [36m│[0m   [32m0x00400505[0m      [33m48[37m85[37mc0[0m         [33mtest[36m rax[0m,[36m[36m rax[0m[0m[0m
-       [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400508[0m      [33m7e[33m77[0m           [32mjle 0x400581[0m[0m
-       [36m│[0m[36m│[0m   [32m0x0040050a[0m      [33m44[37m8a[33m5c[33m24[37m03[0m     [37mmov[36m r11b[0m,[36m byte [0m[[36mrsp [0m+[36m[36m [33m3[0m][36m[0m[0m[0m
-       [36m│[0m[36m│[0m   [32m0x0040050f[0m      [33m31[37mc9[0m           [33mxor[36m ecx[0m,[36m[36m ecx[0m[0m[0m
-       [36m│[0m[36m│[0m   [32m0x00400511[0m      [37m8a[37m14[37m0e[0m         [37mmov[36m dl[0m,[36m byte [0m[[36mrsi [0m+[36m[36m rcx[0m][36m[0m[0m[0m
-       [36m│[0m[36m│[0m   [32m0x00400514[0m      [33m45[37m84[37mc9[0m         [33mtest[36m r9b[0m,[36m[36m r9b[0m[0m[0m
-      [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400517[0m      [33m75[33m26[0m           [32mjne 0x40053f[0m[0m
-      [36m│[0m[36m│[0m[36m│[0m   [32m0x00400519[0m      [33m49[33m63[37md8[0m         [37mmovsxd[36m rbx[0m,[36m[36m r8d[0m[0m[0m
-      [36m│[0m[36m│[0m[36m│[0m   [32m0x0040051c[0m      [33m38[33m54[37m1c[37m03[0m       [33mcmp byte [0m[[36mrsp [0m+[36m rbx [0m+[36m[36m [33m3[0m][36m[0m,[36m[36m dl[0m[0m[0m
-     [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400520[0m      [33m75[37m11[0m           [32mjne 0x400533[0m[0m
-     [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400522[0m      [33m41[31mff[37mc0[0m         [33minc[36m r8d[0m[0m[0m
-     [36m│[0m[36m│[0m[36m│[0m[36m│[0m   [32m0x00400525[0m      [33m41[37m83[37mf8[37m0a[0m       [33mcmp[36m r8d[0m,[36m[36m [33m0xa[0m[0m[31m                [31m; 10[0m
- [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x00400529[0m [33m75[33m3c[0m [32mjne 0x400567[0m[0m
  +Commande: r2 -qc "aa;pd 64 @ 0x4002d9" bin/Sn0wF4ult
-            [31m[31m; CALL XREFS from segment.LOAD0 @ [31m0x400102[31m, 0x400126[31m[0m
-            [32m0x004002d9[0m      [33m48[37mc7[37mc0[33m3c[32m00[32m00[37m.[0m  [37mmov[36m rax[0m,[36m[36m [33m0x3c[0m[0m[31m               [31m; '<'[31m [31m; 60[0m
-            [32m0x004002e0[0m      [37m0f[37m05[0m           [35msyscall[0m[0m[0m
-            [32m0x004002e2[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [31m[31m; CALL XREF from segment.LOAD0 @ [31m0x400118[31m[0m
-            [32m0x004002e3[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
-            [32m0x004002e5[0m      [33m48[37m8d[37m0d[33m44[37m0a[32m00[37m.[0m  [37mlea[36m rcx[0m,[36m[36m [0m[[36m[33m0x00400d30[0m][36m[0m[0m[0m
-        [34m┌[0m[34m─[0m[34m>[0m [32m0x004002ec[0m      [37m8a[37m14[37m01[0m         [37mmov[36m dl[0m,[36m byte [0m[[36mrcx [0m+[36m[36m rax[0m][36m[0m[0m[0m
-        [34m╎[0m   [32m0x004002ef[0m      [37m83[37mf2[37maa[0m         [33mxor[36m edx[0m,[36m[36m [33m0xffffffaa[0m[0m[31m         [31m; 4294967210[0m
-        [34m╎[0m   [32m0x004002f2[0m      [33m38[37m14[37m07[0m         [33mcmp byte [0m[[36mrdi [0m+[36m[36m rax[0m][36m[0m,[36m[36m dl[0m[0m[0m
-       [36m┌[0m[36m─[0m[36m─[0m[36m<[0m [32m0x004002f5[0m      [33m75[37m0c[0m           [32mjne 0x400303[0m[0m
-       [36m│[0m[34m╎[0m   [32m0x004002f7[0m      [33m48[31mff[37mc0[0m         [33minc[36m rax[0m[0m[0m
-       [36m│[0m[34m╎[0m   [32m0x004002fa[0m      [33m48[37m83[37mf8[37m10[0m       [33mcmp[36m rax[0m,[36m[36m [33m0x10[0m[0m[31m               [31m; 16[0m
-       [36m│[0m[34m└[0m[34m─[0m[34m<[0m [32m0x004002fe[0m      [33m75[37mec[0m           [32mjne 0x4002ec[0m[0m
-       [36m│[0m    [32m0x00400300[0m      [37mb0[37m01[0m           [37mmov[36m al[0m,[36m[36m [33m1[0m[0m[0m
-       [36m│[0m    [32m0x00400302[0m      [37mc3[0m             [31mret[0m[0m[0m
-       [36m└[0m[36m─[0m[36m─[0m[36m>[0m [32m0x00400303[0m      [33m31[37mc0[0m           [33mxor[36m eax[0m,[36m[36m eax[0m[0m[0m
-            [32m0x00400305[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x00400306[0m      [33m48[37mc7[37mc0[37me4[32m00[32m00[37m.[0m  [37mmov[36m rax[0m,[36m[36m [33m0xe4[0m[0m[31m               [31m; 228[0m
-            [32m0x0040030d[0m      [33m48[33m31[31mff[0m         [33mxor[36m rdi[0m,[36m[36m rdi[0m[0m[0m
-            [32m0x00400310[0m      [33m48[37m8d[33m74[33m24[37mf0[0m     [37mlea[36m rsi[0m,[36m [0m[[36mrsp [0m-[36m[36m [33m0x10[0m][36m[0m[0m[0m
-            [32m0x00400315[0m      [37m0f[37m05[0m           [35msyscall[0m[0m[0m
-            [32m0x00400317[0m      [33m48[37m8b[33m44[33m24[37mf0[0m     [37mmov[36m rax[0m,[36m qword [0m[[36mrsp [0m-[36m[36m [33m0x10[0m][36m[0m[0m[0m
-            [32m0x0040031c[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x0040031d[0m      [33m48[33m6b[37mc7[37m1f[0m       [33mimul[36m rax[0m,[36m[36m rdi[0m,[36m[36m [33m0x1f[0m[0m[0m
-            [32m0x00400321[0m      [33m48[37mc1[37me7[37m02[0m       [33mshl[36m rdi[0m,[36m[36m [33m2[0m[0m[0m
-            [32m0x00400325[0m      [33m48[33m6b[37md6[37m11[0m       [33mimul[36m rdx[0m,[36m[36m rsi[0m,[36m[36m [33m0x11[0m[0m[0m
-            [32m0x00400329[0m      [33m48[37mc1[37me6[37m03[0m       [33mshl[36m rsi[0m,[36m[36m [33m3[0m[0m[0m
-            [32m0x0040032d[0m      [33m48[33m31[37mfe[0m         [33mxor[36m rsi[0m,[36m[36m rdi[0m[0m[0m
-            [32m0x00400330[0m      [33m48[33m31[37md0[0m         [33mxor[36m rax[0m,[36m[36m rdx[0m[0m[0m
-            [32m0x00400333[0m      [37m01[37mf0[0m           [33madd[36m eax[0m,[36m[36m esi[0m[0m[0m
-            [32m0x00400335[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x00400336[0m      [37m89[37mf8[0m           [37mmov[36m eax[0m,[36m[36m edi[0m[0m[0m
-            [32m0x00400338[0m      [37m89[37mf1[0m           [37mmov[36m ecx[0m,[36m[36m esi[0m[0m[0m
-            [32m0x0040033a[0m      [37md2[37mc0[0m           [33mrol[36m al[0m,[36m[36m cl[0m[0m[0m
-            [32m0x0040033c[0m      [37mc3[0m             [31mret[0m[0m[0m
-            [32m0x0040033d[0m      [37m89[37mf8[0m           [37mmov[36m eax[0m,[36m[36m edi[0m[0m[0m
-            [32m0x0040033f[0m      [37m89[37mf1[0m           [37mmov[36m ecx[0m,[36m[36m esi[0m[0m[0m
-            [32m0x00400341[0m      [37md2[37mc8[0m           [33mror[36m al[0m,[36m[36m cl[0m[0m[0m
-            [32m0x00400343[0m      [37mc3[0m             [31mret[0m[0m[0m
-       [34m┌[0m[34m┌[0m[34m─[0m[34m>[0m [32m0x00400344[0m      [33m48[37m8b[37m07[0m         [37mmov[36m rax[0m,[36m qword[36m [0m[[36mrdi[0m][36m[0m[0m[0m
-       [34m╎[0m[34m╎[0m   [32m0x00400347[0m      [33m48[37m8b[33m4f[37m08[0m       [37mmov[36m rcx[0m,[36m qword [0m[[36mrdi [0m+[36m[36m [33m8[0m][36m[0m[0m[0m
-      [34m┌[0m[34m─[0m[34m─[0m[34m─[0m[34m>[0m [32m0x0040034b[0m      [33m48[33m39[37mc8[0m         [33mcmp[36m rax[0m,[36m[36m rcx[0m[0m[0m
-     [36m┌[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m<[0m [32m0x0040034e[0m      [33m73[37m0c[0m           [32mjae 0x40035c[0m[0m
-     [36m│[0m[34m╎[0m[34m╎[0m[34m╎[0m   [32m0x00400350[0m      [37m8a[37m10[0m           [37mmov[36m dl[0m,[36m byte[36m [0m[[36mrax[0m][36m[0m[0m[0m
-     [36m│[0m[34m╎[0m[34m╎[0m[34m╎[0m   [32m0x00400352[0m      [33m48[31mff[37mc0[0m         [33minc[36m rax[0m[0m[0m
-     [36m│[0m[34m╎[0m[34m╎[0m[34m╎[0m   [32m0x00400355[0m      [33m31[37mf2[0m           [33mxor[36m edx[0m,[36m[36m esi[0m[0m[0m
-     [36m│[0m[34m╎[0m[34m╎[0m[34m╎[0m   [32m0x00400357[0m      [37m88[33m50[31mff[0m         [37mmov byte [0m[[36mrax [0m-[36m[36m [33m1[0m][36m[0m,[36m[36m dl[0m[0m[0m
-     [36m│[0m[34m└[0m[34m─[0m[34m─[0m[34m─[0m[34m<[0m [32m0x0040035a[0m      [37meb[37mef[0m           [32mjmp 0x40034b[0m[0m
-     [36m└[0m[36m─[0m[36m─[0m[36m─[0m[36m─[0m[36m>[0m [32m0x0040035c[0m      [37mc3[0m             [31mret[0m[0m[0m
-       [34m╎[0m[34m╎[0m   [32m0x0040035d[0m      [37m0f[37mb6[33m77[37m10[0m       [37mmovzx[36m esi[0m,[36m byte [0m[[36mrdi [0m+[36m[36m [33m0x10[0m][36m[0m[0m[0m
-       [34m└[0m[34m─[0m[34m─[0m[34m<[0m [32m0x00400361[0m      [37meb[37me1[0m           [32mjmp 0x400344[0m[0m
-        [34m╎[0m   [32m0x00400363[0m      [37m8a[33m47[37m10[0m         [37mmov[36m al[0m,[36m byte [0m[[36mrdi [0m+[36m[36m [33m0x10[0m][36m[0m[0m[0m
-        [34m╎[0m   [32m0x00400366[0m      [37m83[37mce[37m01[0m         [33mor[36m esi[0m,[36m[36m [33m1[0m[0m[0m
-        [34m╎[0m   [32m0x00400369[0m      [37m8d[33m50[33m5d[0m         [37mlea[36m edx[0m,[36m [0m[[36mrax [0m+[36m[36m [33m0x5d[0m][36m[0m[0m[0m
-        [34m╎[0m   [32m0x0040036c[0m      [33m31[37md6[0m           [33mxor[36m esi[0m,[36m[36m edx[0m[0m[0m
-        [34m╎[0m   [32m0x0040036e[0m      [37m01[37mc6[0m           [33madd[36m esi[0m,[36m[36m eax[0m[0m[0m
-        [34m╎[0m   [32m0x00400370[0m      [33m40[37m88[33m77[37m10[0m       [37mmov byte [0m[[36mrdi [0m+[36m[36m [33m0x10[0m][36m[0m,[36m[36m sil[0m[0m[0m
-        [34m╎[0m   [32m0x00400374[0m      [33m40[37m0f[37mb6[37mf6[0m       [37mmovzx[36m esi[0m,[36m[36m sil[0m[0m[0m
-        [34m└[0m[34m─[0m[34m<[0m [32m0x00400378[0m      [37meb[37mca[0m           [32mjmp 0x400344[0m[0m
-            [32m0x0040037a[0m      [37me8[37mde[31mff[31mff[31mff[0m     [1;92mcall 0x40035d[0m[0m
-            [32m0x0040037f[0m      [33m48[37m8b[37m07[0m         [37mmov[36m rax[0m,[36m qword[36m [0m[[36mrdi[0m][36m[0m[0m[0m
-            [32m0x00400382[0m      [33m48[37m8b[33m57[37m08[0m       [37mmov[36m rdx[0m,[36m qword [0m[[36mrdi [0m+[36m[36m [33m8[0m][36m[0m[0m[0m
-            [32m0x00400386[0m      [33m48[33m39[37md0[0m         [33mcmp[36m rax[0m,[36m[36m rdx[0m[0m[0m
-        [36m┌[0m[36m─[0m[36m<[0m [32m0x00400389[0m      [33m73[37m08[0m           [32mjae 0x400393[0m[0m
-        [36m│[0m   [32m0x0040038b[0m      [37mc6[32m00[32m00[0m         [37mmov byte[36m [0m[[36mrax[0m][36m[0m,[36m[36m [36m0[0m[0m[0m
  +[32m- offset - 0 1 2 3 4 5 6 7 8 9 A B C D E F 0123456789ABCDEF
  +[0m[32m0x00400d30[0m [37me6[0m[37m9e[0m [37me9[0m[37m9a[0m [37mc6[0m[37mc6[0m [37m9b[0m[37mc4[0m [37m99[0m[37mce[0m [37mdf[0m[37mc9[0m [37md8[0m[37m9e[0m [37mc9[0m[37mc1[0m [37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m[37m.[0m
- +## 9. Débogage (gdb)
- +Note: installation de gdb (journal: /tmp/apt-install-gdb.log).
- +### Résumé provisoire
  +- Anti-debug : fonction à 0x40049b déchiffre "/proc/self/status" (xor 0x5a) et "TracerPid:" (xor 0xa5), lit le fichier et retourne 1 si TracerPid != 0. Fonction utilisée deux fois (main et fcn.00400158).
  +- Vérification simple : fcn.004002e3 compare l'entrée avec la constante obtenue après XOR 0xaa des 16 octets stockés à 0x400d30 → mot de passe en clair supposé `L4C0ll1n3ducr4ck`.
  +- Flux principal (segment.LOAD0) :
- - Vérifie argc==2, longueur <=16, sinon message d'erreur/exit.
- - Appelle anti-debug (0x40049b), puis éventuellement fcn.004002e3 ; si échec évident -> exit(13).
- - Appelle fcn.00400158, succès si EAX≠0 → écrit "OK\n".
    +- Observation gdb : avec `L4C0ll1n3ducr4ck`, le programme affiche "Good Password" puis quitte (exit code 13) avant d'atteindre l'impression "OK" (probable effet de l'anti-debug ou d'un check interne).
- +### Points ouverts / prochaines étapes
  +- Comprendre fcn.00400158 et les helpers (0x40035d/0x400363/0x400598...) : semblent manipuler des structures à 0x400d40/0x400dc0 et peut-être un moteur pseudo-aléatoire (0x4006e5 non décodé correctement). Déterminer la condition pour qu'EAX soit non nul.
  +- Vérifier le comportement hors débogueur avec le mot de passe candidat : exécution silencieuse, code retour 1 → check final non satisfait.
  +- Explorer 0x400598 et 0x4006e5 avec un désassemblage plus fiable (définir sections manuellement dans r2 ou utiliser objdump avec adresse de base) pour reconstruire l'algorithme final.
  +- Tester d'autres hypothèses : input peut nécessiter transformation additionnelle ou dépendre d'un état interne (TracerPid non nul ?), ou d'une graine issue de /proc/self/status.
- +### 10. Reprise des points ouverts (session courante)
- +#### 10.1 Exécution hors débogueur avec mot de passe candidat
  +- Commande: `./bin/Sn0wF4ult L4C0ll1n3ducr4ck; echo $?`
  +- Résultat: aucune sortie, code de retour `1` → confirme que l'étape finale échoue encore malgré le mot de passe trouvé précédemment (cf. chunk 03012f).
- +#### 10.2 Désassemblage ciblé de fcn.00400158 et des helpers (objdump mode binaire)
  +- Nouvelle tentative radare2 bloquée (`command not found` avant réinstallation), installation relancée (`apt-get install -y radare2`, journal /tmp/apt-install-radare2.log).
  +- Désassemblage direct avec `objdump -bbinary -mi386:x86-64 --adjust-vma=0x400000 -D bin/Sn0wF4ult` pour éviter les soucis d'absence d'en-têtes de section.
  +- Extraction autour de 0x400158 (flux principal) et des helpers à 0x40035d/0x400363 via `radare2 -q -c "s 0x400158; pd 128" bin/Sn0wF4ult` + `radare2 -q -c "s 0x400340; pd 96" bin/Sn0wF4ult` (chunks cbd1f4, d6a24c).
- - fcn.00400158 aligne une structure locale (0xb8 octets), nettoie 0x1e dwords à 0, sauve argv[1] en [rsp+0x78]/[rsp+0xa0], appelle 0x4003dd puis l'anti-debug (0x40049b). Si anti-debug + fcn.004002e3 ok, remplit deux buffers temporaires (boucle 0x4001e3/0x400280) puis déclenche 0x400398/exit(13). Dans tous les cas, finit par appeler 0x40035d (sur 0x400d40), 0x400598 (structure au stack, paramètre seed = byte [rsp+0x9f] issu de 0x4003dd) et quelques combinaisons arithmétiques/byte mix avant ret de eax (=retour de 0x400598).
- - fcn.004002e3 reste la comparaison XOR 0xaa sur 16 octets @0x400d30 (mot de passe `L4C0ll1n3ducr4ck`).
- - 0x40035d/0x400344: buffer abstrait contenant start/end ptrs en [rdi]/[rdi+8] et un octet "clé" en [rdi+0x10]; boucle XOR chaque octet du segment avec `esi` (chargé depuis [rdi+0x10]) jusqu'à atteindre la fin. 0x400363 met à jour la clé: `esi = ((byte[key]|1) xor (al+0x5d)) + al` puis réécrit en [rdi+0x10] avant de relancer la boucle XOR. 0x40037a/0x400398 utilisent 0x40035d pour nettoyer plusieurs structures en mémoire statique (0x400dc0, 0x400de0, 0x400da0, 0x400d60, 0x400d80, 0x400d40) et mettre l'octet clé à 0 via 0x40037a.
- +#### 10.3 Analyse de 0x400598 (initialisation des structures de vérification)
  +- Désassemblage plus fiable via objdump (chunks 2e0ef7, 2134b1) :
- - Prologue: sauvegarde r15=esi (seed externe), zéro r9d et enchaîne 5 appels à 0x40035d pour remettre à zéro les buffers 0x400da0/0x400d60/0x400d80/0x400dc0 puis calcule longueur `len = [0x400da8]-[0x400da0]` stockée en [rdi] + [rdi+8].
- - Si len <=0 → sortie directe (return 0). Sinon r9d = len/2 (sar 1) ; si r9d==0 → sortie.
- - Initialise nombreux champs dans la structure pointée par RBX (dérivée du buffer stack passé par fcn.00400158) : [rbx+0x14]=0, [rbx+0x48]/[rbx+0x58]/[rbx+0x60]=0, [rbx+0x10]=0, [rbx+0x50]=r15b, [rbx+0xc]=edx (résidu division), [rbx+0x38]/[rbx+0x70]=0, [rbx]=ptr 0x400da0, [rbx+8]=len, [rbx+0x68] laissé configurable (fallback 0x400cff si nul).
- - Calcule `r8d` = ( [rbx+0x58] + [rbx+0x38] + [rbx+0x40] ) xor r15d ; puis applique trois XOR successifs via 0x400363 sur (0x400d60, 0x400d80, 0x400dc0, 0x400d80?) avec des variantes de r8b et r8d^0x7a^(~0x5f)… (cf. suite d'appels 0x400363 à 0x40067e, 0x400690, 0x4006aa, 0x4006b6). Le résultat final renvoyé par 0x400598 est r9d, mis à jour après l'appel à 0x4009e4.
- - Appel critique: `call 0x4009e4` avec (rdi=rbx, rsi=rbx, rdx= pointer maybe 0x400cff si [rbx+0x68]==0, ecx = r9d, sil = r15b). La valeur de retour (eax) remonte dans r9d et devient le `ret` final de fcn.00400158. Condition de succès semble donc être `0x4009e4(...) != 0`.
- +#### 10.4 0x4006e5 et 0x4009e4 : disassemblage corrompu / hypothèses
  +- Tentatives gdb (`x/40i 0x4006e5`) échouent car gdb ne mappe pas l'ELF dépourvu d'en-têtes (erreur "Cannot access memory 0x4006e5"), même après `starti`. Passage par objdump en mode binaire fournit des instructions incohérentes/invalides (chunks 2134b1, 0c1431) : séquences de `insd/in/out` et immédiats fantaisistes, puis plus loin une portion structurée (`mov rax,[rsp+0x28]; mov rsi,rdx; mov rdi,rax; call 0x40031d; add rsp,0x48; ret`).
  +- Interprétation actuelle : 0x4006e5 correspond probablement à un bloc de données auto-déchiffré ou à un moteur PRNG obfusqué par self-modification/overlapping instructions. Le flux réel peut sauter au milieu (vers 0x400904) après transformation en mémoire. 0x4009e4 est appelé depuis 0x400598 mais se trouve dans la même zone "brouillée" (désassemblage quasi uniquement composé d'opcodes invalides). Hypothèse : ces blocs contiennent la logique finale de validation (peut-être un parseur pseudo-XML ou un pseudo-random matcher) qui est déchiffrée au runtime via 0x40035d/0x400363 ou via la clé `sil`.
  +- Pistes pour stabiliser le désassemblage :
- 1.  Instrumenter via gdb en arrêtant juste avant 0x4006e5 dans fcn.004003dd (break \*0x4003f8) puis dumper la mémoire de 0x4006e5 après que les préparations 0x40035d aient tourné (utiliser `dump memory`).
- 2.  Sauvegarder le segment LOAD complet depuis /proc/<pid>/mem après lancement pour observer si la zone 0x4006e5 a été modifiée.
- 3.  Utiliser `r2 -b 8` ou `r2 -e asm.bits=64; ao` pour détecter des instructions valides en alignement alternatif (recherche d'overlapping code).
- 4.  Traçage dynamique avec `strace -f` focalisé sur éventuels read/open pour confirmer si 0x4006e5 fait du I/O caché.
- +#### 10.5 Synthèse provisoire sur condition EAX≠0
  +- fcn.00400158 renvoie eax = résultat de 0x400598, lui-même égal à la valeur retournée par 0x4009e4 (chargée dans r9d après les phases de mélange). Pour que l'impression "OK" s'affiche, il faut donc que 0x4009e4 retourne non nul lorsque appelée avec :
- - struct rbx initialisée (len = 0x400da8-0x400da0, seed = mot de passe XOR anti-debug, pointeurs vers 0x400d40/0x400d60/0x400d80/0x400dc0),
- - ecx = len/2, sil = byte seed (de 0x4003dd: combinaison de pseudo-random issu de 0x4006e5 + len + byte[0x3f]).
    +- Tant que 0x4006e5/0x4009e4 restent brouillées, la condition précise est inconnue. Hypothèses : (a) comparaison contre un flux pseudo-aléatoire généré depuis la clé; (b) require que len/seed aient une propriété (pair/impair, multiple de 10) ; (c) interprétation de l'input comme digits (voir machine d'état sur `\n` à 0x4004fb-0x40058f), donc peut-être besoin d'un formatage (code PIN + checksum) plutôt que texte brut.
- +#### 10.6 Prochaines actions proposées
  +- Placer des breakpoints avant 0x4006e5 et 0x4009e4 pour capturer le code déchiffré en mémoire (gdb + `dump memory`).
  +- Comparer dump mémoire de la zone 0x4006e5 entre exécutions (sans debugger pour éviter anti-debug ?) en instrumentant avec `ptrace` minimal ou `LD_PRELOAD` pour snapshot avant sortie.
  +- Écrire un script r2 pour essayer toutes alignements et tenter `aab; aaaa; pdf @ 0x4006e5` en forçant `e io.cache=true`.
  +- Vérifier si 0x40049b (anti-debug) modifie la clé seed et si lancer sans debugger mais avec `ptrace` neutre (p.ex. `strace -o /tmp/trace`) change la sortie de 0x400598.
- +#### 10.7 Dump mémoire statique des buffers 0x400d30/0x400da0 (vérifier si les zones sont déjà structurées ou auto-déchiffrées)
  +- Contexte : suspicion que 0x4006e5 soit du code auto-modifié ou que les structures soient chiffrées à l'exécution. Objectif : voir si des données cohérentes sont déjà présentes dans le binaire brut.
  +- Installation de xxd (absent par défaut) pour faciliter les lectures hexadécimales :
- - Commande : `apt-get update -y >/tmp/apt-update-xxd.log 2>&1 && apt-get install -y xxd >/tmp/apt-install-xxd.log 2>&1`
    +- Dump du bloc contenant la clé XOR (zone 0x400d30) :
- - Commande : `dd if=bin/Sn0wF4ult bs=1 skip=$((0xd30)) count=$((0x40)) 2>/dev/null | xxd`
- - Résultat :
- - Octets brouillés `e6 9e e9 9a c6 c6 9b c4 ...` suivis de deux pointeurs 0x4009e4 / 0x400c82 (cohérent avec les références identifiées) et de la longueur 0x33 (51) qui correspond à la taille de la zone de texte chiffrée.
    +- Dump de la zone de structures manipulées par 0x400598 (0x400da0) :
- - Commande : `dd if=bin/Sn0wF4ult bs=1 skip=$((0xda0)) count=$((0x80)) 2>/dev/null | xxd`
- - Résultat :
- - qword [0x400da0] = 0x400e20 et [0x400da8] = 0x400e42 → longueur initiale calculée = 0x77 (119) non nulle, donc la fonction 0x400598 ne peut pas retourner 0 par longueur vide (invalide l'hypothèse d'un piège « longueur toujours zéro »).
- - Section 0x400de0 contient la série ASCII `EDGDFD@DGEFD@DGF...` déjà en clair, ce qui suggère que les chaînes ne sont pas toutes chiffrées dynamiquement et qu'il n'y a pas de déchiffrement différé pour ces buffers-là.
    +- Conclusion provisoire : les structures clés ne sont pas vides ni entièrement chiffrées à l'exécution ; 0x4006e5/0x4009e4 semblent donc plutôt transformer un état existant que matérialiser un code inexistant.
- +#### 10.8 Évaluation du risque de « faux crackme » ou de fausses conditions (anti-piste)
  +- Pistes à invalider :
- 1.  **Fonctions volontairement illisibles mais jamais exécutées.**
-     - 0x4006e5 est appelé directement depuis 0x4003dd (qui est lui-même toujours invoqué par fcn.00400158 avant l'anti-debug). Le saut vers 0x4003f8/0x400413 ne comporte pas de condition contournable ; on arrive donc forcément dans la zone brouillée à chaque tentative → pas un leurre trivial.
- 2.  **Condition finale impossible à satisfaire (retour toujours 0).**
-     - Les valeurs statiques montrent `len=0x77` et un seed byte en [rbx+0x50] pris sur le pseudo-aléa généré en 0x4003dd ; 0x400598 ne force pas r9d à 0 sauf si 0x4009e4 le fait. La présence de boucles XOR paramétrées par des pointeurs réels (0x400d60/0x400d80/0x400dc0) laisse penser à un vrai calcul plutôt qu'à une sortie forcée.
- 3.  **Anti-debug bloquant en permanence.**
-     - Le test anti-debug (0x40049b) a déjà été validé hors debugger (TracerPid=0) et l'exécution continue jusqu'au code retour 1 → le refus final vient bien d'autre chose. On peut donc écarter l'hypothèse d'un crackme « impossible » limité à l'anti-debug.
  +- Signaux faibles de possible piège :
- - Le bloc 0x4006e5 reste illisible même via objdump, ce qui peut être un mélange code/données pour faire perdre du temps. Toutefois les pointeurs présents en clair (0x4009e4, 0x400cff) et les longueurs cohérentes renforcent l'idée d'un algorithme réel.
- - La chaîne "EDGDFD@DGE..." pourrait être un bruit destiné à semer le doute ; mais elle est alignée sur les structures attendues et xordynamique via 0x400363, donc plausible comme table.
    +- Plan pour disqualifier rapidement les fausses pistes (éviter de s'enfoncer) :
- - Mesurer dynamiquement si 0x4006e5/0x4009e4 modifient le segment (break sur 0x400d60/0x400d80/0x400dc0 en écriture) pour confirmer un comportement concret ; si aucune écriture ne survient, considérer la piste « faux code » et chercher un autre déclencheur.
- - Comparer le dump du segment texte autour de 0x4006e5 avant/après l'appel de 0x4003dd (outil `gcore` ou `dump memory` gdb) : si identiques, l'hypothèse auto-déchiffrement tombe et il faudra chercher une interprétation en données (table) plutôt qu'en code.
- - Tester un format d'entrée différent (ex. uniquement digits/paires hex, ou longueur 0x77) pour voir si le code final réagit : si aucun changement de code retour, suspecter un leurre et revoir l'analyse initiale (possibilité que la vraie condition soit restée dans 0x4002e3/anti-debug).
    +- Conclusion intermédiaire : pas de preuve d'un « faux crackme » pour l'instant, mais les éléments statiques réduisent les pistes impossibles (len vide, anti-debug bloquant). Les prochaines actions doivent viser à capturer l'état mémoire après 0x4006e5 pour confirmer que le calcul final est réel et éviter de persister dans une zone brouillée inutile.
- +#### 10.9 Test rapide « avant/après 0x4006e5 » pour détecter une éventuelle auto-modification (et vérifier que 0x4006e5 est bien du code)
  +- Objectif : comparer le bloc [0x4006e5, 0x400705) avant et après son exécution, afin de disqualifier l'hypothèse d'un code « fantôme » jamais modifié. Plan : breakpoints sur 0x4003dd (appelant 0x4006e5) et 0x4003f8 (retour), dump du bloc à chaque arrêt.
  +- Préparation : installation de gdb (absent par défaut) `apt-get update -y >/tmp/apt-update-gdb.log 2>&1 && apt-get install -y gdb >/tmp/apt-install-gdb.log 2>&1`.
  +- Tentative 1 (gdb script avec commandes automatiques) : échec, commandes `x/32xb` exécutées hors contexte « inferior » → erreurs "The program is not being run." (mauvais positionnement des commandes dans le script batch).
  +- Tentative 2 (session interactive) :
- - Commandes :
- - `gdb -q bin/Sn0wF4ult`
- - `set pagination off; set logging file /tmp/gdb_mem2.log; set logging enabled on`
- - `b *0x4006c9` (entry), `b *0x4003dd`, `b *0x4003f8`
- - `run test`
- - Résultat : arrêt à l'entrée (0x4006c9) puis **exit immédiat code 015** dès le `continue`, sans jamais atteindre 0x4003dd/0x4003f8. Logs gdb : `[Inferior 1 (process 4928) exited with code 015]`.
- - Interprétation : sous debugger, le crackme déclenche l'anti-debug très tôt (avant l'appel de 0x4003dd), ce qui empêche de capturer l'état du bloc 0x4006e5. Ce comportement plaide contre l'idée d'un « code fantôme » (le binaire réagit activement à l'environnement) et confirme que nos outils doivent contourner l'anti-debug pour observer la section obfusquée.
    +- Suites proposées pour disqualifier (ou confirmer) l'auto-modification :
- - Rejouer la capture mémoire via un wrapper `ptrace(PTRACE_SEIZE)` minimal (pour éviter que `TracerPid` soit non nul) ou via un dumper externe (`gcore` lancé depuis un processus parent non traçable) afin de comparer avant/après sans déclencher l'anti-debug.
- - Utiliser `rr` ou un LD_PRELOAD qui enregistre `mprotect`/`write` sur la zone 0x4006e5 : si aucune écriture n'est détectée hors debugger, reclasser 0x4006e5 comme données obfusquées plutôt que code auto-déchiffré.
- +### 11. Recentrage systémique et plan d'action prioritaire (session courante)
- +Objectif : sortir de l'impasse « comment dumper » et comprendre **pourquoi** le code retour reste à 1 malgré le mot de passe clair `L4C0ll1n3ducr4ck`. Les constats précédents montrent que l'anti-debug n'est pas le seul facteur ; il faut valider ou invalider rapidement les hypothèses structurelles.
- +#### 11.1 Requalification des hypothèses principales
  +- **Intégrité/Checksum** : fcn.00400158 pourrait vérifier l'intégrité du segment .text. Toute modification (breakpoints logiciels, patch) invaliderait le hash et forcerait eax=0. À tester sans toucher au binaire mais en neutralisant l'anti-debug via `LD_PRELOAD` pour éviter l'impact TracerPid.
  +- **Seed dynamique (r15) critique** : 0x400598 consomme un octet seed provenant de 0x4003dd, avant l'anti-debug. Si ce seed dépend d'une lecture /proc ou d'un état d'environnement (PID/heure), un mot de passe correct mais un seed inattendu peuvent rendre 0x4009e4 déterministe à 0. Priorité : capturer la valeur seed hors debugger (exécution simple + instrumentation LD_PRELOAD) et la comparer à celle sous debugger.
  +- **Format d'entrée non textuel** : la boucle 0x4004fb-0x40058f parcourt 10 octets copiés dans le stack et compare chaque octet à la zone [rsp+3], suggérant une forme de parsing (peut-être digits/hex). Il est possible que `L4C0ll1n3ducr4ck` soit une « clé d'ouverture » mais qu'une seconde phase utilise ces 10 octets pour un calcul ou checksum. Vérifier si une entrée numérique de longueur 10 modifie le code retour.
- +#### 11.2 Neutralisation invisible de l'anti-debug (sans patch disque)
  +- **Hook LD_PRELOAD** : écrire une bibliothèque partagée interceptant `open`/`read` ; si le chemin est `/proc/self/status`, renvoyer un contenu statique où `TracerPid:\t0` pour faire passer 0x40049b sans modifier le binaire.
  +- Avantage : permet d'utiliser gdb/ptrace sur le processus fils sans que `TracerPid` soit non nul vu depuis le crackme (si l'injection PRELOAD se fait via un parent propre), tout en laissant l'intégrité disque intacte (utile si fcn.00400158 fait un checksum du .text).
  +- Action concrète à loguer : compiler `bypass.c` → `bypass.so`, lancer `LD_PRELOAD=./bypass.so ./bin/Sn0wF4ult L4C0ll1n3ducr4ck` et observer code retour + éventuelle sortie. Si succès, refaire avec gdb pour dumper la zone 0x4006e5/0x4009e4.
- +#### 11.3 Validation rapide de l'hypothèse « checksum du .text »
  +- **Test sans breakpoint ni patch** : déjà fait, code retour = 1 → soit le checksum échoue par conception (donc attendu), soit le seed invalide le calcul final.
  +- **Test avec copie du binaire** : exécuter depuis un chemin différent ou via un wrapper pour voir si argv[0] ou l'inode joue un rôle. Si le code retour change, suspicion d'un hachage incluant le nom du binaire.
  +- **Observation écriture mémoire** : instrumenter `mprotect`/`write` via LD_PRELOAD pour savoir si le code touche son propre segment. S'il calcule un hash sans écrire, il devrait lire en boucle 0x400000.. ; ces lectures peuvent être détectées via `perf`/`ptrace` ou en plaçant des watchpoints matériels (si gdb devient possible après hook TracerPid).
- +#### 11.4 Décodage ciblé de 0x4006e5/0x4009e4 sans s'enfermer
  +- **Alignements alternatifs** : lancer `r2 -qc "s 0x4006e5; pd 32; s 0x4006e6; pd 32; s 0x4006e7; pd 32" bin/Sn0wF4ult` pour détecter un prologue lisible (pattern push rbp / mov rbp, rsp). Si aucun alignement ne donne du code cohérent, reclasser 0x4006e5 comme table/VM et chercher les interprètes ailleurs (pointeurs de fonction ou décodage bytecode dans 0x400598).
  +- **Dump en exécution non tracée** : utiliser un parent non débogué qui fork/exec le crackme et lit `/proc/<pid>/mem` avant la fin pour capturer la zone [0x4006e5,0x400a50]. Permet de voir si un auto-déchiffrement a lieu sans introduire TracerPid ≠ 0. Si le dump est identique au binaire brut, il faudra chercher une sémantique « données ». Si différent, le désassemblage du dump donnera la vraie logique.
- +#### 11.5 Expériences rapides à prioriser (ordre) pour éviter le « trou de souris »
  +1. **LD_PRELOAD anti-anti-debug** + exécution simple : vérifier code retour et valeur seed (instrumenter getrandom/time si appelés).
  +2. **Entrée alternative (10 digits/hex)** : tester un input de longueur 10 pour voir si 0x400598/0x4009e4 se comporte différemment (code retour ≠1?).
  +3. **Dump mémoire sans debugger** : /proc/<pid>/mem sur la zone 0x4006e5/0x4009e4 après lancement sous PRELOAD. Analyse du dump avec `objdump -bbinary --adjust-vma=0x400000 -D`.
  +4. **Alignement r2** : si dump montre code intact, essayer désassemblages multi-offset pour identifier un interprète ou une VM.
- +#### 11.6 Critère d'arrêt pour les fausses pistes
  +- Si après PRELOAD + dump la zone 0x4006e5 reste inchangée et aucune écriture n'est observée sur 0x400d60/0x400d80/0x400dc0, considérer que 0x4006e5 est une table immuable et réorienter l'analyse vers :
- - la sémantique de la boucle 0x4004fb-0x40058f (format d'entrée) ;
- - la provenance exacte de r15 (seed) et son rôle dans 0x400598 ;
- - un possible second mot de passe ou checksum intégré aux données statiques.
- +Conclusion de session : la priorité est de débloquer l'observation du chemin final sans altérer l'intégrité (LD_PRELOAD) et de mesurer l'impact du seed r15. Les prochaines actions doivent être journalisées (commande + résultat) pour trancher rapidement entre « code chiffré réel » et « table obfusquée », et éviter de persister dans une instrumentation qui fausse l'état (TracerPid non nul, breakpoints).
- +### 12. Désassemblage binaire brut (sans en-têtes de section)
  +- Installation des outils manquants (`file`, `binutils`, `gdb`, `strace`, `ltrace`, `xxd`) pour pouvoir désassembler un ELF sans section headers. Commande clé :
- - `objdump -D -b binary -m i386:x86-64 --adjust-vma=0x400000 bin/Sn0wF4ult > /tmp/disasm.txt`.
    +- Le binaire est un seul segment RWE (pas d'en-têtes de section) avec un point d’entrée à `0x4006c9`. La routine principale est à `0x4000b0`.
    +- Les structures statiques à `0x400d40/0x400d60/0x400d80/0x400da0` contiennent des couples (start, end, len) pointant notamment vers la plage `0x4009e4–0x400c82` et vers des tampons dérivés (`0x400e00`, `0x400e1c`, etc.).
    +- Le message de succès "Good Password" est présent à `0x400c90` et la chaîne XOR de mot de passe à `0x400d30`.
- +### 13. Compréhension du flux final (0x400158 → 0x400598)
  +- `0x400158` nettoie un tampon de 0x1e octets, appelle `fcn.004003dd` (charge/altère le stack frame via le bloc obfusqué 0x4006e5) puis `fcn.0040049b` (anti-debug via `/proc/self/status`).
  +- Si le XOR (`0x4002e3`) ne matche pas, le code sort immédiatement avec `exit(13)` via `0x4002d9` (observé dans l’exécution strace). Sinon, il enchaîne vers la chaîne `0x400214 → 0x400598 → 0x4009e4`.
  +- `0x400598` initialise quatre structures (bases `0x400d40/0x400d60/0x400d80/0x400dc0`), calcule un diviseur/quotient dépendant du seed (`r15` = octet issu de `0x4003dd`), puis appelle `0x4009e4` avec :
- - `rdi` = contexte struct (stack),
- - `rsi` = seed (r15b),
- - `rdx` = ptr/taille dérivée,
- - `rcx` = demi-longueur du bloc primaire.
    +- Le résultat (`eax`) est réinjecté dans plusieurs XOR/ROL/OR avec les structures, puis converti en index pour la routine 0x400363. La sortie finale de `0x400598` remonte à `0x400158` qui la teste dans `0x400133` (succès si `eax != 0`).
- +### 14. Observation déterminante : succès uniquement sous traçage
  +- Exécution directe avec la clé récupérée « L4C0ll1n3ducr4ck » reste silencieuse et retourne 1.
- - `bin/Sn0wF4ult L4C0ll1n3ducr4ck` → code retour 1 (pas de sortie).
    +- Exécution **sous traçage** (`strace`) avec la même clé affiche immédiatement « Good Password » puis fait un `exit(13)` (trace `/proc/self/status` lue trois fois + `clock_gettime`).
- - `strace -o /tmp/trace.txt bin/Sn0wF4ult L4C0ll1n3ducr4ck` → impression du message, sortie 13.
    +- Hypothèse validée : le seed ou l’anti-debug s’inverse en présence de `TracerPid != 0`, rendant la vérification `0x400598/0x4009e4` vraie. Sans traceur, le même chemin retourne 0.
    +- Commande actuelle pour obtenir le message « OK » (Good Password) de manière reproductible : lancer le binaire sous ptrace (ex. `strace`) avec le mot de passe XOR extrait.
- +### 15. Proposition de solution reproductible
  +1. Utiliser la clé extraite par `0x4002e3` : `L4C0ll1n3ducr4ck` (16 octets, longueur attendue par la boucle d’entrée).
  +2. Forcer un contexte où `TracerPid` est non nul (ptrace actif) pour que `0x400598/0x4009e4` renvoie une valeur non nulle.
- - Exemple reproductible :
-     - `strace -o /tmp/trace.txt ./bin/Sn0wF4ult L4C0ll1n3ducr4ck`
- - Effet observé : impression de « Good Password » suivie d’un `exit(13)` (conforme à la voie « succès » interne malgré le code de sortie non nul).
    +3. Pour une exploitation « propre », on peut remplacer `strace` par tout tracer léger (ptrace attach) ou un `LD_PRELOAD` qui renvoie un `TracerPid` factice non nul.
-
