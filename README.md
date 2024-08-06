# MiniProyectoOMP
Este proyecto tiene como objetivo desarrollar una simulación de un ecosistema utilizando OpenMP para paralelizar operaciones. El ecosistema incluirá diferentes especies con comportamientos específicos. Se deberán implementar la simulación y demostrar cómo se comportan las especies en un ambiente compartido.

# Ecosistema Simulado con OpenMP

Este proyecto es una simulación de un ecosistema utilizando paralelismo con OpenMP. En este ecosistema, interactúan plantas, herbívoros y carnívoros dentro de una cuadrícula de 20x20 celdas. Cada tipo de entidad tiene reglas específicas para moverse, reproducirse y consumir recursos.

## Características

- **Plantas**: Crecen y se reproducen.
- **Herbívoros**: Consumen plantas y pueden reproducirse.
- **Carnívoros**: Consumen herbívoros y pueden reproducirse.
- **Paralelismo**: La simulación de cada tipo de entidad se ejecuta en paralelo para mejorar el rendimiento.

## Requisitos

- Compilador C compatible con OpenMP.
- Sistema operativo compatible con OpenMP.

## Instalación

1. Clona este repositorio:
    ```bash
    git clone https://github.com/tu_usuario/ecosistema_simulado.git
    cd ecosistema_simulado
    ```

2. Compila el código utilizando un compilador compatible con OpenMP:
    ```bash
    gcc -fopenmp -o ecosistema ecosistema.c
    ```

## Uso

Ejecuta el programa compilado:
```bash
./ecosistema
```

El programa imprimirá la distribución inicial del ecosistema y luego ejecutará 1500 ticks (pasos de tiempo). Cada 100 ticks, se imprimirá el estado actual del ecosistema y el conteo de cada tipo de entidad.

## Codigo Principal

El código principal del programa está en ecosistema.c. Aquí hay una breve descripción de sus funciones principales:

- **initialize_grid()**: Inicializa la cuadrícula con plantas, herbívoros y carnívoros en posiciones aleatorias.
- **print_grid()**: Imprime la cuadrícula actual.
- **update_plant(int i, int j)**: Actualiza el estado de una planta.
- **update_herbivore(int i, int j)**: Actualiza el estado de un herbívoro.
- **update_carnivore(int i, int j)**: Actualiza el estado de un carnívoro.
- **simulate_tick()**: Simula un tick de tiempo, actualizando todas las entidades en la cuadrícula.
- **count_species(int *num_plants, int *num_herbivores, int *num_carnivores)**: Cuenta la cantidad de plantas, herbívoros y carnívoros en la cuadrícula.


## Creadores

Sebastian Solorzano 21826
Manuel Rodas 21509
Jose Santisteban 21153
