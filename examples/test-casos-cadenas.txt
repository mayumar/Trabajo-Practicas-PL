leer_cadena(cadena);

casos(cadena)
    valor 'mayte': escribir('Hola Mayte');
    valor 'paco': escribir('Hola Paco');
    valor 'pedro': escribir('Hola Pedro');
    defecto: escribir('No se quien eres');
fin_casos;

leer_cadena(cadena);

casos(cadena || ' alumno')
    valor 'mayte alumno': escribir('Hola Mayte');
    valor 'paco alumno': escribir('Hola Paco');
    valor 'pedro alumno': escribir('Hola Pedro');
    defecto: escribir('No se quien eres');
fin_casos;