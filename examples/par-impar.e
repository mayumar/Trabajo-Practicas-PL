<<
Este programa te dice que numeros
son pares o impares en un rango dado
por el usuario.
>>

escribir('Escribe el numero de inicio: ');
leer(init);

escribir('Escribe el numero final: ');
leer(end);

!! Comprobamos que los limites son positivos
si (init < 0 #o end < 0) entonces
    escribir('Los limites no pueden ser negativos\n');
si_no
    para i desde init hasta end hacer
        si (i >= 0 #y i % 2 = 0) entonces
            escribir(i);
            escribir(' es par\n');
        si_no
            escribir(i);
            escribir(' es impar\n');
        fin_si;
    fin_para;
fin_si;
