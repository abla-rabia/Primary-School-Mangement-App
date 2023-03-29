#include "Menu_tovc.h"
#include <windows.h>
void menu_1()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
    printf("\n\n\n");
    printf("                                   $$   $$  $$$$  $$  $  $  $\n");
    printf("                                   $ $ $ $  $     $ $ $  $  $\n");
    printf("                                   $ $$$ $  $$$$  $  $$  $  $\n");
    printf("                                   $  $  $  $     $   $  $  $ \n");
    printf("                                   $     $  $$$$  $   $  $$$$\n  ");
    printf("                                 __________________________\n");
    printf("                                     Que voulez vous faire ?");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
    printf("\n\n");
    printf("             ----------------------------------------------------------------------\n");
    printf("            |                                                                      |\n");
    printf("            |       _____                                 _____                    |\n");
    printf("            |      |_____\\_______                        |_____\\_______            |\n");
    printf("            |      |             |                       |             |           |\n");
    printf("            |      | 1-Affichage |                       | 2-Recherche |           |\n");
    printf("            |      |_____________|                       |_____________|           |\n");
    printf("            |                                                                      |\n");
    printf("            |                                                                      |\n");
    printf("            |       _____                                 _____                    |\n");
    printf("            |      |_____\\_______                        |_____\\_______            |\n");
    printf("            |      |             |                       |             |           |\n");
    printf("            |      |  3-Ajouter  |                       | 4-Supprimer |           |\n");
    printf("            |      |_____________|                       |_____________|           |\n");
    printf("            |                                                                      |\n");
    printf("            |                                                                      |\n");
    printf("            |       _____                                 _____                    |\n");
    printf("            |      |_____\\_______                        |_____\\_______            |\n");
    printf("            |      |             |                       |             |           |\n");
    printf("            |      |    5-MaJ    |                       | 6-Archiver  |           |\n");
    printf("            |      |_____________|                       |_____________|           |\n");
    printf("            |                                                                      |\n");
    printf("            |                                                                      |\n");
    printf("            |                       7- QUITTER LE PROGRAMME.                       |\n");
    printf("             ----------------------------------------------------------------------\n");
    printf("                                    Votre choix : ");

}
