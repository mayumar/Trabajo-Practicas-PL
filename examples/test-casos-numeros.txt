leer(c);

casos(c)
    valor 1: escribir('case 1');
    valor 2: escribir('case 2');
    valor 3: escribir('case 3');
    defecto: escribir('hola');
fin_casos;

leer(c);

casos(c - 1)
    valor 0: escribir('case 0');
    valor 1: escribir('case 1');
    valor 2: escribir('case 2');
    defecto: escribir('hola');
fin_casos;