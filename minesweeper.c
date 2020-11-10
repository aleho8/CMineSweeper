#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
int saveProgress(MineSweeperMap *m, char *name);
int loadProgress(MineSweeperMap *m, char *name);
void getSubstr(char *s1, char *s2, char *str, char sub);

int main(int argc, char *argv[]) {
    MineSweeperMap ms;
    if (argc <= 1) {
        printf("Start parameter must be a number between 1 and 100.\n");
        return 0;
    }
    int nOfMines = atoi(argv[1]);
    if (nOfMines > 100 || nOfMines < 1) {
        printf("Number of mines must be between 1 and 100.\n");
        return 0;
    }
    clearMapMemory(&ms);
    generateMines(ms.mines, nOfMines);
    displayMap(&ms);
    int gameOver = 0;
    int won = 0;
    int guesses = 0;
    while (!gameOver && !won) {
        char guess[100];
        if (gets(guess)) {
            if (strlen(guess) > 2) {
                char *command = (char *)malloc(sizeof(char));
                char *filename = (char *)malloc(sizeof(char));
                getSubstr(command, filename, guess, ' ');
                printf("Parancs: %s\n", command);
                printf("Filename: %s\n", filename);
                if (!strcmp(command, "save")) {
                    saveProgress(&ms, filename);
                }
                if (!strcmp(command, "load")) {
                    loadProgress(&ms, filename);
                }
                free(command);
                free(filename);
            }
            else {
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
    srand(time(0));
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

  /*
  * 0 = semmi, 1 = mine, 2 = tipp
  * 00001200020
  * 01122000000
  *
  *
  */

int saveProgress(MineSweeperMap *m, char *name) {
  FILE *saveFile = fopen(name, "w+b");
  if (!saveFile) {
    printf("Error:  Couldn't open file for write!\n");
    return 1;
  }
  for (int i = 0; i < MAP_SIZE; i++) {
    char line[MAP_SIZE];
    for (int j = 0; j < MAP_SIZE; j++) {
      line[j] = m->mines[i][j] == 1 ? '1' : m->guesses[i][j] == 1 ? '2' : '0';
    }
    size_t rl = fwrite(line, sizeof line[0], MAP_SIZE, saveFile);
    fputc('\n', saveFile);
    if (rl < MAP_SIZE) return 1;
  }
  fclose(saveFile);
  return 0;
}


int loadProgress(MineSweeperMap *m, char *name)
{
  FILE *loadFile = fopen(name, "r");
  int conSize = MAP_SIZE*MAP_SIZE + MAP_SIZE;
  char con[conSize];
  if (loadFile == NULL)
  {
    printf("Error: The file does not exist!\n");
    return 1;
  }
  else
  {
    fgets(con, conSize, loadFile);
    int row = 0;
    int col = 0;
    for (int i = 0; i < con; i++)
    {
      if (con[i] == '\n')
      {
        row++;
        col--;
      }
      else 
      {
        col = i % 10; 
        m->mines[row][col] = con[i];
      }

    }
  }
  fclose(loadFile);
  return 0;
}

void getSubstr(char *s1, char *s2, char *str, char sub) {
    int s1l = 0;
    int s2l = 0;
    int foundSub = 0;
    for (unsigned int i = 0; i < strlen(str); i++) {
        if (!foundSub && str[i] != sub) {
            s1[s1l] = str[i];
            s1l++;
            s1 = (char *)realloc(s1, sizeof(char) * (1 + s1l));
        }
        if(foundSub) {
            s2[s2l] = str[i];
            s2l++;
            s2 = (char *)realloc(s2, sizeof(char) * (1 + s2l));
        }
        if (!foundSub) foundSub = str[i] == sub;
    }
    s1[s1l] = '\0';
}