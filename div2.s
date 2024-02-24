	.file	"div.c"
	.text
	.globl	div_qr
	.type	div_qr, @function
div_qr:
.LFB0:
	.cfi_startproc
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movl	%edi, %r12d
	movl	%esi, %r13d
	movq	%rdx, %r14
	movl	%r12d, %eax
	cltd
	idivl	%r13d
	movl	%eax, %r15d
	movq	%r14, %rax
	movl	%edx, (%rax)
	movl	%r15d, %eax
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	div_qr, .-div_qr
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-44)"
	.section	.note.GNU-stack,"",@progbits
