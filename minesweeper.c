#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 10

typedef struct {
    int guesses[MAP_SIZE][MAP_SIZE];
    int mines[MAP_SIZE][MAP_SIZE];
} MineSweeperMap;

void clearMapMemory(MineSweeperMap *m);
void generateMines(int minemap[MAP_SIZE][MAP_SIZE], int n);
void displayMap(MineSweeperMap *m);
int getNeighbours(int minemap[MAP_SIZE][MAP_SIZE], int x, int y);
int checkGuess(MineSweeperMap *m, int x, int y);

int main() {
    MineSweeperMap ms;
    int nOfMines = 0;
    while (nOfMines < 3 || nOfMines > 30) {
        scanf("%d", &nOfMines);
    }
    clearMapMemory(&ms);
    generateMines(ms.mines, nOfMines);
    displayMap(&ms);
    int gameOver = 0;
    int won = 0;
    int guesses = 0;
    while (!gameOver && !won) {
        char guess[100];
        scanf("%s", guess);
        int guessX = (int)guess[0] - 65;
        char guessYtemp[2] = { guess[1], '\0' };
        int guessY = atoi(guessYtemp);
        if (guessY < 0 || guessY > 9 || guessX < 0 || guessX > 9) {
            printf("Invalid input!");
        }
        else {
            gameOver = checkGuess(&ms, guessX, guessY);
            guesses++;
            won = guesses == (MAP_SIZE * MAP_SIZE) - nOfMines;
            displayMap(&ms);
        }
    }
    if (gameOver) printf("Game over!\n");
    if (!gameOver && won) printf("You won!");
    return 0;
}

void clearMapMemory(MineSweeperMap *m) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            m->guesses[i][j] = 0;
            m->mines[i][j] = 0;
        }
    }
}

void generateMines(int minemap[MAP_SIZE][MAP_SIZE], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int x = rand() % MAP_SIZE;
        int y = rand() % MAP_SIZE;
        if (minemap[x][y] != 1) {
            minemap[x][y] = 1;
        }
        else {
            i--;
        }
    }
}

void displayMap(MineSweeperMap *m) {
    for (int i = -1; i < MAP_SIZE; i++) {
        if (i == -1) {
            printf("  ");
            for (int j = 0; j < MAP_SIZE; j++) {
                printf("%d ", j);
            }
            printf("\n");
        }
        else {
            for (int j = -1; j < MAP_SIZE; j++) {
                if (j == -1) {
                    printf("%c ", 65 + i);
                }
                else {
                    if (m->guesses[i][j] == 1) {
                        if (m->guesses[i][j] == m->mines[i][j]) {
                            printf("* ");
                        }
                        else {
                            int res = getNeighbours(m->mines, i, j);
                            printf("%d ", res);
                        }
                    }
                    else {
                        printf("x ");
                    }
                }
            }
            printf("\n");
        }
    }
}

int checkGuess(MineSweeperMap *m, int x, int y) {
    m->guesses[x][y] = 1;
    if (m->mines[x][y] == 1) return 1;
    return 0;
}

//Ezt maybe lehetne rövidebben?
int getNeighbours(int minemap[MAP_SIZE][MAP_SIZE], int x, int y) {
    int db = 0;
    if (x > 0 && x < MAP_SIZE - 1) {
        if (y > 0 && y < MAP_SIZE - 1) {
            db += minemap[x + 1][y];
            db += minemap[x - 1][y];
            db += minemap[x][y + 1];
            db += minemap[x][y - 1];
        }
        else {
            if (y == 0) {
                db += minemap[x + 1][y];
                db += minemap[x - 1][y];
                db += minemap[x][y + 1];
            }
            else {
                db += minemap[x + 1][y];
                db += minemap[x - 1][y];
                db += minemap[x][y - 1];
            }
        }
    }
    else {
        if (x == 0) {
            if (y > 0 && y < MAP_SIZE - 1) {
                db += minemap[x + 1][y];
                db += minemap[x][y + 1];
                db += minemap[x][y - 1];
            }
            else {
                if (y == 0) {
                    db += minemap[x + 1][y];
                    db += minemap[x][y + 1];
                }
                else {
                    db += minemap[x + 1][y];
                    db += minemap[x][y - 1];
                }
            }
        }
        else {
            if (y > 0 && y < MAP_SIZE - 1) {
                db += minemap[x - 1][y];
                db += minemap[x][y + 1];
                db += minemap[x][y - 1];
            }
            else {
                if (y == 0) {
                    db += minemap[x - 1][y];
                    db += minemap[x][y + 1];
                }
                else {
                    db += minemap[x - 1][y];
                    db += minemap[x][y - 1];
                }
            }
        }
    }
    return db;
}