<<
Este programa descompone un numero n dado por el usuario
en sus factores primos.
>>

escribir('Escribe un numero: ');
leer(n);

si (n < 0) entonces
    escribir('El numero debe ser un entero positivo');
si_no
    !! Dividimos n por 2 hasta que sea impar
    mientras (n % 2 = 0) hacer
        escribir(2);
        escribir(' ');
        n := n // 2;
    fin_mientras;

    !! Dividir por todos los números impares desde 3 hasta la raíz cuadrada de n
    i := 3;

    mientras (i*i <= n) hacer
        mientras (n % i = 0) hacer
            escribir(i);
            escribir(' ');
            n := n // i;
        fin_mientras;
        i +:= 2;
    fin_mientras;

    !! Si n es un número primo mayor que 2, imprimirlo
    si (n > 2) entonces
        escribir(n);
        escribir(' ');
    fin_si;
fin_si;

escribir('\n');
