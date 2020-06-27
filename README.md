# Proyecto 3: Aventuras en Threadville

#### Descripción
En este proyecto se considera un manejo no trivial de threads y
concurrencia. Toda la programacion está realizada en C sobre Linux,
utilizando la biblioteca Pthreads para el manejo de hilos de ejecucion.


####  Diseño General
En Threadville hay recursos que deben ser compartidos armoniosamente por todos los vehículos. A fin de cuentas, su trabajo es
escribir el Sistema Operativo de esta ciudad, el cual fundamentamente
proporciona protección, dejando la administracion a los procesos.
Estos procesos debieran ser escritos de la manera mas genérica posible: un automóvil, una ambulancia, un bus... todos son equivalentes
(e.g., ¿bastará con un único tipo de thread que sabe manejar un vehículo arbitrario?).


## Estructura y compilación del proyecto
- include: Contiene los archivos .h de la interfaz, la lógica y utils.
- src: Contiene los archivos .c  de la interfaz, la lógica y utils.
- build: contiene los archivos ejecutables generados por el makefile. Este directorio se crea con la regla `all` del makefile.


### Instalación
Para correrlo hay que instalar previamente la biblioteca [SDL](https://www.libsdl.org/):

```bash
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
```

## Correr y ejecutar el proyecto
 Para ejecutar el binario es necesario compilar el proyecto, entrar al directorio de build, por las referencias de las imágenes y ejecutar el proyecto:

```
make
cd build
./proyecto3
```

## Faltó por implementar
- Interacción con el usuario.
- Información en pantalla de número de viajes de los carros.
- Prioridad de paso de ambulancias por las intersecciones.
- Protección en las rotondas.
- Las intersecciones pueden quedar bloqueadas.
- Larry y Joe funcionan por defecto con k = 1.

## Autores
- Daniel Alvarado Chou.
- Greylin Arias Montiel.
- Jorge Bolaños Solís.
- Alonso Mondal Durán.
- Kenneth Paniagua Díaz.

Este proyecto se puede encontrar en https://github.com/soatec/proyecto3
