<<
Dada una secuencia de numeros, si un numero es divisible por 3, se imprime 'Fizz'
si es divisible por 5, se imprime 'Buzz', si es divisible por ambos, escribir 'FizzBuzz'
>>

escribir('Escribe un numero: ');
leer(n);

para i desde 1 hasta n hacer
    output := '';

    si (i % 3 = 0) entonces
        output := output || 'Fizz';
    fin_si;

    si (i % 5 = 0) entonces
        output := output || 'Buzz';
    fin_si;

    si (output = '') entonces
        escribir(i);
    fin_si;

    escribir(output || '\n');
fin_para;
