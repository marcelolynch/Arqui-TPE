GLOBAL sys_write
GLOBAL sys_read

section .text

sys_write:
	mov rbx, rdi ;STDOUT
	mov rcx, rsi ;string
	mov rdx, rdx ;size

	mov rax, 4 ;Write
	int 80h

	ret

;sys_read(fd, buffer, size)
sys_read:
	mov rbx, rdi ;STDIN
	mov rcx, rsi ;buffer
	mov rdx, rdx ;size

	mov rax, 3 ;Read
	int 80h
	
	ret
