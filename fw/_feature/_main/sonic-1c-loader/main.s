
main.o:     file format elf32-littlearm


Disassembly of section .text.NVIC_SystemReset:

00000000 <NVIC_SystemReset>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	f3bf 8f4f 	dsb	sy
   8:	bf00      	nop
   a:	4b07      	ldr	r3, [pc, #28]	; (28 <NVIC_SystemReset+0x28>)
   c:	68db      	ldr	r3, [r3, #12]
   e:	f403 62e0 	and.w	r2, r3, #1792	; 0x700
  12:	4905      	ldr	r1, [pc, #20]	; (28 <NVIC_SystemReset+0x28>)
  14:	4b05      	ldr	r3, [pc, #20]	; (2c <NVIC_SystemReset+0x2c>)
  16:	4313      	orrs	r3, r2
  18:	60cb      	str	r3, [r1, #12]
  1a:	f3bf 8f4f 	dsb	sy
  1e:	bf00      	nop
  20:	bf00      	nop
  22:	bf00      	nop
  24:	e7fc      	b.n	20 <NVIC_SystemReset+0x20>
  26:	bf00      	nop
  28:	e000ed00 	.word	0xe000ed00
  2c:	05fa0004 	.word	0x05fa0004

Disassembly of section .text.main:

00000000 <main>:
   0:	b580      	push	{r7, lr}
   2:	b082      	sub	sp, #8
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	6039      	str	r1, [r7, #0]
   a:	4810      	ldr	r0, [pc, #64]	; (4c <main+0x4c>)
   c:	f7ff fffe 	bl	0 <hw_gpio_reset>
  10:	f7ff fffe 	bl	0 <main>
  14:	4603      	mov	r3, r0
  16:	f083 0301 	eor.w	r3, r3, #1
  1a:	b2db      	uxtb	r3, r3
  1c:	2b00      	cmp	r3, #0
  1e:	d00f      	beq.n	40 <main+0x40>
  20:	480a      	ldr	r0, [pc, #40]	; (4c <main+0x4c>)
  22:	f7ff fffe 	bl	0 <hw_gpio_set>
  26:	e004      	b.n	32 <main+0x32>
  28:	4808      	ldr	r0, [pc, #32]	; (4c <main+0x4c>)
  2a:	f7ff fffe 	bl	0 <hw_gpio_reset>
  2e:	f7ff fffe 	bl	0 <main>
  32:	4b07      	ldr	r3, [pc, #28]	; (50 <main+0x50>)
  34:	781b      	ldrb	r3, [r3, #0]
  36:	2b07      	cmp	r3, #7
  38:	d1f6      	bne.n	28 <main+0x28>
  3a:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
  3e:	e001      	b.n	44 <main+0x44>
  40:	f06f 0301 	mvn.w	r3, #1
  44:	4618      	mov	r0, r3
  46:	3708      	adds	r7, #8
  48:	46bd      	mov	sp, r7
  4a:	bd80      	pop	{r7, pc}
	...

Disassembly of section .text.bootstrap_service:

00000000 <bootstrap_service>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	4b30      	ldr	r3, [pc, #192]	; (c8 <bootstrap_service+0xc8>)
   6:	781b      	ldrb	r3, [r3, #0]
   8:	3b01      	subs	r3, #1
   a:	2b06      	cmp	r3, #6
   c:	d859      	bhi.n	c2 <bootstrap_service+0xc2>
   e:	a201      	add	r2, pc, #4	; (adr r2, 14 <bootstrap_service+0x14>)
  10:	f852 f023 	ldr.w	pc, [r2, r3, lsl #2]
  14:	00000031 	.word	0x00000031
  18:	0000004b 	.word	0x0000004b
  1c:	00000063 	.word	0x00000063
  20:	0000007b 	.word	0x0000007b
  24:	00000093 	.word	0x00000093
  28:	000000b3 	.word	0x000000b3
  2c:	000000bd 	.word	0x000000bd
  30:	f7ff fffe 	bl	0 <bootstrap_service>
  34:	4b24      	ldr	r3, [pc, #144]	; (c8 <bootstrap_service+0xc8>)
  36:	f893 33e0 	ldrb.w	r3, [r3, #992]	; 0x3e0
  3a:	2b00      	cmp	r3, #0
  3c:	d101      	bne.n	42 <bootstrap_service+0x42>
  3e:	2202      	movs	r2, #2
  40:	e000      	b.n	44 <bootstrap_service+0x44>
  42:	2207      	movs	r2, #7
  44:	4b20      	ldr	r3, [pc, #128]	; (c8 <bootstrap_service+0xc8>)
  46:	701a      	strb	r2, [r3, #0]
  48:	e03c      	b.n	c4 <bootstrap_service+0xc4>
  4a:	f7ff fffe 	bl	0 <bootstrap_service>
  4e:	4b1f      	ldr	r3, [pc, #124]	; (cc <bootstrap_service+0xcc>)
  50:	691b      	ldr	r3, [r3, #16]
  52:	2b00      	cmp	r3, #0
  54:	db01      	blt.n	5a <bootstrap_service+0x5a>
  56:	2203      	movs	r2, #3
  58:	e000      	b.n	5c <bootstrap_service+0x5c>
  5a:	2207      	movs	r2, #7
  5c:	4b1a      	ldr	r3, [pc, #104]	; (c8 <bootstrap_service+0xc8>)
  5e:	701a      	strb	r2, [r3, #0]
  60:	e030      	b.n	c4 <bootstrap_service+0xc4>
  62:	f7ff fffe 	bl	0 <bootstrap_service>
  66:	4b19      	ldr	r3, [pc, #100]	; (cc <bootstrap_service+0xcc>)
  68:	691b      	ldr	r3, [r3, #16]
  6a:	2b00      	cmp	r3, #0
  6c:	db01      	blt.n	72 <bootstrap_service+0x72>
  6e:	2204      	movs	r2, #4
  70:	e000      	b.n	74 <bootstrap_service+0x74>
  72:	2207      	movs	r2, #7
  74:	4b14      	ldr	r3, [pc, #80]	; (c8 <bootstrap_service+0xc8>)
  76:	701a      	strb	r2, [r3, #0]
  78:	e024      	b.n	c4 <bootstrap_service+0xc4>
  7a:	f7ff fffe 	bl	0 <bootstrap_service>
  7e:	4b13      	ldr	r3, [pc, #76]	; (cc <bootstrap_service+0xcc>)
  80:	691b      	ldr	r3, [r3, #16]
  82:	2b00      	cmp	r3, #0
  84:	db01      	blt.n	8a <bootstrap_service+0x8a>
  86:	2205      	movs	r2, #5
  88:	e000      	b.n	8c <bootstrap_service+0x8c>
  8a:	2207      	movs	r2, #7
  8c:	4b0e      	ldr	r3, [pc, #56]	; (c8 <bootstrap_service+0xc8>)
  8e:	701a      	strb	r2, [r3, #0]
  90:	e018      	b.n	c4 <bootstrap_service+0xc4>
  92:	f7ff fffe 	bl	0 <bootstrap_service>
  96:	4b0c      	ldr	r3, [pc, #48]	; (c8 <bootstrap_service+0xc8>)
  98:	f8d3 21c8 	ldr.w	r2, [r3, #456]	; 0x1c8
  9c:	4b0a      	ldr	r3, [pc, #40]	; (c8 <bootstrap_service+0xc8>)
  9e:	f8d3 31c0 	ldr.w	r3, [r3, #448]	; 0x1c0
  a2:	429a      	cmp	r2, r3
  a4:	d101      	bne.n	aa <bootstrap_service+0xaa>
  a6:	2206      	movs	r2, #6
  a8:	e000      	b.n	ac <bootstrap_service+0xac>
  aa:	2205      	movs	r2, #5
  ac:	4b06      	ldr	r3, [pc, #24]	; (c8 <bootstrap_service+0xc8>)
  ae:	701a      	strb	r2, [r3, #0]
  b0:	e008      	b.n	c4 <bootstrap_service+0xc4>
  b2:	f7ff fffe 	bl	0 <bootstrap_service>
  b6:	f7ff fffe 	bl	0 <bootstrap_service>
  ba:	e003      	b.n	c4 <bootstrap_service+0xc4>
  bc:	f7ff fffe 	bl	0 <bootstrap_service>
  c0:	e000      	b.n	c4 <bootstrap_service+0xc4>
  c2:	bf00      	nop
  c4:	bf00      	nop
  c6:	bd80      	pop	{r7, pc}
  c8:	00000000 	.word	0x00000000
  cc:	40023c00 	.word	0x40023c00

Disassembly of section .text.loader_open:

00000000 <loader_open>:
   0:	b580      	push	{r7, lr}
   2:	b084      	sub	sp, #16
   4:	af04      	add	r7, sp, #16
   6:	2201      	movs	r2, #1
   8:	490f      	ldr	r1, [pc, #60]	; (48 <loader_open+0x48>)
   a:	4810      	ldr	r0, [pc, #64]	; (4c <loader_open+0x4c>)
   c:	f7ff fffe 	bl	0 <f_open>
  10:	4603      	mov	r3, r0
  12:	461a      	mov	r2, r3
  14:	4b0e      	ldr	r3, [pc, #56]	; (50 <loader_open+0x50>)
  16:	f883 23e0 	strb.w	r2, [r3, #992]	; 0x3e0
  1a:	4b0d      	ldr	r3, [pc, #52]	; (50 <loader_open+0x50>)
  1c:	f893 33e0 	ldrb.w	r3, [r3, #992]	; 0x3e0
  20:	2b00      	cmp	r3, #0
  22:	d10d      	bne.n	40 <loader_open+0x40>
  24:	2300      	movs	r3, #0
  26:	9303      	str	r3, [sp, #12]
  28:	2383      	movs	r3, #131	; 0x83
  2a:	9302      	str	r3, [sp, #8]
  2c:	4b09      	ldr	r3, [pc, #36]	; (54 <loader_open+0x54>)
  2e:	9301      	str	r3, [sp, #4]
  30:	4b09      	ldr	r3, [pc, #36]	; (58 <loader_open+0x58>)
  32:	9300      	str	r3, [sp, #0]
  34:	4b09      	ldr	r3, [pc, #36]	; (5c <loader_open+0x5c>)
  36:	4a0a      	ldr	r2, [pc, #40]	; (60 <loader_open+0x60>)
  38:	4904      	ldr	r1, [pc, #16]	; (4c <loader_open+0x4c>)
  3a:	480a      	ldr	r0, [pc, #40]	; (64 <loader_open+0x64>)
  3c:	f7ff fffe 	bl	0 <srec_reader_init>
  40:	bf00      	nop
  42:	46bd      	mov	sp, r7
  44:	bd80      	pop	{r7, pc}
  46:	bf00      	nop
  48:	00000000 	.word	0x00000000
  4c:	000001b4 	.word	0x000001b4
  50:	00000000 	.word	0x00000000
  54:	00000001 	.word	0x00000001
	...
  64:	00000090 	.word	0x00000090

Disassembly of section .text.loader_loading:

00000000 <loader_loading>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	4802      	ldr	r0, [pc, #8]	; (10 <loader_loading+0x10>)
   6:	f7ff fffe 	bl	0 <srec_reader_read>
   a:	bf00      	nop
   c:	bd80      	pop	{r7, pc}
   e:	bf00      	nop
  10:	00000090 	.word	0x00000090

Disassembly of section .text.loader_complete:

00000000 <loader_complete>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	4802      	ldr	r0, [pc, #8]	; (10 <loader_complete+0x10>)
   6:	f7ff fffe 	bl	0 <f_close>
   a:	bf00      	nop
   c:	bd80      	pop	{r7, pc}
   e:	bf00      	nop
  10:	000001b4 	.word	0x000001b4

Disassembly of section .text.system_reset:

00000000 <system_reset>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	f7ff fffe 	bl	0 <system_reset>
   8:	e7fe      	b.n	8 <system_reset+0x8>

Disassembly of section .text.boot_memset:

00000000 <boot_memset>:
   0:	b480      	push	{r7}
   2:	b087      	sub	sp, #28
   4:	af00      	add	r7, sp, #0
   6:	60f8      	str	r0, [r7, #12]
   8:	460b      	mov	r3, r1
   a:	607a      	str	r2, [r7, #4]
   c:	72fb      	strb	r3, [r7, #11]
   e:	68fb      	ldr	r3, [r7, #12]
  10:	617b      	str	r3, [r7, #20]
  12:	e002      	b.n	1a <boot_memset+0x1a>
  14:	697b      	ldr	r3, [r7, #20]
  16:	7afa      	ldrb	r2, [r7, #11]
  18:	701a      	strb	r2, [r3, #0]
  1a:	687b      	ldr	r3, [r7, #4]
  1c:	1e5a      	subs	r2, r3, #1
  1e:	607a      	str	r2, [r7, #4]
  20:	2b00      	cmp	r3, #0
  22:	d1f7      	bne.n	14 <boot_memset+0x14>
  24:	bf00      	nop
  26:	bf00      	nop
  28:	371c      	adds	r7, #28
  2a:	46bd      	mov	sp, r7
  2c:	bc80      	pop	{r7}
  2e:	4770      	bx	lr

Disassembly of section .text.boot_delay:

00000000 <boot_delay>:
   0:	b480      	push	{r7}
   2:	b083      	sub	sp, #12
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	4b11      	ldr	r3, [pc, #68]	; (50 <boot_delay+0x50>)
   a:	2200      	movs	r2, #0
   c:	601a      	str	r2, [r3, #0]
   e:	e013      	b.n	38 <boot_delay+0x38>
  10:	4b0f      	ldr	r3, [pc, #60]	; (50 <boot_delay+0x50>)
  12:	2200      	movs	r2, #0
  14:	601a      	str	r2, [r3, #0]
  16:	e004      	b.n	22 <boot_delay+0x22>
  18:	4b0e      	ldr	r3, [pc, #56]	; (54 <boot_delay+0x54>)
  1a:	681b      	ldr	r3, [r3, #0]
  1c:	3301      	adds	r3, #1
  1e:	4a0d      	ldr	r2, [pc, #52]	; (54 <boot_delay+0x54>)
  20:	6013      	str	r3, [r2, #0]
  22:	4b0c      	ldr	r3, [pc, #48]	; (54 <boot_delay+0x54>)
  24:	681b      	ldr	r3, [r3, #0]
  26:	f242 720f 	movw	r2, #9999	; 0x270f
  2a:	4293      	cmp	r3, r2
  2c:	ddf4      	ble.n	18 <boot_delay+0x18>
  2e:	4b08      	ldr	r3, [pc, #32]	; (50 <boot_delay+0x50>)
  30:	681b      	ldr	r3, [r3, #0]
  32:	3301      	adds	r3, #1
  34:	4a06      	ldr	r2, [pc, #24]	; (50 <boot_delay+0x50>)
  36:	6013      	str	r3, [r2, #0]
  38:	4b05      	ldr	r3, [pc, #20]	; (50 <boot_delay+0x50>)
  3a:	681b      	ldr	r3, [r3, #0]
  3c:	687a      	ldr	r2, [r7, #4]
  3e:	429a      	cmp	r2, r3
  40:	dce6      	bgt.n	10 <boot_delay+0x10>
  42:	bf00      	nop
  44:	bf00      	nop
  46:	370c      	adds	r7, #12
  48:	46bd      	mov	sp, r7
  4a:	bc80      	pop	{r7}
  4c:	4770      	bx	lr
  4e:	bf00      	nop
	...

Disassembly of section .text.boot_blink:

00000000 <boot_blink>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	4803      	ldr	r0, [pc, #12]	; (14 <boot_blink+0x14>)
   6:	f7ff fffe 	bl	0 <hw_gpio_toggle>
   a:	2080      	movs	r0, #128	; 0x80
   c:	f7ff fffe 	bl	0 <boot_blink>
  10:	bf00      	nop
  12:	bd80      	pop	{r7, pc}
  14:	00000000 	.word	0x00000000

Disassembly of section .text.boot_wait_busy:

00000000 <boot_wait_busy>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	e001      	b.n	a <boot_wait_busy+0xa>
   6:	f7ff fffe 	bl	0 <boot_wait_busy>
   a:	f7ff fffe 	bl	0 <boot_wait_busy>
   e:	4603      	mov	r3, r0
  10:	2b00      	cmp	r3, #0
  12:	d1f8      	bne.n	6 <boot_wait_busy+0x6>
  14:	bf00      	nop
  16:	bf00      	nop
  18:	bd80      	pop	{r7, pc}

Disassembly of section .text.s19_meta_fn:

00000000 <s19_meta_fn>:
   0:	b480      	push	{r7}
   2:	b083      	sub	sp, #12
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	2300      	movs	r3, #0
   a:	4618      	mov	r0, r3
   c:	370c      	adds	r7, #12
   e:	46bd      	mov	sp, r7
  10:	bc80      	pop	{r7}
  12:	4770      	bx	lr

Disassembly of section .text.s19_store_fn:

00000000 <s19_store_fn>:
   0:	b580      	push	{r7, lr}
   2:	b086      	sub	sp, #24
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	687b      	ldr	r3, [r7, #4]
   a:	3304      	adds	r3, #4
   c:	60fb      	str	r3, [r7, #12]
   e:	68fb      	ldr	r3, [r7, #12]
  10:	3308      	adds	r3, #8
  12:	617b      	str	r3, [r7, #20]
  14:	68fb      	ldr	r3, [r7, #12]
  16:	685b      	ldr	r3, [r3, #4]
  18:	60bb      	str	r3, [r7, #8]
  1a:	2300      	movs	r3, #0
  1c:	827b      	strh	r3, [r7, #18]
  1e:	e00b      	b.n	38 <s19_store_fn+0x38>
  20:	f7ff fffe 	bl	0 <s19_store_fn>
  24:	697b      	ldr	r3, [r7, #20]
  26:	1d1a      	adds	r2, r3, #4
  28:	617a      	str	r2, [r7, #20]
  2a:	4619      	mov	r1, r3
  2c:	68b8      	ldr	r0, [r7, #8]
  2e:	f7ff fffe 	bl	0 <s19_store_fn>
  32:	8a7b      	ldrh	r3, [r7, #18]
  34:	3304      	adds	r3, #4
  36:	827b      	strh	r3, [r7, #18]
  38:	68fb      	ldr	r3, [r7, #12]
  3a:	885b      	ldrh	r3, [r3, #2]
  3c:	8a7a      	ldrh	r2, [r7, #18]
  3e:	429a      	cmp	r2, r3
  40:	d3ee      	bcc.n	20 <s19_store_fn+0x20>
  42:	f7ff fffe 	bl	0 <s19_store_fn>
  46:	2300      	movs	r3, #0
  48:	4618      	mov	r0, r3
  4a:	3718      	adds	r7, #24
  4c:	46bd      	mov	sp, r7
  4e:	bd80      	pop	{r7, pc}

Disassembly of section .text.s19_term_fn:

00000000 <s19_term_fn>:
   0:	b480      	push	{r7}
   2:	b085      	sub	sp, #20
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	687b      	ldr	r3, [r7, #4]
   a:	3304      	adds	r3, #4
   c:	60bb      	str	r3, [r7, #8]
   e:	2300      	movs	r3, #0
  10:	81fb      	strh	r3, [r7, #14]
  12:	e002      	b.n	1a <s19_term_fn+0x1a>
  14:	89fb      	ldrh	r3, [r7, #14]
  16:	3301      	adds	r3, #1
  18:	81fb      	strh	r3, [r7, #14]
  1a:	68bb      	ldr	r3, [r7, #8]
  1c:	885b      	ldrh	r3, [r3, #2]
  1e:	89fa      	ldrh	r2, [r7, #14]
  20:	429a      	cmp	r2, r3
  22:	d3f7      	bcc.n	14 <s19_term_fn+0x14>
  24:	2300      	movs	r3, #0
  26:	4618      	mov	r0, r3
  28:	3714      	adds	r7, #20
  2a:	46bd      	mov	sp, r7
  2c:	bc80      	pop	{r7}
  2e:	4770      	bx	lr

Disassembly of section .text.flash_unlock:

00000000 <flash_unlock>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	4b04      	ldr	r3, [pc, #16]	; (18 <flash_unlock+0x18>)
   6:	4a05      	ldr	r2, [pc, #20]	; (1c <flash_unlock+0x1c>)
   8:	605a      	str	r2, [r3, #4]
   a:	4b03      	ldr	r3, [pc, #12]	; (18 <flash_unlock+0x18>)
   c:	4a04      	ldr	r2, [pc, #16]	; (20 <flash_unlock+0x20>)
   e:	605a      	str	r2, [r3, #4]
  10:	bf00      	nop
  12:	46bd      	mov	sp, r7
  14:	bc80      	pop	{r7}
  16:	4770      	bx	lr
  18:	40023c00 	.word	0x40023c00
  1c:	45670123 	.word	0x45670123
  20:	cdef89ab 	.word	0xcdef89ab

Disassembly of section .text.flash_lock:

00000000 <flash_lock>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	4b04      	ldr	r3, [pc, #16]	; (18 <flash_lock+0x18>)
   6:	691b      	ldr	r3, [r3, #16]
   8:	4a03      	ldr	r2, [pc, #12]	; (18 <flash_lock+0x18>)
   a:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
   e:	6113      	str	r3, [r2, #16]
  10:	bf00      	nop
  12:	46bd      	mov	sp, r7
  14:	bc80      	pop	{r7}
  16:	4770      	bx	lr
  18:	40023c00 	.word	0x40023c00

Disassembly of section .text.flash_locked:

00000000 <flash_locked>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	4b03      	ldr	r3, [pc, #12]	; (14 <flash_locked+0x14>)
   6:	691b      	ldr	r3, [r3, #16]
   8:	0fdb      	lsrs	r3, r3, #31
   a:	b2db      	uxtb	r3, r3
   c:	4618      	mov	r0, r3
   e:	46bd      	mov	sp, r7
  10:	bc80      	pop	{r7}
  12:	4770      	bx	lr
  14:	40023c00 	.word	0x40023c00

Disassembly of section .text.flash_busy:

00000000 <flash_busy>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	4b06      	ldr	r3, [pc, #24]	; (20 <flash_busy+0x20>)
   6:	68db      	ldr	r3, [r3, #12]
   8:	f403 3380 	and.w	r3, r3, #65536	; 0x10000
   c:	2b00      	cmp	r3, #0
   e:	bf14      	ite	ne
  10:	2301      	movne	r3, #1
  12:	2300      	moveq	r3, #0
  14:	b2db      	uxtb	r3, r3
  16:	4618      	mov	r0, r3
  18:	46bd      	mov	sp, r7
  1a:	bc80      	pop	{r7}
  1c:	4770      	bx	lr
  1e:	bf00      	nop
  20:	40023c00 	.word	0x40023c00

Disassembly of section .text.flash_erase:

00000000 <flash_erase>:
   0:	b580      	push	{r7, lr}
   2:	af00      	add	r7, sp, #0
   4:	f7ff fffe 	bl	0 <flash_erase>
   8:	4b07      	ldr	r3, [pc, #28]	; (28 <flash_erase+0x28>)
   a:	691b      	ldr	r3, [r3, #16]
   c:	4a06      	ldr	r2, [pc, #24]	; (28 <flash_erase+0x28>)
   e:	f043 0304 	orr.w	r3, r3, #4
  12:	6113      	str	r3, [r2, #16]
  14:	4b04      	ldr	r3, [pc, #16]	; (28 <flash_erase+0x28>)
  16:	691b      	ldr	r3, [r3, #16]
  18:	4a03      	ldr	r2, [pc, #12]	; (28 <flash_erase+0x28>)
  1a:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
  1e:	6113      	str	r3, [r2, #16]
  20:	f7ff fffe 	bl	0 <flash_erase>
  24:	bf00      	nop
  26:	bd80      	pop	{r7, pc}
  28:	40023c00 	.word	0x40023c00

Disassembly of section .text.flash_setup:

00000000 <flash_setup>:
   0:	b480      	push	{r7}
   2:	af00      	add	r7, sp, #0
   4:	4b07      	ldr	r3, [pc, #28]	; (24 <flash_setup+0x24>)
   6:	691b      	ldr	r3, [r3, #16]
   8:	4a06      	ldr	r2, [pc, #24]	; (24 <flash_setup+0x24>)
   a:	f423 7340 	bic.w	r3, r3, #768	; 0x300
   e:	6113      	str	r3, [r2, #16]
  10:	4b04      	ldr	r3, [pc, #16]	; (24 <flash_setup+0x24>)
  12:	691b      	ldr	r3, [r3, #16]
  14:	4a03      	ldr	r2, [pc, #12]	; (24 <flash_setup+0x24>)
  16:	f443 7300 	orr.w	r3, r3, #512	; 0x200
  1a:	6113      	str	r3, [r2, #16]
  1c:	bf00      	nop
  1e:	46bd      	mov	sp, r7
  20:	bc80      	pop	{r7}
  22:	4770      	bx	lr
  24:	40023c00 	.word	0x40023c00

Disassembly of section .text.flash_write:

00000000 <flash_write>:
   0:	b480      	push	{r7}
   2:	b083      	sub	sp, #12
   4:	af00      	add	r7, sp, #0
   6:	6078      	str	r0, [r7, #4]
   8:	6039      	str	r1, [r7, #0]
   a:	4b09      	ldr	r3, [pc, #36]	; (30 <flash_write+0x30>)
   c:	691b      	ldr	r3, [r3, #16]
   e:	4a08      	ldr	r2, [pc, #32]	; (30 <flash_write+0x30>)
  10:	f043 0301 	orr.w	r3, r3, #1
  14:	6113      	str	r3, [r2, #16]
  16:	687b      	ldr	r3, [r7, #4]
  18:	683a      	ldr	r2, [r7, #0]
  1a:	6812      	ldr	r2, [r2, #0]
  1c:	601a      	str	r2, [r3, #0]
  1e:	f3bf 8f4f 	dsb	sy
  22:	bf00      	nop
  24:	bf00      	nop
  26:	370c      	adds	r7, #12
  28:	46bd      	mov	sp, r7
  2a:	bc80      	pop	{r7}
  2c:	4770      	bx	lr
  2e:	bf00      	nop
  30:	40023c00 	.word	0x40023c00
