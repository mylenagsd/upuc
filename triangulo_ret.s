.global main

main:
    push {r0-r4, lr}

    /* INICIO */
    ldr     r0, =msgI
    bl      printf

    /* SOLICITA FLOAT */
    ldr     r0, =msg_x
    bl      printf

    /* LÊ FLOAT */
    ldr     r0, =scan_x
    ldr     r1, =x
    bl      scanf

    /* GUARDA FLOAT EM S16 E CONVERTE PARA DOUBLE EM D8 */
    ldr     r1, =x
    vldr    s16, [r1]
    vcvt.f64.f32    d8, s16

    /* SOLICITA FLOAT */
    ldr     r0, =msg_y
    bl      printf

    /* LÊ FLOAT */
    ldr     r0, =scan_y
    ldr     r1, =y
    bl      scanf

    /* GUARDA FLOAT EM S18 E CONVERTE PARA DOUBLE EM D9 */
    ldr     r1, =y
    vldr    s18, [r1]
    vcvt.f64.f32    d9, s18

    /* CALCULO DA HIPOTENUSA */
    vmul.f64    d8, d8, d8  /* ca^2 */
    vmul.f64    d9, d9, d9  /* co^2 */
    vadd.f64    d10, d9, d8 /* co^2 + ca^2 */
    vsqrt.f64   d10, d10    /* (co^2 + ca^2)^1/2 = H */

    /* PRINTA RESULTADO guardado em r2 e r3 = d10 = H */
    ldr     r0, =msgH
    vmov    r2, r3, d10
    bl      printf
    
    /* CALCULO DO SENO */
    vsqrt.f64   d9, d9       /* (co^2)^1/2 = co */
    vdiv.f64    d11, d9, d10 /* co/H */

    /* PRINTA RESULTADO guardado em r2 e r3 = d11 = Seno */
    ldr     r0, =msgS
    vmov    r2, r3, d11
    bl      printf

    /* CALCULO DO COSSENO */
    vsqrt.f64   d8, d8       /* (ca^2)^1/2 = ca */
    vdiv.f64    d12, d8, d10 /* ca/H */

    /* PRINTA RESULTADO guardado em r2 e r3 = d12 = Cosseno */
    ldr     r0, =msgC
    vmov    r2, r3, d12
    bl printf

    /* CALCULO DA TANGENTE */
    vdiv.f64   d13, d9, d8  /* co/ca */

    /* PRINTA RESULTADO guardado em r2 e r3 = d13 = Tangente */
    ldr     r0, =msgT
    vmov    r2, r3, d13
    bl printf
    
    pop {r0-r4, pc}

        .data

x:      .float  0.0
y:      .float  0.0
msgI:   .string "Calculadora: Triangulo Retangulo \n"
msg_x:  .string "Digite o valor float do cateto adjacente: " 
msg_y:  .string "Digite o valor float do cateto oposto: " 
scan_x: .string "%f" 
scan_y: .string "%f" 

msgH:   .string "Hipotenusa = %f \n"
msgS:   .string "Seno = %f \n"
msgC:   .string "Cosseno = %f \n"
msgT:   .string "Tangente = %f \n"