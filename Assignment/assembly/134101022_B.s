	.file	"134101022_B.c"
	.section	.rodata
	.align 4
.LC2:
	.string	"\n\nFinal  stable matching is\n\n "
.LC3:
	.string	"Men      Women"
.LC4:
	.string	"  %d         %d\n"
	.align 32
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.align 32
.LC1:
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	andl	$-16, %esp
	subl	$400, %esp
	leal	20(%esp), %edx
	movl	$.LC0, %ebx
	.cfi_offset 3, -20
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	movl	$25, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	leal	120(%esp), %edx
	movl	$.LC1, %ebx
	movl	$25, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	movl	$-1, 320(%esp)
	movl	$-1, 324(%esp)
	movl	$-1, 328(%esp)
	movl	$-1, 332(%esp)
	movl	$-1, 336(%esp)
	movl	$-1, 340(%esp)
	movl	$-1, 344(%esp)
	movl	$-1, 348(%esp)
	movl	$-1, 352(%esp)
	movl	$-1, 356(%esp)
	movl	$-1, 360(%esp)
	movl	$-1, 364(%esp)
	movl	$-1, 368(%esp)
	movl	$-1, 372(%esp)
	movl	$-1, 376(%esp)
	movl	$-1, 380(%esp)
	movl	$0, 380(%esp)
	jmp	.L2
.L5:
	movl	$0, 384(%esp)
	jmp	.L3
.L4:
	movl	380(%esp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	384(%esp), %eax
	movl	120(%esp,%eax,4), %ecx
	movl	380(%esp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	leal	(%eax,%ecx), %edx
	movl	384(%esp), %eax
	movl	%eax, 220(%esp,%edx,4)
	addl	$1, 384(%esp)
.L3:
	cmpl	$4, 384(%esp)
	jle	.L4
	addl	$1, 380(%esp)
.L2:
	cmpl	$4, 380(%esp)
	jle	.L5
	movl	$0, 384(%esp)
	jmp	.L6
.L12:
	addl	$1, 380(%esp)
	movl	380(%esp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	%edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 380(%esp)
	movl	380(%esp), %eax
	movl	340(%esp,%eax,4), %eax
	cmpl	$-1, %eax
	jne	.L7
	movl	380(%esp), %eax
	movl	320(%esp,%eax,4), %eax
	leal	1(%eax), %edx
	movl	380(%esp), %eax
	movl	%edx, 320(%esp,%eax,4)
	movl	380(%esp), %eax
	movl	320(%esp,%eax,4), %eax
	movl	%eax, 388(%esp)
	jmp	.L8
.L11:
	movl	380(%esp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	388(%esp), %eax
	movl	20(%esp,%eax,4), %eax
	movl	%eax, 392(%esp)
	movl	392(%esp), %eax
	movl	360(%esp,%eax,4), %eax
	cmpl	$-1, %eax
	jne	.L9
	movl	380(%esp), %eax
	movl	392(%esp), %edx
	movl	%edx, 340(%esp,%eax,4)
	movl	392(%esp), %eax
	movl	380(%esp), %edx
	movl	%edx, 360(%esp,%eax,4)
	jmp	.L8
.L9:
	movl	392(%esp), %eax
	movl	360(%esp,%eax,4), %eax
	movl	%eax, 396(%esp)
	movl	392(%esp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	396(%esp), %eax
	movl	220(%esp,%eax,4), %ecx
	movl	392(%esp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	380(%esp), %eax
	movl	220(%esp,%eax,4), %eax
	cmpl	%eax, %ecx
	jle	.L10
	movl	396(%esp), %eax
	movl	$-1, 340(%esp,%eax,4)
	movl	$0, 384(%esp)
	movl	380(%esp), %eax
	movl	392(%esp), %edx
	movl	%edx, 340(%esp,%eax,4)
	movl	392(%esp), %eax
	movl	380(%esp), %edx
	movl	%edx, 360(%esp,%eax,4)
	jmp	.L8
.L10:
	movl	380(%esp), %eax
	movl	320(%esp,%eax,4), %eax
	leal	1(%eax), %edx
	movl	380(%esp), %eax
	movl	%edx, 320(%esp,%eax,4)
	movl	380(%esp), %eax
	movl	320(%esp,%eax,4), %eax
	movl	%eax, 388(%esp)
.L8:
	movl	380(%esp), %eax
	movl	340(%esp,%eax,4), %eax
	cmpl	$-1, %eax
	je	.L11
.L7:
	addl	$1, 384(%esp)
.L6:
	cmpl	$4, 384(%esp)
	jle	.L12
	movl	$.LC2, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$.LC3, (%esp)
	call	puts
	movl	$0, 380(%esp)
	jmp	.L13
.L14:
	movl	380(%esp), %eax
	movl	340(%esp,%eax,4), %edx
	movl	$.LC4, %eax
	movl	%edx, 8(%esp)
	movl	380(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, 380(%esp)
.L13:
	cmpl	$4, 380(%esp)
	jle	.L14
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
