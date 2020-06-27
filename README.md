# proyecto3
Aventuras en Threadville

#### Descripción
En este proyecto se considerara un manejo no trivial de threads y
concurrencia. Toda la programacion debe realizarse en C sobre Linux,
utilizando la biblioteca Pthreads para el manejo de hilos de ejecucion.

####  Diseño General
En Threadville hay recursos que deben ser compartidos armoniosamente por todos los vehículos. A fin de cuentas, su trabajo es
escribir el Sistema Operativo de esta ciudad, el cual fundamentamente
proporciona protección , dejando la administracion a los procesos.
Estos procesos debieran ser escritos de la manera mas genérica posible: un automóvil, una ambulancia, un bus... todos son equivalentes
(e.g., ¿bastara con un único tipo de thread que sabe manejar un veh´ıculo arbitrario?).

## Installation

Please run the commands to install the library [SDL](https://www.libsdl.org/).

```bash
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
```

## Run and execute the project

```python
make
cd build
./proyecto3
```

## Contributing
- Daniel Alvarado Chou.
- Greylin Arias Montiel.
- Jorge Bolaños Solís.
- Alonso Mondal Durán.
- Kenneth Paniagua Díaz.

