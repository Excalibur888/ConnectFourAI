#include "AI.h"
#include "Game.h"
#include <stdbool.h>

INLINE int Int_Max(int a, int b) { return a > b ? a : b; }

INLINE int Int_Min(int a, int b) { return a < b ? a : b; }


//
//  Check ligne à droite
//
INLINE int checkLDroite(int currentPosI, int currentPosJ, Game *self, int playerID) {
    bool possible = 1;
    int score = 0;

    if (currentPosJ - 1 < 0)
        possible = 0;

    if (self->grid[currentPosI][currentPosJ - 1] != playerID && self->grid[currentPosI][currentPosJ - 1] != 0)
        possible = 0;

    for (int i = 0; i < 4; ++i) {
        if (currentPosJ + i > 6)
            continue;
        if (self->grid[currentPosI][currentPosJ + i] == playerID || self->grid[currentPosI][currentPosJ + i] == 0)
            ++score;
        else
            score = 0;
    }
    if (score == 4 && possible)
        return 2;
    if ((score == 3 && possible) || score == 4)
        return 1;
    return 0;
}

//
//  Check colonne en haut
//
INLINE int checkCHaut(int currentPosI, int currentPosJ, Game *self, int playerID) {
    bool possible = 1;
    int score = 0;

    if (currentPosI - 1 < 0)
        possible = 0;

    if (self->grid[currentPosI - 1][currentPosJ] != playerID && self->grid[currentPosI - 1][currentPosJ] != 0)
        possible = 0;

    for (int i = 0; i < 4; ++i) {
        if (currentPosI + i > 5)
            continue;
        if (self->grid[currentPosI + i][currentPosJ] == playerID || self->grid[currentPosI + i][currentPosJ] == 0)
            ++score;
        else
            score = 0;
    }
    if (score == 4 && possible)
        return 2;
    if ((score == 3 && possible) || score == 4)
        return 1;
    return 0;
}

//
//  Check Diagonale en haut à droite
//
INLINE int checkDHautDroite(int currentPosI, int currentPosJ, Game *self, int playerID) {
    bool possible = 1;
    int score = 0;

    if (currentPosI - 1 < 0 || currentPosJ - 1 < 0)
        possible = 0;

    if (self->grid[currentPosI - 1][currentPosJ - 1] != playerID && self->grid[currentPosI - 1][currentPosJ - 1] != 0)
        possible = 0;


    for (int i = 0; i < 4; ++i) {
        if (currentPosI + i > 5)
            continue;
        if (currentPosJ + i > 6)
            continue;
        if (self->grid[currentPosI + i][currentPosJ + i] == playerID ||
            self->grid[currentPosI + i][currentPosJ + i] == 0)
            ++score;
        else
            score = 0;
    }

    if (score == 4 && possible)
        return 2;
    if ((score == 3 && possible) || score == 4)
        return 1;
    return 0;
}


int Game_Eval(Game *self, int playerID) {

    int res = 0;
    int score = 0;
    int weight[6][7] = {3, 4, 5, 7, 5, 4, 3, 4, 6, 8, 10, 8, 6, 4, 5, 8, 11, 13, 11, 8, 5, 5, 8, 11, 13, 11, 8, 5, 4, 6, 8, 10, 8, 6, 4, 3, 4, 5, 7, 5, 4, 3};

    for (int i = 0; i < GRID_H; ++i) {
        for (int j = 0; j < GRID_W; ++j) {
            score = 0;
            score += checkLDroite(i, j, self, playerID) * weight [i][j];
            score += checkCHaut(i, j, self, playerID) * weight [i][j];
            score += checkDHautDroite(i, j, self, playerID) * weight [i][j];

            playerID = 3 - playerID;

            score += checkLDroite(i, j, self, playerID) * weight [i][j] ;
            score += checkCHaut(i, j, self, playerID) * weight [i][j];
            score += checkDHautDroite(i, j, self, playerID) * weight [i][j];

            if (self->grid[i][j] == 1)
                res += score;
            if (self->grid[i][j] == 2)
                res -= score;
        }
    }


    if (playerID == 2) {
        //printf("res : %d\n", -res);
        return -res;
    }
    //printf("res : %d\n", res);
    return res;
}

/// @brief Détruit récursivement un noeud et tous ses sous-arbres.
/// @param self le noeud.
void AINode_DeleteRec(AINode *self) {
    if (self == NULL) {
        return;
    }
    Game_Delete(self->gameState);
    for (int i = 0; i < GRID_W; i++) {
/**/        if (self->children[i] != NULL) {
            AINode_DeleteRec(self->children[i]);
        }
    }
    free(self);
}

/// @brief Crée un noeud de l'arbre de l'algorithme MinMax.
/// Cette méthode est appelée par AITree_NewMinMax().
/// @param game la partie associée au noeud.
/// @param depth la hauteur maximale du noeud.
/// @param minLayer indique si le noeud est un noeud MIN.
/// @param playerID identifiant du joueur dont l'algorithme calcule l'action.
/// @return Le noeud créé (et initialisé).
AINode *AINode_NewMinMax(Game *game, int depth, bool minLayer, int playerID, int alpha, int beta) {
    AINode *self = (AINode *) calloc(1, sizeof(AINode));
    AssertNew(self);

    self->gameState = game;
    if (game->state != GAME_IN_PROGRESS) {
        if (game->state == playerID) self->score = 999999;
        else if (game->state == GAME_IS_TIED) self->score = 0;
        else self->score = -999999;
        return self;
    }

    if (depth == 0) {
        self->score = Game_Eval(game, playerID);
        return self;
    }

    int tab[] = {3, 2, 4, 1, 5, 0, 6};

    self->score = (minLayer) ? +1000000000 : -1000000000;
    self->move = -100;


    for (int i = 0; i < GRID_W; i++) {
        int colone = tab[i];

        if (Game_CanPlayAt(game, colone) == true) {

            Game *newGame = Game_Copy(game);
            Game_PlayTurn(newGame, colone);
            self->children[colone] = AINode_NewMinMax(newGame, depth - 1, !minLayer, playerID, alpha, beta);

            if (minLayer) {
                if (self->children[colone]->score < self->score) {
                    self->score = self->children[colone]->score;
                    self->move = colone;
                }
                beta = Int_Min(beta, self->score);

            } else {
                if (self->children[colone]->score > self->score) {
                    self->score = self->children[colone]->score;
                    self->move = colone;
                }
                alpha = Int_Max(alpha, self->score);
            }
        }
        AINode_DeleteRec(self->children[colone]);
        self->children[colone] = NULL;
        if (beta <= alpha) {
            return self;
        }
    }
    return self;
}

AITree *AITree_NewMinMax(Game *game, int depth, int playerID) {
    AITree *self = (AITree *) calloc(1, sizeof(AITree));
    AssertNew(self);

    Game *curGame = Game_Copy(game);
    self->root = AINode_NewMinMax(curGame, depth, false, playerID, -1000000000, 1000000000);

    return self;
}

void AITree_Delete(AITree *self) {
    if (!self) return;

    AINode_DeleteRec(self->root);
    free(self);
}

int AI_ComputeMove(Game *game, int playerID) {
    /////////
    // TODO : Cette fonction représente votre IA.
    // Modifiez-la comme vous le souhaitez.

    int depth = 8;
    AITree *aiTree = AITree_NewMinMax(game, depth, playerID);
    int move = aiTree->root->move;
    AITree_Delete(aiTree);

    return move;
}