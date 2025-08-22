#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Colores ANSI
#define RESET    "\x1b[0m"
#define PURPLEBG "\x1b[45m"   // Morado claro
#define WHITEBG  "\x1b[47m"   // Blanco
#define WHITE    "\x1b[97m"   // Texto blanco brillante
#define BLACK    "\x1b[30m"   // Texto negro

#define SIZE 8

char board[SIZE][SIZE] = {
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

void printBoard() {
    printf("\n    a  b  c  d  e  f  g  h\n");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d ", 8-i);
        for (int j = 0; j < SIZE; j++) {
            if ((i + j) % 2 == 0)
                printf(PURPLEBG);  // Casilla morada
            else
                printf(WHITEBG);   // Casilla blanca

            if (board[i][j] >= 'A' && board[i][j] <= 'Z')
                printf(WHITE " %c " RESET, board[i][j]);
            else if (board[i][j] >= 'a' && board[i][j] <= 'z')
                printf(BLACK " %c " RESET, board[i][j]);
            else
                printf("   " RESET);
        }
        printf(" %d\n", 8-i);
    }
    printf("    a  b  c  d  e  f  g  h\n\n");
}

// Validación simplificada
bool isValidMove(int sx, int sy, int dx, int dy) {
    if (sx < 0 || sx >= SIZE || sy < 0 || sy >= SIZE ||
        dx < 0 || dx >= SIZE || dy < 0 || dy >= SIZE) return false;
    if (board[sx][sy] == ' ') return false;
    if (board[dx][dy] != ' ' &&
        ((board[sx][sy] >= 'A' && board[sx][sy] <= 'Z' && board[dx][dy] >= 'A' && board[dx][dy] <= 'Z') ||
         (board[sx][sy] >= 'a' && board[sx][sy] <= 'z' && board[dx][dy] >= 'a' && board[dx][dy] <= 'z')))
        return false;
    return true;
}

bool movePiece(int sx, int sy, int dx, int dy) {
    if (!isValidMove(sx, sy, dx, dy)) return false;
    board[dx][dy] = board[sx][sy];
    board[sx][sy] = ' ';
    return true;
}

// Detectar si un rey sigue vivo
bool isKingAlive(char king) {
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            if (board[i][j] == king) return true;
        }
    }
    return false;
}

void playerMove() {
    char from[3], to[3];
    int sx, sy, dx, dy;
    while (1) {
        printf("Tu movimiento (ejemplo: e2 e4): ");
        scanf("%s %s", from, to);
        sy = from[0] - 'a';
        sx = 8 - (from[1] - '0');
        dy = to[0] - 'a';
        dx = 8 - (to[1] - '0');
        if (board[sx][sy] >= 'A' && board[sx][sy] <= 'Z' && movePiece(sx, sy, dx, dy)) break;
        printf("Movimiento inválido. Intenta de nuevo.\n");
    }
}

void aiMove() {
    srand(time(NULL));
    int sx, sy, dx, dy;
    while (1) {
        sx = rand() % SIZE;
        sy = rand() % SIZE;
        if (board[sx][sy] >= 'a' && board[sx][sy] <= 'z') {
            dx = rand() % SIZE;
            dy = rand() % SIZE;
            if (movePiece(sx, sy, dx, dy)) {
                printf("La IA movió una pieza de %c%d a %c%d.\n",
                       'a'+sy, 8-sx, 'a'+dy, 8-dx);
                break;
            }
        }
    }
}

int main() {
    printf("=== Ajedrez en C: Jugador (Blancas) vs IA (Negras) ===\n");
    while (1) {
        printBoard();
        playerMove();
        if (!isKingAlive('k')) { // ¿Murió el rey negro?
            printBoard();
            printf("\n¡JAQUE MATE! Ganaste.\n");
            break;
        }
        printBoard();
        aiMove();
        if (!isKingAlive('K')) { // ¿Murió el rey blanco?
            printBoard();
            printf("\n¡JAQUE MATE! La IA ganó.\n");
            break;
        }
    }
    return 0;
}
