!! escribir('Indique el inicio del bucle: ');
!! leer(init);
escribir('Indique el final del bucle: ');
leer(N);

factorial := 1;

para i desde 2 hasta N paso 1 hacer
    factorial := factorial * i;
fin_para;

escribir(factorial);
escribir('\n');
