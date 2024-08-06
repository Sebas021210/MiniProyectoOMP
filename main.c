#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <string.h>

#define GRID_SIZE 20
#define PLANT 'P'
#define HERBIVORE 'H'
#define CARNIVORE 'C'
#define EMPTY 'X'

#define INITIAL_PLANTS 250
#define INITIAL_HERBIVORES 100
#define INITIAL_CARNIVORES 30

typedef struct {
    char type;
    int energy;
    int moved;  // Nueva variable para controlar si la celda ya se movió en este tick
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];

void initialize_grid() {
    // Inicializa la cuadrícula vacía
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].type = EMPTY;
            grid[i][j].energy = 0;
            grid[i][j].moved = 0;
        }
    }

    srand(time(NULL));

    // Inicializa plantas
    for (int k = 0; k < INITIAL_PLANTS; k++) {
        int i, j;
        do {
            i = rand() % GRID_SIZE;
            j = rand() % GRID_SIZE;
        } while (grid[i][j].type != EMPTY);
        grid[i][j].type = PLANT;
        grid[i][j].energy = 5; // Energía inicial de la planta
    }

    // Inicializa herbívoros
    for (int k = 0; k < INITIAL_HERBIVORES; k++) {
        int i, j;
        do {
            i = rand() % GRID_SIZE;
            j = rand() % GRID_SIZE;
        } while (grid[i][j].type != EMPTY);
        grid[i][j].type = HERBIVORE;
        grid[i][j].energy = 10; // Energía inicial del herbívoro
    }

    // Inicializa carnívoros
    for (int k = 0; k < INITIAL_CARNIVORES; k++) {
        int i, j;
        do {
            i = rand() % GRID_SIZE;
            j = rand() % GRID_SIZE;
        } while (grid[i][j].type != EMPTY);
        grid[i][j].type = CARNIVORE;
        grid[i][j].energy = 15; // Energía inicial del carnívoro
    }
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
    if (grid[i][j].moved) return;

    grid[i][j].energy++;
    int reproduce = rand() % 100 < 70;
    if (reproduce && grid[i][j].energy > 17) {
        int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        if (grid[ni][nj].type == EMPTY) {
            grid[ni][nj].type = PLANT;
            grid[ni][nj].energy = 5;
            grid[ni][nj].moved = 1;
            grid[i][j].energy -= 5;
        }
    }
    grid[i][j].moved = 1;
}

void update_herbivore(int i, int j) {
    if (grid[i][j].moved) return;

    grid[i][j].energy--;
    if (grid[i][j].energy <= 0) {
        grid[i][j].type = EMPTY;
        return;
    }

    int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;

    if (grid[ni][nj].type == PLANT && !grid[ni][nj].moved) {
        grid[i][j].energy += grid[ni][nj].energy;
        grid[ni][nj].type = HERBIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[ni][nj].moved = 1;
        grid[i][j].type = EMPTY;
    } else if (grid[ni][nj].type == EMPTY) {
        grid[ni][nj].type = HERBIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[ni][nj].moved = 1;
        grid[i][j].type = EMPTY;
    } else {
        grid[i][j].moved = 1;
    }

    // Reproducción
    if (grid[i][j].energy > 1) {
        int reproduce_i = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        int reproduce_j = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        if (grid[reproduce_i][reproduce_j].type == EMPTY) {
            grid[reproduce_i][reproduce_j].type = HERBIVORE;
            grid[reproduce_i][reproduce_j].energy = 10;
            grid[reproduce_i][reproduce_j].moved = 1;
            grid[i][j].energy -= 10;
        }
    }
}

void update_carnivore(int i, int j) {
    if (grid[i][j].moved) return;

    grid[i][j].energy--;
    if (grid[i][j].energy <= 0) {
        grid[i][j].type = EMPTY;
        return;
    }

    int ni = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
    int nj = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;

    if (grid[ni][nj].type == HERBIVORE && !grid[ni][nj].moved) {
        grid[i][j].energy += grid[ni][nj].energy;
        grid[ni][nj].type = CARNIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[ni][nj].moved = 1;
        grid[i][j].type = EMPTY;
    } else if (grid[ni][nj].type == EMPTY) {
        grid[ni][nj].type = CARNIVORE;
        grid[ni][nj].energy = grid[i][j].energy;
        grid[ni][nj].moved = 1;
        grid[i][j].type = EMPTY;
    } else {
        grid[i][j].moved = 1;
    }

    // Reproducción
    if (grid[i][j].energy > 20) {
        int reproduce_i = (i + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        int reproduce_j = (j + (rand() % 3) - 1 + GRID_SIZE) % GRID_SIZE;
        if (grid[reproduce_i][reproduce_j].type == EMPTY) {
            grid[reproduce_i][reproduce_j].type = CARNIVORE;
            grid[reproduce_i][reproduce_j].energy = 15;
            grid[reproduce_i][reproduce_j].moved = 1;
            grid[i][j].energy -= 15;
        }
    }
}

void simulate_tick() {
    // Resetear el estado 'moved' de todas las celdas
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].moved = 0;
        }
    }

    // Actualizar la cuadrícula
    #pragma omp parallel for collapse(2)
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
