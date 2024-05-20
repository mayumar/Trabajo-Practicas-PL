ESCRIBIR('Introduzca un numero --> ');
LEER(dato);

HACER
    ESCRIBIR(dato);
    ESCRIBIR('\n');
    dato := dato - 1;
MIENTRAS (dato <> 0);