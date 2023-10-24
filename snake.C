#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>

// Définir les dimensions du terrain
#define LARGEUR 20
#define HAUTEUR 20

// Variables globales
int gameOver;
int enPause = 0;
int x, y, pomme1X, pomme1Y, pomme2X, pomme2Y, pomme3X, pomme3Y ,  score; 
int tailX[100], tailY[100];
int nTail;
const int width = 20;
const int height = 20;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
enum eDirection dir;
int count =0;
int vitesse = 160;
int vitesse2 = 10;

void Sauvegarde(char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier != NULL) {
        // Écrivez les données de sauvegarde ici
        fprintf(fichier, "%d %d %d %d %d %d %d %d %d %d %d %d",
           gameOver, x, y, pomme1X, pomme1Y, pomme2X, pomme2Y, pomme3X, pomme3Y, score, nTail, count);

        for(int i = 0; i < nTail; i++) {
            fprintf(fichier, " %d %d", tailX[i], tailY[i]);
        }

        fclose(fichier);
    }
}

void Charger(char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier != NULL) {
        fscanf(fichier, "%d %d %d %d %d %d %d %d %d %d %d %d",
           &gameOver, &x, &y, &pomme1X, &pomme1Y, &pomme2X, &pomme2Y, &pomme3X, &pomme3Y, &score, &nTail, &count);

        for(int i = 0; i < nTail; i++) {
            fscanf(fichier, " %d %d", &tailX[i], &tailY[i]);
        }

        fclose(fichier);
    }
}

void Setup()
{
    // Initialisation du jeu
    gameOver = 0;
    dir = STOP;
    x = LARGEUR / 2;
    y = HAUTEUR / 2;

     do {
        pomme1X = rand() % width;
        pomme1Y = rand() % height;
    } while ((pomme1X == x && pomme1Y == y) );
    
    do {
        pomme2X = rand() % width;
        pomme2Y = rand() % height;
     
    } while ((pomme2X == x && pomme2Y == y) || (pomme2X == pomme1X && pomme2Y == pomme1Y));
    

     do {
        pomme3X = rand() % width;
        pomme3Y = rand() % height;
     
    } while ((pomme3X == x && pomme3Y == y) || (pomme3X == pomme2X && pomme3Y == pomme2Y)|| (pomme3X == pomme1X && pomme3Y == pomme1Y) );
    score = 0;
}

void Draw()
{
    // Affichage du terrain
    system("cls");
    for(int i = 0; i < LARGEUR+2; i++)
        printf("#");
    printf("\n");

    for(int i = 0; i < HAUTEUR; i++)
    {
        for(int j = 0; j < LARGEUR; j++)
        {
            if(j == 0)
                printf("#");
            
            if(i == y && j == x)
                printf("O");
             
    // Affichage de la premiere pomme
             if (i == pomme1X && j == pomme1Y)
                  printf("*");
    // Affichage de la deuxieme pomme
             else if (i == pomme2X && j == pomme2Y)
                  printf("*");
    // Affichage de la troisieme pomme
             else if (i == pomme3X && j == pomme3Y)
                  printf("*");
            else
            {
                int isPrinted = 0;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        printf("o");
                        isPrinted = 1;
                    }
                }
                if (isPrinted == 0)
                    printf(" ");
            }

            if(j == LARGEUR-1)
                printf("#");
        }
        printf("\n");
    }

    for(int i = 0; i < LARGEUR+2; i++)
        printf("#");
    printf("\n");
    printf("Score:%d",score);
    printf("\n");
    printf("Pour sauvegarder la partie, cliquez sur c" );
    printf("\n");
    printf("Pour charger la partie, cliquez sur v" );
    printf("\n");
    printf("Pour vitesse rapide, cliquez sur r" );
    printf("\n");
    printf("Pour vitesse lente, cliquez sur t ");
}

