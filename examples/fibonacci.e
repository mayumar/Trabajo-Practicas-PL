<<
Este algoritmo iterativo muestra los n primeros 
numeros de la sucesion de Fibonacci
>>

escribir('Escribe el numero limite de la sucesion: ');
leer(n);

si (n = 0 #o n = 1) entonces
    escribir(n);
si_no
    a := 0;
    b := 1;
    escribir(a);
    escribir(' ');
    escribir(b);
    escribir(' ');

    n -:= 2;

    mientras (n > 0) hacer
        temp := a + b;
        escribir(temp);
        escribir(' ');
    
        temp := b;
        b +:= a;
        a := temp;
        n--;
    fin_mientras;
fin_si;

escribir('\n');