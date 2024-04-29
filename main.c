#include "Settings.h"
#include "Game.h"
#include "AI.h"

int jvj(){
    char tmp[128];
    Game *game = Game_New();
    while (Game_GetState(game) == GAME_IN_PROGRESS) {
        printf("----------------------------------------\n");
        printf("Tour du joueur %d\n\n", Game_GetPlayerID(game));

        Game_Print(game);
        printf("\n");

        // Choix de la colonne
        int res = 0;
        int column = -1;
        do {
            printf("Choisissez votre colonne (entre 1 et 7) : ");
            res = scanf("%d", &column);

            if (res != 1) {
                scanf("%s", tmp);
            }
        } while (!Game_CanPlayAt(game, column));

        // Action du joueur
        Game_PlayTurn(game, column);
        printf("\n");
    }

    Game_Print(game);

    // Affichage du résultat
    printf("\n----------------------------------------\n");
    switch (Game_GetState(game)) {
        case GAME_P1_WON:
            printf("Le joueur 1 gagne\n");
            //free(game);
            return 1;

        case GAME_P2_WON:
            printf("Le joueur 2 gagne\n");
            //free(game);
            return 2;

        case GAME_IS_TIED:
            printf("Égalité parfaite\n");
            //free(game);
            return 0;

        default:
            printf("ERREUR\n");
            //free(game);
            assert(false);
    }
}

int jvi(){
    char tmp[128];
    Game *game = Game_New();
    while (Game_GetState(game) == GAME_IN_PROGRESS) {
        printf("----------------------------------------\n");
        printf("À votre tour\n\n");

        Game_Print(game);
        printf("\n");

        // Choix de la colonne
        int res = 0;
        int column = -1;
        do {
            printf("Choisissez votre colonne (entre 1 et 7) : ");
            res = scanf("%d", &column);

            if (res != 1) {
                scanf("%s", tmp);
            }
        } while (!Game_CanPlayAt(game, column));

        // Action du joueur
        Game_PlayTurn(game, column);

        printf("\n");
        Game_Print(game);
        printf("\n");
        Game_PlayTurn(game, AI_ComputeMove(game, 2));
    }

    Game_Print(game);


    // Affichage du résultat
    printf("\n----------------------------------------\n");
    switch (Game_GetState(game)) {
        case GAME_P1_WON:
            printf("Vous gagnez\n");
            //free(game);
            return 1;

        case GAME_P2_WON:
            printf("L'IA gagne\n");
            //free(game);
            return 2;

        case GAME_IS_TIED:
            printf("Égalité parfaite\n");
            //free(game);
            return 0;

        default:
            printf("ERREUR\n");
            //free(game);
            assert(false);
    }
}

int ivi(){
    char tmp[128];
    Game *game = Game_New();

    printf("----------------------------------------\n");
    printf("Tour du joueur %d\n\n", Game_GetPlayerID(game));

    Game_Print(game);
    printf("\n");

    Game_PlayTurn(game, (rand() % (7 - 1 + 1)) + 1);

    printf("\n----------------------------------------\n");
    printf("Tour du joueur %d\n\n", Game_GetPlayerID(game));

    Game_Print(game);
    printf("\n");

    Game_PlayTurn(game, (rand() % (7 - 1 + 1)) + 1);

    while (Game_GetState(game) == GAME_IN_PROGRESS) {
        printf("----------------------------------------\n");
        printf("Tour du joueur %d\n\n", Game_GetPlayerID(game));

        Game_Print(game);
        printf("\n");
    }

    Game_Print(game);


    // Affichage du résultat
    printf("\n----------------------------------------\n");
    switch (Game_GetState(game)) {
        case GAME_P1_WON:
            printf("L'IA 1 gagne\n");
            //free(game);
            return 1;

        case GAME_P2_WON:
            printf("L'IA 2 gagne\n");
            //free(game);
            return 2;

        case GAME_IS_TIED:
            printf("Égalité parfaite\n");
            //free(game);
            return 0;

        default:
            printf("ERREUR\n");
            //free(game);
            assert(false);
    }
}




int main() {
    int mode = 0;
    int res = 0;
    char tmp[128];


    // Boucle de jeu
//    while (Game_GetState(game) == GAME_IN_PROGRESS) {
    while (mode != 5) {
        do {
            printf("\nVeuillez choisir un mode :\n[1] Joueur vs Joueur\n[2] Joueur vs IA\n[3] IA vs IA\n[4] Résumé dernière partie\n[5] Sortie\n> ");
            res = scanf("%d", &mode);
            if (res != 1) {
                scanf("%s", tmp);
            }
        } while (res != 1 || (mode < 1 || mode > 5));

        switch (mode) {
            case 5:
                return EXIT_SUCCESS;
                break;

            case 1:
                jvj();
                mode = 0;
                break;

            case 2:
                jvi();
                mode = 0;
                break;

            case 3:
                ivi();
                mode = 0;
                break;

            default:
                mode = 0;
                continue;
        }
    }
}