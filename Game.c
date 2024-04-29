#include "Game.h"

Game *Game_New() {
    Game *self = (Game *) calloc(1, sizeof(Game));
    AssertNew(self);

    self->state = GAME_IN_PROGRESS;
    self->playerID = PLAYER_1;

    return self;
}

void Game_Delete(Game *self) {
    if (!self) return;
    free(self);
}

Game *Game_Copy(Game *self) {

    Game *copy = (Game *) calloc(1, sizeof(Game));
    AssertNew(copy);
    memcpy(copy, self, sizeof(Game));
    return copy;
}

int TestGameState(Game *self, int P1, int P2, int Take) {
    if (P1 > 3) return 1;
    else if (P2 > 3) return 2;
    else return 0;
}


/// @brief Met à jour l'état de la partie.
/// Cette méthode est appelée par Game_PlayTurn().
/// @param self la partie.
void Game_UpdateState(Game *self) {

    //Ligne
    int P1 = 0, P2 = 0, Take = 0;
    for (int i = 0; i < GRID_H; i++) {
        P1 = 0, P2 = 0;
        for (int j = 0; j < GRID_W; j++) {
            if (self->grid[i][j] == 1) P1++;
            else P1 = 0;
            if (self->grid[i][j] == 2) P2++;
            else P2 = 0;
            self->state = TestGameState(self, P1, P2, Take);
            if (self->state != 0) return;
        }
    }

    P1 = 0, P2 = 0;

    //Colonne
    for (int j = 0; j < GRID_W; j++) {
        P1 = 0, P2 = 0;
        for (int i = 0; i < GRID_H; i++) {
            if (self->grid[i][j] == 1) P1++;
            else P1 = 0;

            if (self->grid[i][j] == 2) P2++;
            else P2 = 0;

            self->state = TestGameState(self, P1, P2, Take);

            if (self->state != 0) return;
        }
    }

    P1 = 0, P2 = 0;

    //Diagonale bas droit
    for (int k = 0; k < GRID_H + GRID_W - 1; k++) {
        P1 = 0, P2 = 0;
        for (int i = 0; i <= k; i++) {
            int j = k - i;
            if (i >= GRID_H || j >= GRID_W) continue;

            if (self->grid[i][j] == 1) P1++;
            else P1 = 0;

            if (self->grid[i][j] == 2) P2++;
            else P2 = 0;

            self->state = TestGameState(self, P1, P2, Take);

            if (self->state != 0) return;
        }
    }

    P1 = 0, P2 = 0;

    //Diagonale bas gauche
    for (int k = 0; k < GRID_H + GRID_W - 1; k++) {
        P1 = 0, P2 = 0;
        for (int i = 0; i <= k; i++) {
            int j = k - i;
            if (i >= GRID_H || j >= GRID_W) continue;

            if (self->grid[i][GRID_W - 1 - j] == 1) P1++;
            else P1 = 0;

            if (self->grid[i][GRID_W - 1 - j] == 2) P2++;
            else P2 = 0;

            self->state = TestGameState(self, P1, P2, Take);

            if (self->state != 0) return;
        }
    }
    for (int i = 0; i < GRID_W; i++) {
        if (self->grid[GRID_H - 1][i] != 0) {
            Take++;
        }
    }


    if (Take == 7) self->state = 3;
}

bool Game_CanPlayAt(Game *self, int column) {
    assert(self);

    if (((column >= 0) && (column <= 6)) && self->grid[5][column] == 0)
        return true;
    else
        return false;
}

void Game_PlayTurn(Game *self, int column) {
    assert(self);
    assert(Game_CanPlayAt(self, column));

    int i = 0;

    while (self->grid[i][column] != 0)
        i++;
    self->grid[i][column] = self->playerID;

    // Met à jour l'état de la partie
    Game_UpdateState(self);

    // Change le joueur courant
    self->playerID = PlayerID_GetOther(self->playerID);
}

void Game_Print(Game *self) {
    printf("\033[0;34m");
    printf("|0 1 2 3 4 5 6|");
    printf("\033[0m");

    for (int i = 5; i > GRID_H - 7; i--) {
        printf("\033[0;34m");
        printf("\n");
        printf("|");
        printf("\033[0m");

        for (int j = 0; j < GRID_W; j++) {
            if (self->grid[i][j] == 0) printf(" ");
            else if (self->grid[i][j] == 1) {
                printf("\033[0;31m");
                printf("O");
                printf("\033[0m");
            } else if (self->grid[i][j] == 2) {
                printf("\033[0;33m");
                printf("O");
                printf("\033[0m");
            }
            printf("\033[0;34m");
            printf("|");
            printf("\033[0m");
        }
    }
}