void Input()
{
    // Gestion des entrées clavier
    Sleep(100);  // Attend pendant 100 millisecondes (0.1 seconde)
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'e':
            dir = RIGHT;
            break;
        case 'z':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = 1;
            break;

        case 'c':
            {
                char nomSauvegarde[50]; // Nom du fichier de sauvegarde
                printf("Entrez un nom de fichier pour la sauvegarde : ");
                scanf("%s", nomSauvegarde);
                Sauvegarde(nomSauvegarde); 
            }
            break;

        case 'v':
            {
                char nomChargement[50]; // Nom du fichier de chargement
                printf("Entrez un nom de fichier pour le chargement : ");
                scanf("%s", nomChargement);
                Charger(nomChargement);
            }
            break;  

        case 'r':
            vitesse = vitesse2; 
            break;

         case 't':
            vitesse = 160; 
            break;
        
        }
    }
}

void Logic()
{
    // Mise à jour de la logique du jeu
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for(int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch(dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if(x >= LARGEUR)  gameOver = 1; else if(x < 0)  gameOver = 1;
    if(y >= HAUTEUR)  gameOver = 1; else if(y < 0)  gameOver = 1;

    for(int i = 0; i < nTail; i++)
        if(tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    if (x == pomme1Y && y == pomme1X)
    {
        pomme1X = -1; 
        pomme1Y = -1;
        count++;
    }

    if (x == pomme2Y && y == pomme2X)
    {
        pomme2X = -1; 
        pomme2Y = -1;
        count++;
    }

    if (x == pomme3Y && y == pomme3X)
    {
        pomme3X = -1; 
        pomme3Y = -1;
        count++;
    }

    if (count == 3)
    {
        score += 10;
        nTail++;

        // Réinitialiser les positions des pommes
        do {
            pomme1X = rand() % LARGEUR;
            pomme1Y = rand() % HAUTEUR;
        } while ((pomme1X == x && pomme1Y == y) || (pomme1X == pomme2Y && pomme1Y == pomme2X) || (pomme1X == pomme3Y && pomme1Y == pomme3X));

        do {
            pomme2X = rand() % LARGEUR;
            pomme2Y = rand() % HAUTEUR;
        } while ((pomme2X == x && pomme2Y == y) || (pomme2X == pomme1Y && pomme2Y == pomme1X) || (pomme2X == pomme3Y && pomme2Y == pomme3X));

        do {
            pomme3X = rand() % LARGEUR;
            pomme3Y = rand() % HAUTEUR;
        } while ((pomme3X == x && pomme3Y == y) || (pomme3X == pomme1Y && pomme3Y == pomme1X) || (pomme3X == pomme2Y && pomme3Y == pomme2X));

        count = 0;
    }
}


int main() {
    Setup();
    char nomSauvegarde[50]; // Nom du fichier de sauvegarde
    char nomChargement[50]; // Nom du fichier de chargement

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(vitesse);
    }

    printf("\n\n\n");
    printf("  GGGGG   AAAAA  M       M EEEEE   \n");
    printf(" G       A     A MM     MM E       \n");
    printf(" G  GGG  AAAAAAA M M   M M EEEE    \n");
    printf(" G    G  A     A M  M M  M E       \n");
    printf("  GGGGG  A     A M   M   M EEEEE   \n");
    printf("\n");
    printf("  OOO  V       V EEEEE RRRR  \n");
    printf(" O   O V       V E     R   R \n");
    printf(" O   O  V     V  EEEE  RRRR  \n");
    printf(" O   O   V   V   E     R  R  \n");
    printf("  OOO     VVV    EEEEE R   R \n");
    printf("\n");
    printf("\n");
    printf("Appuyez sur 'x' pour quitter\n");
    printf("\n");
    printf("Game Over!\n");

    while (1) {
        if (_kbhit()) {
            switch (_getch()) {
                case 'x':
                    return 0; // Quitter en appuyant sur 'x'
                    break;
                case 'c':
                    printf("Entrez un nom de fichier pour la sauvegarde : ");
                    scanf("%s", nomSauvegarde);
                    Sauvegarde(nomSauvegarde);
                    break;
                case 'v':
                    printf("Entrez un nom de fichier pour le chargement : ");
                    scanf("%s", nomChargement);
                    Charger(nomChargement);
                    break;
                case 'r':
                    vitesse = vitesse2;
                    break;
                
                case 't':
                    vitesse = 160;
                    break;
                
                default:
                    break;
            }
        }
    }

    return 0;
}