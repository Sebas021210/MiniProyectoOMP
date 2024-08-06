#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define GRID_SIZE 10
#define PLANT 'P'
#define HERBIVORE 'H'
#define CARNIVORE 'C'
#define EMPTY 'X'

typedef struct {
    char type;
    int energy;
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];

void initialize_grid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].type = EMPTY;
            grid[i][j].energy = 0;
        }
    }

    grid[0][0].type = PLANT;
    grid[1][1].type = HERBIVORE;
    grid[2][2].type = CARNIVORE;
}

void print_grid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%c ", grid[i][j].type);
        }
        printf("\n");
    }
}

void update_plant(int i, int j) {
    // Ejemplo de reproducción de planta
    int reproduce = rand() % 100 < 30; // 30% de probabilidad de reproducción
    if (reproduce) {
        int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        if (grid[ni][nj].type == EMPTY) {
            grid[ni][nj].type = PLANT;
        }
    }
}

void update_herbivore(int i, int j) {
    // Ejemplo de movimiento y consumo de plantas por herbívoro
    int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    if (grid[ni][nj].type == PLANT) {
        grid[ni][nj].type = HERBIVORE;
        grid[ni][nj].energy = grid[i][j].energy + 1;
        grid[i][j].type = EMPTY;
    } else if (grid[ni][nj].type == EMPTY) {
        grid[ni][nj].type = HERBIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[i][j].type = EMPTY;
    }
}

void update_carnivore(int i, int j) {
    // Ejemplo de movimiento y consumo de herbívoros por carnívoro
    int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    if (grid[ni][nj].type == HERBIVORE) {
        grid[ni][nj].type = CARNIVORE;
        grid[ni][nj].energy = grid[i][j].energy + 2;
        grid[i][j].type = EMPTY;
    } else if (grid[ni][nj].type == EMPTY) {
        grid[ni][nj].type = CARNIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[i][j].type = EMPTY;
    }
}

void simulate_tick() {
#pragma omp parallel for
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].type == PLANT) {
                update_plant(i, j);
            } else if (grid[i][j].type == HERBIVORE) {
                update_herbivore(i, j);
            } else if (grid[i][j].type == CARNIVORE) {
                update_carnivore(i, j);
            }
        }
    }
}

void count_species(int *num_plants, int *num_herbivores, int *num_carnivores) {
    *num_plants = 0;
    *num_herbivores = 0;
    *num_carnivores = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].type == PLANT) {
                (*num_plants)++;
            } else if (grid[i][j].type == HERBIVORE) {
                (*num_herbivores)++;
            } else if (grid[i][j].type == CARNIVORE) {
                (*num_carnivores)++;
            }
        }
    }
}

int main() {
    initialize_grid();
    print_grid();

    int num_ticks = 1500;
    for (int tick = 0; tick < num_ticks; tick++) {
        simulate_tick();

        if ((tick + 1) % 100 == 0) {
            int num_plants, num_herbivores, num_carnivores;
            count_species(&num_plants, &num_herbivores, &num_carnivores);

            printf("\nTick %d\n", tick + 1);
            printf("Plantas: %d\n", num_plants);
            printf("Herbivoros: %d\n", num_herbivores);
            printf("Carnivoros: %d\n", num_carnivores);
            printf("Distribucion:\n");
            print_grid();
        }
    }

    return 0;
}
