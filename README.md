# Job Shop Scheduling Problem - Entrega 1

Ignacio Salas Mateluna
201473508-1

### Instrucciones de ejecución

- Para compilar utilizar make
- Para ejecutar utilizar "./main"

### Consideraciones

- El programa se ejecuta para varias instacias a la vez.

- El programa consdiera las instacias que estan especificadas en la linea 47 del codigo.

- Si se desea agregar una nueva instacia esta se debe agregar en la carpeta "instancias" y en la linea 47 del codigo en el vector instance.

- Los resultados para todas las instancias de la ejecución son entregados en el archivo results.txt. Cada instancia incluye nombre, tiempo y matriz de asignación

- Se implementaron 4 funciones para la resolución de los conflictos: SPT, LPT, SRPT, LRPT. Por defecto el programa utiliza SPT, si se desea modificar se debe comentar la linea 214 y descomentar alguna de las lineas siguiente.

### Instancias:

Las instacias son obtenidas del siguiente enlace:
- http://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/jobshop1.txt

Se siguio el formato utilizado por estas instancias.

### Limites:

- http://optimizizer.com/Extra.php

- http://jobshop.jjvh.nl/
