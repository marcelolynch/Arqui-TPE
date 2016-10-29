GLOBAL keyRead

section .text
keyRead:
	xor rax, rax
	in al, 64h
	and al, 1 ;Me quedo con el ultimo bit
	test al, al
	jz .end
	in al, 60h
	.end:
	ret
