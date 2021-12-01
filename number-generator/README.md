# Generadores de Números

Considere un sistema con dos procesos generadores de
números enteros, dos procesos que escriben estos valores en los archivos (Salida1 y
Salida2), un proceso de control y un proceso de sincronización. 

El proceso de sincronización 
recibe los números enteros de los procesos generadores y los envía a los
procesos escritores, dependiendo del estado de una variable de control. 

Incialmente, se
envían los datos a Salida1. Cada vez que se recibe un mensaje del proceso de control
el proceso de sincronización cambia el proceso escritor al que se envían los datos.