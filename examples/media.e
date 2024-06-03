<<
Este programa calcula la media de una secuencia de numeros
dada por el usuario, el usuario insertara los numeros hasta
que se introduzca un numero <= a 0
>>

sum := 0;
cont := 0;

hacer
    escribir('Introduce un numero: ');
    leer(dato);
    si (dato > 0) entonces
        sum +:= dato;
        cont++;
    fin_si;
mientras (dato > 0);

si (cont > 0) entonces
    media := sum / cont;
    escribir('La media es: ');
    escribir(media);
fin_si;

escribir('\n');
