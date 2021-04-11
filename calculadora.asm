global main
extern printf, scanf

section     .text
main:
    ;INICIAR
    mov     rsi,msg0
    mov     rdx,lmsg0
    mov     rdi,1
    mov     rax,1
    syscall

opcoes:
	;OPÇÕES DISPONÍVEIS DE OPERAÇÃO
	mov     rsi,msgO
    mov     rdx,lmsgO
    mov     rdi,1
    mov     rax,1
    syscall

    ;SOLICITA TIPO DE OPERAÇÃO
    mov     rsi,msg3
    mov     rdx,lmsg3
    mov     rdi,1
    mov     rax,1
    syscall

    ;INPUT TIPO
	mov	   	rsi, w0
    mov     rdx, 1 
    mov 	rdi, 0                                          
	mov     rax, 0                                                  
	syscall
    
    ;SOLICITA PRIMEIRO NUMERO
    mov     rsi,msg1
    mov     rdx,lmsg1
    mov     rdi,1
    mov     rax,1
    syscall

    ;INPUT X
    sub     rsp, 8
    mov 	rax, 0                                          
	mov     rdi, op                      
	mov	   	rsi, x0                                    
	call    scanf  
    add     rsp, 8

    ;SOLICITA SEGUNDO NUMERO
    mov     rsi,msg2
    mov     rdx,lmsg2
    mov     rdi,1
    mov     rax,1
    syscall

    ;INPUT Y
    sub     rsp, 8
    mov 	rax, 0                                          
	mov     rdi, op                      
	mov	   	rsi, y0                                    
	call    scanf  
    add     rsp, 8   

    ;OPERAÇÃO ESCOLHIDA
comparacao:
    mov rax, [w0]
    sub rax, '0'  ;Converte de ascii para decimal
	
	cmp rax, 1
    jz adicao
	cmp rax, 2
    jz subtracao
	cmp rax, 3
    jz multiplicacao
    cmp rax, 4
    jz divisao
	cmp rax, 5
    jz raiz

adicao:
    ;ATRIBUIÇÃO DOS PARAMENTROS
    sub     rsp, 8 ; alinhar 16 bits
    movsd   xmm2, qword [y0] ; 3º parâmetro - printf
    movsd   xmm1, qword [x0] ; 2º parâmetro - printf
    ;ADIÇÃO
    xorps   xmm0, xmm0 ; zera xmm0
    addsd   xmm0, xmm1 ; xmm0 recebe 0 + xmm1
    addsd   xmm0, xmm2 ; xmm0 recebe xmm0 + xmm2:
    ;PRINT DO RESULTADO
    mov     rax, 3 ; regs XMM para printf
    mov     rdi, msgP0 ; 1º parâmetro - printf
    call    printf
    add     rsp, 8 ; pré-alinhamento

	jmp end

subtracao:
    ;ATRIBUIÇÃO DOS PARAMENTROS
    sub     rsp, 8 ; alinhar 16 bits
    movsd   xmm2, qword [y0] ; 3º parâmetro - printf
    movsd   xmm1, qword [x0] ; 2º parâmetro - printf
    ;SUBTRAÇÃO
    xorps   xmm0, xmm0 ; zera xmm0
    addsd   xmm0, xmm1 ; xmm0 recebe 0 + xmm1
    subsd   xmm0, xmm2 ; xmm0 recebe xmm0 + xmm2:
    ;PRINT DO RESULTADO
    mov     rax, 3 ; regs XMM para printf
    mov     rdi, msgP1 ; 1º parâmetro - printf
    call    printf
    add     rsp, 8 ; pré-alinhamento

	jmp end

multiplicacao:
    ;ATRIBUIÇÃO DOS PARAMENTROS
    sub     rsp, 8 ; alinhar 16 bits
    movsd   xmm2, qword [y0] ; 3º parâmetro - printf
    movsd   xmm1, qword [x0] ; 2º parâmetro - printf
    ;MULTIPLICAÇÃO
    xorps   xmm0, xmm0 ; zera xmm0
    addsd   xmm0, xmm1 ; xmm0 recebe 0 + xmm1
    mulsd   xmm0, xmm2 ; xmm0 recebe xmm0 + xmm2:
    ;PRINT DO RESULTADO
    mov     rax, 3 ; regs XMM para printf
    mov     rdi, msgP2 ; 1º parâmetro - printf
    call    printf
    add     rsp, 8 ; pré-alinhamento

	jmp end

divisao:
    ;ATRIBUIÇÃO DOS PARAMENTROS
    sub     rsp, 8 ; alinhar 16 bits
    movsd   xmm2, qword [y0] ; 3º parâmetro - printf
    movsd   xmm1, qword [x0] ; 2º parâmetro - printf
    ;DIVISÃO
    xorps   xmm0, xmm0 ; zera xmm0
    addsd   xmm0, xmm1 ; xmm0 recebe 0 + xmm1
    divsd   xmm0, xmm2 ; xmm0 recebe xmm0 + xmm2:
    ;PRINT DO RESULTADO
    mov     rax, 3 ; regs XMM para printf
    mov     rdi, msgP3 ; 1º parâmetro - printf
    call    printf
    add     rsp, 8 ; pré-alinhamento

    jmp end

raiz:
    ;ATRIBUIÇÃO DOS PARAMENTROS
    sub     rsp, 8 ; alinhar 16 bits
    movsd   xmm1, qword [x0] ; 2º parâmetro - printf
    ;RAIZ QUADRADA
    xorps   xmm0, xmm0 ; zera xmm0
    sqrtsd  xmm0, xmm1 ; xmm0 recebe 0 + xmm1
    ;PRINT DO RESULTADO
    mov     rax, 2 ; regs XMM para printf
    mov     rdi, msgP4 ; 1º parâmetro - printf
    call    printf  
    add     rsp, 8 ; pré-alinhamento

    jmp end

end:;RETURN 0
    xor     rdi,rdi ; zera rdi
    mov     rax,60  ;EXIT
    syscall

section     .data
msg0:       db  "CALCULADORA: ", 10, 0
lmsg0:      equ $-msg0 
msg1:       db  "Insira o primeiro numero: ", 0
lmsg1:      equ $-msg1 
msg2:       db  "Insira o segundo numero: ", 0
lmsg2:      equ $-msg2 
msg3:       db  "Insira o tipo de operaçao: ", 0
lmsg3:      equ $-msg3 
msgO:		db  "1 - Adição", 10, "2 - subtração", 10, "3 - Multiplicação", 10, "4 - Divisão", 10, "5 - Raiz Quadrada", 10, 0
lmsgO:		equ $-msgO 
msgP0:      db  "Adic: %f = %f + %f", 10, 0
msgP1:      db  "Subi: %f = %f - %f", 10, 0
msgP2:      db  "Mult: %f = %f * %f", 10, 0
msgP3:      db  "Divi: %f = %f / %f", 10, 0
msgP4:      db  "Raiz: %f = %f^1/2", 10, 0
msgS:       db  "%f, %f", 0
op          db  "%lf", 0

section     .bss
        align 16
x0:         resq  1   
y0:         resq  1
w0:         resb  1
