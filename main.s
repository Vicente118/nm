bits 64

section .data
	msg db "Hello world !", 10
	len equ $ -(msg + 1) 

section .text

global _start

_start:
	jmp .hello_label

.hello_label:
	mov rax, 1
	mov rdi, 1
	mov rsi, msg 
	mov rdx, len
	syscall

	mov rax, 60
	mov rdi, 0
	syscall
