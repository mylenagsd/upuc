    .global main
    .func   main

main:
    push {r0-r4, lr}

    /* INICIO */
    ldr     r0, =msgI
    bl      printf

    /* SOLICITA INTEIRO */
    ldr     r0, =msg_x
    bl      printf

    /* LÊ INTEIRO */
    ldr     r0, =scan_x
    ldr     r1, =x
    bl      scanf

    mov     r2, r1

    /* SOLICITA INTEIRO */
    ldr     r0, =msg_y
    bl      printf

    /* LÊ INTEIRO */
    ldr     r0, =scan_y
    ldr     r1, =y
    bl      scanf

    /* SOMA OS DOIS INTEIROS */
    ldr     r0, =msgA
    ldr     r1, =x         
    ldr     r1, [r1]
    ldr     r2, =y
    ldr     r2, [r2]
    add     r1, r1, r2
    /* PRINTA RESULTADO guardado em r1 */
    bl      printf

    /* SUBTRAI OS DOIS INTEIROS */
    ldr     r0, =msgS
    ldr     r1, =x         
    ldr     r1, [r1]
    ldr     r2, =y
    ldr     r2, [r2]
    sub     r1, r1, r2
    /* PRINTA RESULTADO guardado em r1 */
    bl      printf

    /* MULTIPLICA OS DOIS INTEIROS */
    ldr     r0, =msgM
    ldr     r2, =x         
    ldr     r2, [r2]
    ldr     r3, =y
    ldr     r3, [r3]
    mul     r1, r2, r3
    /* PRINTA RESULTADO guardado em r1 */
    bl      printf

    /* DIVIDE OS DOIS INTEIROS */
    ldr     r0, =msgD
    ldr     r2, =x         
    ldr     r2, [r2]
    ldr     r3, =y
    ldr     r3, [r3]
    udiv    r1, r2, r3
    mov     r5, r1
    /* PRINTA RESULTADO guardado em r1 */
    bl      printf    

    /* CALCULA O RESTO DA DIVISÃO PRECENDENTE */
    ldr     r0, =msgDr
    ldr     r2, =x         
    ldr     r2, [r2]
    ldr     r3, =y
    ldr     r3, [r3]
    mul     r1, r5, r3
    sub     r1, r2, r1
    /* PRINTA RESULTADO guardado em r1 */
    bl      printf

    pop {r0-r4, pc}

    mov     r0, #0          /* status -> 0 */
    mov     r7, #1          /* #1 - EXIT */
    swi     #0              /* CHAMA SYSCALL */

    .data
msgI:   .string "Calculadora de inteiros \n"
msg_x:  .string "Digite o primeiro valor inteiro:  " 
scan_x: .string "%d" 
x:      .int 0
msg_y:  .string "Digite o segundo valor inteiro:  " 
scan_y: .string "%d" 
y:      .int 0

msgA:   .string "Soma = %d \n"
msgS:   .string "Subtração = %d \n"
msgM:   .string "Multiplicação = %ld \n"
msgD:   .string "Divisão: quociente = %d "
msgDr:  .string " resto = %d \n"
