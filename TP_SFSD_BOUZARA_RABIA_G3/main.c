
#include <stdio.h>
#include <stdlib.h>
#include "dec_functions.h"
#include <time.h>
#include <windows.h>
#include "Menu_tovc.h"




///Voici la reponse des question du tp , pour une meuilleur lisibilite  veuillez le trouver sou format pdf dans le repertoire courant ^^

/**----------------------------------------------------------QUESTIONS TP---------------------------------------------------------------**/
/**
3-a-
Dans la partie 1 du TP , on a déjà  utilisé un arbre de recherche binaire a fin d’enregistrer les ID des élèves  puisqu’ils sont uniques , et l’arbre facilitera la recherche/verification si l’ID est déjà  utilisé ou non  .donc  on a décidé de la ré-utiliser  pour construire notre fichier archive , car ça va diminuer les nombre d’accès disque vers le fichier initial F, et on a procéder  comme suit :
	On améliore la structure de l’arbre en ajoutant autres champs : Nom, prenom, supp, année _scolaire et la moyenne correspondante  á  l’année _scolaire .
	On optimise/ ajoute  les mises à jour nécessaires dans les fonctions précédemment créées .donc à la fin de la création du fichier élèves ,on aura notre arbre avec tous ses champs  remplis , et tout opérations sur le fichier des élèves (insertion , Maj ,suppression  ) s’imposera une sur l’arbre (si elle est nécessaire ).
	La création initiale de l’archive va être faite par le parcours de l’arbre (Parcours Inordre ) et le remplissage du fichier archive par  les informations  des élèves  (nom ,prénom, ID)+ l’année  scolaire et la moyenne calculée .
	A la fin de chaque année scolaire, on archive des résultats des élèves en 2 étapes globales :
i.	M-A-J de l’arbre : simultanément  avec le chargement  du fichier des élèves  ,d’où  larbre va contenir  les anciens élèves  avec l’année scolaires incrémenté (ou pas si ils sont en 5 eme année) + la nouvelle moyenne calculée  . et les nouveaux étudiants  des années préparatoire  (+100 éleves chaque année).
ii.	M-A-J archive : en utilisant l’arbre , on insère  les étudiants des années préparatoires et on ajoute les informations de l’année dernière pour les anciens.

	Comme l’arbre est ordonnée selon les identifiants des élèves , alors la clé sera l’identifiant  (ID).
	Le type du modèle choisi : TOF.
3-b- Comme le fichier archive est de type TOF (ordonné ), et il s’agit d’une recherche par intervalle, il faut choisir un index du type non dense.
On a choisi ce type et on a améliorer  la structure de cet index en ajoutant le ID min et max par bloc afin de l’utiliser dans les autres opérations  (recherche,insertions….).
Par exemple , dans l’insertions ..il suffit juste de faire une recherche dans la table d’index pour trouver la position i, j … donc 1 seule lecture +écriture sans avoir besoin de la recherche dichotomique dans le fichier TOF.
	Type index  choisi : Index non dense.

4-a-
le coût de l’archivage :
On utilise l’arbre pour le remplissage de l’archive ,donc on aura pas besoin du fichier principale .
A.	Creation de l’archive  (année 1 ) :
	On choisit :
1.	Les blocs de l’archive avec capacité  maximale  de 25 enreg par bloc.
2.	Taux de chargement  : 10 eleves par bloc (pour eviter les decalage cause par les insertions dans les années suivantes
	On a 600 élèves  au debut , et 10 eleves par bloc donc initialement  il y’a 60 blocs dans l’archive.

Le coût  totale = 60 écriture .

B.	Archivage des années precedents  (4 années) :
	Archivage des résultats des anciens élèves  : 1 seul parcours séquentiels du fichier archive , supposons que l’archive  ayant  N bloc , donc le coût  = N lecture + N écriture  pour chaque années.
	Insertion des élèves (nouveaux) :  insertion avec décalage sans recherche , mais en utilisant l’index de l’archive , et on a 100 nouveaux élèves  chaque  année .
Supposons N : nombre de blocs et prenons le pire des cas : tous les blocs sont remplis + décalage de tout le fichiers , le coût  : 100 (N lecture + N écriture )

Le coût  totale : 101 (N lecture + N écriture)


D’où  le coût  d’archivage pour 5 année  : 404 ( N lecture + N écriture) + 600 écritures.

Dans le cas ou on insère des nouveaux élèves dans le fichier initial, le cout va évidemment augmenter du nombre d’élèves insérés .
**/
/**-------------------------------------------------------------------------------------------------------------------------------------**/
int main()
{   bool nouv_fich=1;
    bool passage=0;
    char old_file[max_file];
    int cmp_annee=1;
    TNoeud *r;
    char nv;
    char note [TeM];
    char classe_an[Tc];
    bool retour4;
    char matiere[max_str];
    int not;
    char nom_ecole[max_str];
    char nom_an[max_str];
    char prenom_an[max_str];
    bool  retour2 ,retour3;
    char classe[Tc];
    char g;
    char ch[max_e];
    char nom[max_str];
    char prenom[max_str];
    int b,p,pos,tn,id;
    tab_ch matieres[TeM/2];
    char nom_fich[max_file];
    char nom_archive[max_file];
    int annee_courante;
    int no[TeM/2];
    f2=fopen("noms.txt","r");
    f1=fopen("prenoms.txt","r");
    f3=fopen("f3.txt","r");
    bool arret=0;
    bool retour=0;
    srand(time(NULL));
    Affichage();
    int choix1,choix2,choix3,choix4,choix5,choix6,choix7,choix8;
    system("cls||clear");
    printf("\n\n\n\n            Que voulez vous faire : \n\n");
    printf("              --------------------------------------------------------------------------\n");
    printf("             |       1 ) ---> Creer un nouveau fichier de travail .                     |\n");
    printf("             |       2 ) ---> Travailler sur un ancien fichier qui existe deja .        |\n");
    printf("             |       3 ) ---> Quitter le programme .                                    |\n");
    printf("              --------------------------------------------------------------------------\n\n");
    printf("                   Votre choix : ");
    scanf("%d",&choix1);
    printf("\n\n");
    switch(choix1){
    case (2) :
        {
            system("cls||clear");
            printf("\n\n\n            Vous avez choisis d'utiliser un ancien fichier ! Aidez nous a les trouver !\n");
            printf("\n\n\n\n            --> Veuillez saisir le nom de votre fichier initial  : ");
            scanf("%s",nom_fich);
            printf("\n\n\n\n            --> Veuillez saisir le nom de votre fichier archive : ");
            scanf("%s",nom_archive);
            system("cls||clear");
            F=Ouvrir(nom_fich,'a');
            if (F==NULL)
                printf("\n\n            Votre fichier n'existe pas maleuresement . Creez le sinon .\n");
            else{
                A=ouvrir_Archive(nom_archive,'a');
                if( A==NULL){
                    printf("\n\n            Votre fichier Archive n'existe pas maleuresement . C'est parcequ'il na pas ete cree comme vous avez quitter le programme avant de commencer l'archive, donc on va commencer la simulation de 0 ! .\n");
                }
                else{
                    printf("\n\n            On a reussie a trouver les deux fichier ! et la simulation vas commencer de la %d annee ! \n",entete_Archive(A,2)+1);
                    cmp_annee=entete_Archive(A,2)+2;
                    Chargement_indexA();
                    nouv_fich=0;
                    getchar();
                    getchar();
                }
                fermer_Archive(A);
                fermer(F);
                Chargement_arb();
                Chargement_index();
                printf("\n\n            Appuyer sur ENTRER pour commencer la manipulation de votre fichier ! ! \n\n\n ");
            do{
                getchar();
                system("cls");
                menu_1();
                scanf("%d",&choix2);
                switch(choix2){
                case (1):
                    {
                        retour=0;
                        do{
                        system("cls");
                        printf("\n\n\n\n            Que voulez vous afficher ?  \n\n");
                        printf("              --------------------------------------------------------------------------\n");
                        printf("             |       1 ) ---> La liste des eleves dans l'ecole .                        |\n");
                        printf("             |       2 ) ---> Les matieres .                                            |\n");
                        printf("             |       3 ) ---> Retour au menu principal .                                |\n");
                        printf("              --------------------------------------------------------------------------\n\n");
                        printf("                   Votre choix : ");
                        scanf("%d",&choix3);
                            switch (choix3){
                            case(1):
                                {
                                    system("cls");
                                    Affichage1(nom_fich);
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    getchar();
                                    system("cls");
                                    printf("\n\n\n\n\n");
                                    break;
                                }
                            case(2):
                                {
                                    retour2=0;
                                    do{
                                    system("cls");
                                    printf("\n\n\n\n            Qu'elle liste de matieres voulez vous afficher  ?  \n\n");
                                    printf("              --------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Les matieres des 1eres et 2emes annees .                  |\n");
                                    printf("             |       2 ) ---> Les matieres des 3iemes,4iemes et 5iemes annee.           |\n");
                                    printf("             |       3 ) ---> Retour au menu de l'affichage .                           |\n");
                                    printf("              --------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix4);
                                    switch(choix4){
                                        case(1):
                                            {
                                                system("cls");
                                                int tn=(TeM-6)/2;
                                                Rech_matieres("11",matieres);
                                                printf("\n\n        Voici les matieres : \n");
                                                printf("        __________________________________________\n\n");
                                                for (int u=0;u<tn;u++){
                                                    printf("                --> %s .\n",matieres[u]);

                                                }
                                                printf("        __________________________________________\n\n");
                                                printf("\n\n            Appuyer sur ENTRER pour aller au menu des matieres ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                system("cls");
                                                printf("\n\n\n\n\n");
                                                break;

                                            }
                                        case (2):
                                            {
                                                system("cls");
                                                int tn=(TeM)/2;
                                                Rech_matieres("31",matieres);
                                                printf("\n\n        Voici les matieres : \n");
                                                printf("        __________________________________________\n\n");
                                                for (int u=0;u<tn;u++){
                                                    printf("                --> %s .\n",matieres[u]);

                                                }
                                                printf("        __________________________________________\n\n");
                                                printf("\n\n            Appuyer sur ENTRER pour aller au menu des matieres ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                system("cls");
                                                printf("\n\n\n\n\n");
                                                break;
                                            }
                                        case(3):{
                                            retour2=1;
                                            break;
                                        }
                                        }
                                    } while(!retour2);
                                    break;
                                }
                            case (3):
                                {
                                    retour=1;
                                    break;
                                }
                            }
                        }while (!retour);
                        break;
                    }
                            case(2):
                                {
                                    do{
                                            system("cls");
                                    printf("\n\n\n\n            Que voulez vous rechercher   ?  \n\n");
                                    printf("              --------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Savoir seulement si un eleve figure dans l'ecole .        |\n");
                                    printf("             |       2 ) ---> Les eleves d'une classe .                                 |\n");
                                    printf("             |       3 ) ---> Les eleves d'un niveau  .                                 |\n");
                                    printf("             |       4 ) ---> Le parcours d'un eleve dans cette annee .                 |\n");
                                    printf("             |       5 ) ---> Retour au menu principal .                                |\n");
                                    printf("              --------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix5);
                                    system("cls");
                                    switch (choix5){
                                    case(1):
                                        {
                                            system("cls");
                                            printf(" \n\n\n\n\n           # ) Entrer la classe de l'eleve : ");
                                            scanf("%s",classe);
                                            printf("            # ) Entrer le nom de l'eleve : ");
                                            scanf(" %[^\n]s",nom);
                                            printf("            # ) Entrer le prenom de l'eleve : ");
                                            scanf(" %[^\n]s",prenom);
                                            F=Ouvrir(nom_fich,'a');
                                            if (!Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch))
                                                printf("        - %s  %s ne figure pas dans la liste des eleves de %s .\n",nom, prenom,classe);
                                            else
                                                printf("        - %s  %s existe dans la liste des eleves de %s .\n",nom, prenom,classe);
                                            fermer(F);
                                            printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                            getchar();
                                            getchar();

                                            break;

                                        }
                                    printf("\n\n\n\n\n");
                                    break;
                                    case (2):
                                        {
                                            system("cls");
                                        printf("\n\n\n\n\n            # ) Entrer la classe : ");
                                        scanf("%s",classe);
                                        affiche_classe(nom_fich,classe);
                                        printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                        getchar();
                                        getchar();
                                        break;
                                    }
                                    case (3):
                                        {
                                            system("cls");
                                        printf("\n\n\n\n\n            # ) Entrer le niveau (p,1,2,3,4 ou 5): ");
                                        scanf(" %c",&nv);
                                        affiche_nv(nom_fich,nv);
                                        printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                        getchar();
                                        getchar();
                                        break;
                                    }
                                    case(4):
                                        {
                                        system("cls");
                                        printf("\n\n\n\n\n           # ) Entrer la classe de l'eleve : ");
                                        scanf("%s",classe);
                                        printf("            # ) Entrer le nom de l'eleve : ");
                                        scanf(" %[^\n]s",nom);
                                        printf("            # ) Entrer le prenom de l'eleve : ");
                                        scanf(" %[^\n]s",prenom);
                                        F=Ouvrir(nom_fich,'a');
                                        if (Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch)){
                                                if (classe[0]=='p' || classe[0]=='P')
                                                    tn=0;
                                                else{
                                                    if (classe[0]=='1' || classe[0]=='2' ){
                                                        tn=TeM-6;
                                                    }
                                                    else if (classe[0]=='3' || (classe[0]=='4' || classe[0]=='5') )
                                                        tn=TeM;
                                                    else
                                                        printf("\n              Erreur ! La classe n'existe pas !!!\n\n");
                                                }
                                                    getnotes(ch,tn,no);
                                                    id=getId(ch);
                                                    if (classe[0]!= 'p' && classe[0]!= 'P')
                                                        Rech_matieres(classe,matieres);
                                                    g=ch[strlen(nom)+strlen(prenom)+Tl+Tc+Tid];
                                                    printf("\n          ----------------------------\n");
                                                    printf("               Nom    :   %s           \n",nom);
                                                    printf("               Prenom :   %s           \n",prenom);
                                                    printf("               Classe :   %s           \n",classe);
                                                    printf("               Id     :   %d           \n",id);
                                                    printf("               genre  :   %c           \n",g);
                                                    printf("\n          ----------------------------\n");
                                                    if (tn!=0){
                                                    printf("            Voici les notes de %s %s dans les differentes matieres etudiees \n",nom,prenom);
                                                    for (int x=0;x<tn/2;x++){
                                                    printf("            %s  : %d  \n",matieres[x],no[x]);
                                                    }
                                                    }
                                                }
                                                else
                                                    printf("\n\n\n              Erreur ! L'eleve ne figure pas dans la liste des eleves de %s\n\n",classe);
                                                fermer(F);
                                                printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                break;
                                                }
                                                case (5):{
                                                    retour3=1;
                                                    break;
                                                }
                                    }
                                    } while (!retour3);
                                    break;
                                }
                                case (3):
                                {
                                    system("cls");
                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                    scanf("%s",classe);
                                    printf("            # ) Entrer le nom de l'eleve : ");
                                    scanf(" %[^\n]s",nom);
                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                    scanf(" %[^\n]s",prenom);
                                    do{
                                    printf("            # ) Entrer le genre de l'eleve : ");
                                    scanf(" %c",&g);
                                    if ((g!='m' && g!='M') && (g!='F'&& g!='f'))
                                        printf("\n\n\n         ERREUR!!!   Le genre n'est que (M ou m) ou (F ou f) !!!!\n\n\n");
                                    }while((g!='m' && g!='M') && (g!='F'&& g!='f'));
                                    if (classe[0]!='p' && classe[0]!='P' )
                                        Saisie_note(classe,note,no);
                                    else
                                        strcpy(note,"");
                                    if(!insert(nom_fich,classe,nom,prenom,note,g,no)){
                                        printf("\n\n\n\n\n          Erreur ! L'eleve %s %s existe deja !\n",nom,prenom);
                                    }
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    break;
                                }
                                case (4):
                                {
                                    system("cls");
                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                    scanf("%s",classe);
                                    printf("            # ) Entrer le nom de l'eleve : ");
                                    scanf(" %[^\n]s",nom);
                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                    scanf(" %[^\n]s",prenom);
                                    if(!Suppression(nom_fich,classe,nom,prenom)){
                                        printf("\n\n\n\n\n          Erreur ! L'eleve %s %s n'existe pas !\n",nom,prenom);
                                    }
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    break;
                                }
                                case (5):
                                    {
                                        retour4=0;
                                        do{
                                            system("cls");
                                            printf("\n\n\n\n            Que voulez vous mettre a jour    ?  \n\n");
                                            printf("              --------------------------------------------------------------------------\n");
                                            printf("             |       1 ) ---> Mise a jour d'une notes  .                                |\n");
                                            printf("             |       2 ) ---> Mise a jour d'un nom.                                     |\n");
                                            printf("             |       3 ) ---> Mise a jour d'une classe                                  |\n");
                                            printf("             |       4 ) ---> Retour au menu principal .                                |\n");
                                            printf("              --------------------------------------------------------------------------\n\n");
                                            printf("                   Votre choix : ");
                                            scanf("%d",&choix6);
                                            system("cls");
                                            switch (choix6){
                                            case (1):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer le nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    printf("            # ) Entrer le nom de la matiere : ");
                                                    scanf(" %[^\n]s",matiere);
                                                    do {printf("            # ) Entrer la nouvelle note sur 20 : ");
                                                    scanf(" %d",&not);
                                                    if (not>20)
                                                        printf("            \n Erreur ! La note ne peut pas depasser 20 !!\n");
                                                    } while(not>20);
                                                    if (MaJ_Notes(nom_fich,classe,nom,prenom,matiere,not))
                                                        printf("             La note a ete modifie ! vous pouvez consulter le parcours dans la recheche pour voir la modification !");
                                                     printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                     getchar();
                                                     getchar();
                                                     break;
                                                }
                                            case (2):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer l'ancien nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom_an);
                                                    printf("            # ) Entrer l'ancien prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom_an);
                                                    printf("            # ) Entrer le nouveau nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le nouveau prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    if (MaJ_Noms(nom_fich,classe,nom_an,prenom_an,nom,prenom))
                                                        printf("            Le nom et le prenom ont ete modifie ! vous pouvez consulter le parcours dans la recheche pour voir la modification !");
                                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                    getchar();
                                                    getchar();
                                                    break;
                                                }
                                            case (3):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer l'ancienne classe de l'eleve : \n");
                                                    scanf("%s",classe_an);
                                                    printf("             # ) Entrer la nouvelle classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer le nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    if (MaJ_classe(nom_fich,classe_an,nom,prenom,classe))
                                                        printf("            La classe a ete modifie ! vous pouvez consulter la liste des eleves pour voir la modification !");
                                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                    getchar();
                                                    getchar();
                                                    break;
                                                }
                                            case (4):
                                                {
                                                    retour4=1;
                                                    break;
                                                }
                                            }
                                        }while(!retour4);
                                        break;
                                    }

                case(6):{ // archiver
                    if (nouv_fich){ // nouv archive
                        system("cls||clear");
                        do{
                            printf("\n\n            --> Veuillez saisir le nom de votre fichier archive : ");
                            scanf("%s",nom_archive);
                            if (strcmp(nom_archive,nom_fich)==0)
                                printf("\n\n\n          ERREUR !!!! Le nom de l'archive ne peut pas etre le meme nom du fichier !! vous allez perdre de donnees comme ca !!\n\n\n");
                        }while(strcmp(nom_archive,nom_fich)==0);
                        chargement_init_Archive(nom_archive);
                        printf("\n\n\n\n           -->Mission accomplie ! votre archive a ete cree avec succes :) !");
                        printf("\n\n            -->Merci  d'etre patientes :) ");
                        nouv_fich=0;
                    }
                    else{
                            MAJ_Archive(nom_archive);
                            printf("\n\n\n\n           -->Mission accomplie ! votre archive a ete modifiee avec succes :) !");
                            printf("\n\n            -->Merci  d'etre patientes :) ");
                    }
                        printf("\n\n\n\n           Appuyer sur ENTRER pour Commencer la manipulation de l'archive ! \n\n\n ");
                        getchar();
                        getchar();
                        system("cls");
                        bool retour5=0;
                        passage=0;
                        do{

                                    system("cls");
                                    printf("\n\n\n\n            Que voulez vous  faire   ?  \n\n");
                                    printf("              ----------------------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Afficher l'historique d'un eleve pendant toutes ses annees scolaires .  |\n");
                                    printf("             |       2 ) ---> Lister les eleves selon une moyenne seuille .                           |\n");
                                    printf("             |       3 ) ---> Un Transfert d'un etudiant   .                                          |\n");
                                    printf("             |       4 ) ---> Passer a l'annee prochaine   .                                          |\n");
                                    printf("             |       5 ) ---> Retour au menu principal .                                              |\n");
                                    printf("              ----------------------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix8);
                                    system("cls");
                                    switch (choix8){
                                        case(1): // historique d'1 eleve
                                        {           system("cls");
                                                    printf("\n\n\n               #) Voulez vous faire un recherche par    :          \n");
                                                    printf("\n                    1/ NOM / PRENOM / CLASSE         \n");
                                                    printf("\n                    2/ IDENTIFIANT                   \n");
                                                    printf("\n\n                     Votre choix : ");
                                                    int u;
                                                    scanf("%d",&u);
                                                    system("cls");
                                                    if (u==1){
                                                        printf("\n               Nom    :              ");
                                                        scanf(" %[^\n]s",nom);
                                                        printf("\n               Prenom :              ");
                                                        scanf(" %[^\n]s",prenom);
                                                        printf("\n               Classe :              ");
                                                        scanf("%s",classe);
                                                        F=Ouvrir(nom_fich,'a');
                                                        if (Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch)){
                                                            id=getId(ch);
                                                        }
                                                        else
                                                            printf("\n\n\n          Erreur !! L'eleve %s %s n'existe pas dans la classe %s",nom,prenom,classe);
                                                        fermer(F);
                                                    }
                                                    else
                                                    {
                                                        printf("\n               Id     :              ");
                                                        scanf("%d",&id);
                                                    };
                                                    system("cls");
                                                    bool t=rechercher_Arb(id,&r,0);
                                                    if (t) affichage_Archive2(nom_archive,id);
                                                    else {printf("\n --  Cet eleve n'appartient pas a l'ecole !                -- ");
                                                          printf("\n --  veuillez verifier la validite des donnees precedentes -- ");

                                                       };

                                                       printf("\n\n\n\n           Appuyer sur ENTRER pour Revenir au menu de l'archive ! \n\n\n ");
                                                       getchar();
                                                       getchar();



                                               break;
                                        }
                                        case(2):{         system("cls");
                                                          printf("\n\n #) Vous avez le choix de faire un listage des eleves ayant obtenus une moyenne superieure a une valeur precisee");
                                                          printf("\n---> veuillez entrer une valeur : ");
                                                          int valeur;
                                                          scanf("%d",&valeur);
                                                          system("cls");
                                                          printf("\n\n #) Lister selon  :  ");
                                                          printf("\n 1/ cette annee ");
                                                          printf("\n 2/ toutes les annees precedentes depuis la creation de l'archive");
                                                          printf("\n\n\---> veuillez entrer votre choix : ");
                                                          int temp;
                                                          scanf("%d",&temp);
                                                          if (temp==1){
                                                            A=ouvrir_Archive(nom_archive,'A');
                                                           lister_eleves_annee_courante(valeur);
                                                           fermer_Archive(A);
                                                          }
                                                          else {
                                                           lister_eleves(valeur,entete_Archive(A,2));
                                                          };
                                                          printf("\n\n\n\n           Appuyer sur ENTRER pour Revenir au menu de l'archive ! \n\n\n ");
                                                          getchar();
                                                           break;
                                                          }
                                        case(3):
                                            {
                                                system("cls");
                                                Transfert(nom_fich,nom_archive,cmp_annee);
                                                printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                getchar();
                                                getchar();
                                    break;
                                            }
                                        case(4): {
                                            system("cls");
                                            strcpy(old_file,nom_fich);
                                            do{
                                                printf("\n\n\n\n            Entrer un nom pour le fichier de la nouvelle annee ! ( pour sauvegarder l'ancien en cas ou vous on aurez besoin ! ) : ");
                                                scanf("%s",nom_fich);
                                                if (strcmp(old_file,nom_fich)==0)
                                                    printf("\n\n\n          ERREUR !!!! Le nom du nouveau ne peut pas etre le meme nom du ancien fichier !! vous allez perdre de donnees comme ca !!\n\n\n");
                                            }while(strcmp(old_file,nom_fich)==0);
                                            Chargement(old_file,nom_fich);
                                            cmp_annee++;// incrementer l'annee
                                            printf("\n\n\n\n           Appuyer sur ENTRER pour passer a l'annee suivante ! ! \n\n\n ");
                                            getchar();
                                            getchar();
                                            system("cls");
                                            passage=1;
                                            n_nonprep=0;
                                            break;
                                        }
                                        case(5):
                                        {
                                         retour5=1;
                                         break;
                                        }
//

               } }while(!retour5 && !passage);
               break;
              }
              case (7):
              {
                  arret=1;
                  printf("\n\n\n\n           Appuyer sur ENTRER pour QUITTER le programme ! \n\n\n ");
                  getchar();
                  getchar();
                  system("cls");
                  break;
              }
              }
              }while(!arret && cmp_annee<5);
            if (cmp_annee==5 ){
                printf("\nFin simulation .......................\n");
            }
            else{//cad , l'utilisateur a arreter le programme lui meme
                printf("\n\n\n\n            Vous allez quitter le programme ? Pas de soucie ,nous allons tout sauvegarder pour vous afin que vous puissiez revenir et manipuler le fichier plutard ! \n\n\n");
                printf("\n\n\n\n            L'operation de sauvegarde peut prendre quelque secondes, patintez SVP ...  \n\n\n");
                Sauvegarde_arb();
                Sauvegarde_index();
                if (cmp_annee>1)
                    Sauvegarde_indexA();
                printf("\n\n\n\n            WHOLAA ! L'operation de sauvegarde a ete faite avec succes !!  ...  \n\n\n");
                printf("                    $$$$ $$ $ $$$$ \n");
                printf("                    $  $  $ $ $    \n");
                printf("                    $$$   $$$ $$$$ \n");
                printf("                    $  $    $ $    \n");
                printf("                    $$$$  $$$ $$$$ \n\n\n");
            }
        }
          break;
        }
    case (1):
        {
            nouv_fich=1;
            system("cls||clear");
            printf("\n\n\n\n            --> Veuillez saisir le nom de votre ecole : ");
            scanf("%s",nom_ecole);
            printf("\n\n\n\n            --> Veuillez saisir l'annee courante  : ");
            scanf("%d",&annee_courante);
            printf("\n\n\n\n            --> Veuillez saisir le nom de votre fichier  : ");
            scanf("%s",nom_fich);
            printf("\n\n            Appuyer sur ENTRER pour commencer la simulation des 5 ans ! \n\n\n ");
            getchar();
            system("cls");
            printf("\n\n            Votre Fichier est en cours de creation , Veuillez patienter ... \n\n\n ");
            Chargement_init(nom_fich);
            printf("\n\n            Enfin ! Votre fichier a bien etait cree   !  \n");
            printf("\n\n            Appuyer sur ENTRER pour commencer la manipulation de votre fichier ! ! \n\n\n ");
            do{
                getchar();
                system("cls");
                menu_1();
                scanf("%d",&choix2);
                switch(choix2){
                case (1):
                    {
                        retour=0;
                        do{
                        system("cls");
                        printf("\n\n\n\n            Que voulez vous afficher ?  \n\n");
                        printf("              --------------------------------------------------------------------------\n");
                        printf("             |       1 ) ---> La liste des eleves dans l'ecole .                        |\n");
                        printf("             |       2 ) ---> Les matieres .                                            |\n");
                        printf("             |       3 ) ---> Retour au menu principal .                                |\n");
                        printf("              --------------------------------------------------------------------------\n\n");
                        printf("                   Votre choix : ");
                        scanf("%d",&choix3);
                            switch (choix3){
                            case(1):
                                {
                                    system("cls");
                                    Affichage1(nom_fich);
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    getchar();
                                    system("cls");
                                    printf("\n\n\n\n\n");
                                    break;
                                }
                            case(2):
                                {
                                    retour2=0;
                                    do{
                                    system("cls");
                                    printf("\n\n\n\n            Qu'elle liste de matieres voulez vous afficher  ?  \n\n");
                                    printf("              --------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Les matieres des 1eres et 2emes annees .                  |\n");
                                    printf("             |       2 ) ---> Les matieres des 3iemes,4iemes et 5iemes annee.           |\n");
                                    printf("             |       3 ) ---> Retour au menu de l'affichage .                           |\n");
                                    printf("              --------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix4);
                                    switch(choix4){
                                        case(1):
                                            {
                                                system("cls");
                                                int tn=(TeM-6)/2;
                                                Rech_matieres("11",matieres);
                                                printf("\n\n        Voici les matieres : \n");
                                                printf("        __________________________________________\n\n");
                                                for (int u=0;u<tn;u++){
                                                    printf("                --> %s .\n",matieres[u]);

                                                }
                                                printf("        __________________________________________\n\n");
                                                printf("\n\n            Appuyer sur ENTRER pour aller au menu des matieres ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                system("cls");
                                                printf("\n\n\n\n\n");
                                                break;

                                            }
                                        case (2):
                                            {
                                                system("cls");
                                                int tn=(TeM)/2;
                                                Rech_matieres("31",matieres);
                                                printf("\n\n        Voici les matieres : \n");
                                                printf("        __________________________________________\n\n");
                                                for (int u=0;u<tn;u++){
                                                    printf("                --> %s .\n",matieres[u]);

                                                }
                                                printf("        __________________________________________\n\n");
                                                printf("\n\n            Appuyer sur ENTRER pour aller au menu des matieres ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                system("cls");
                                                printf("\n\n\n\n\n");
                                                break;
                                            }
                                        case(3):{
                                            retour2=1;
                                            break;
                                        }
                                        }
                                    } while(!retour2);
                                    break;
                                }
                            case (3):
                                {
                                    retour=1;
                                    break;
                                }
                            }
                        }while (!retour);
                        break;
                    }
                            case(2):
                                {
                                    do{
                                            system("cls");
                                    printf("\n\n\n\n            Que voulez vous rechercher   ?  \n\n");
                                    printf("              --------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Savoir seulement si un eleve figure dans l'ecole .        |\n");
                                    printf("             |       2 ) ---> Les eleves d'une classe .                                 |\n");
                                    printf("             |       3 ) ---> Les eleves d'un niveau  .                                 |\n");
                                    printf("             |       4 ) ---> Le parcours d'un eleve dans cette annee .                 |\n");
                                    printf("             |       5 ) ---> Retour au menu principal .                                |\n");
                                    printf("              --------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix5);
                                    system("cls");
                                    switch (choix5){
                                    case(1):
                                        {
                                            system("cls");
                                            printf(" \n\n\n\n\n           # ) Entrer la classe de l'eleve : ");
                                            scanf("%s",classe);
                                            printf("            # ) Entrer le nom de l'eleve : ");
                                            scanf(" %[^\n]s",nom);
                                            printf("            # ) Entrer le prenom de l'eleve : ");
                                            scanf(" %[^\n]s",prenom);
                                            F=Ouvrir(nom_fich,'a');
                                            if (!Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch))
                                                printf("        - %s  %s ne figure pas dans la liste des eleves de %s .\n",nom, prenom,classe);
                                            else
                                                printf("        - %s  %s existe dans la liste des eleves de %s .\n",nom, prenom,classe);
                                            fermer(F);
                                            printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                            getchar();
                                            getchar();

                                            break;

                                        }
                                    printf("\n\n\n\n\n");
                                    break;
                                    case (2):
                                        {
                                            system("cls");
                                        printf("\n\n\n\n\n            # ) Entrer la classe : ");
                                        scanf("%s",classe);
                                        affiche_classe(nom_fich,classe);
                                        printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                        getchar();
                                        getchar();
                                        break;
                                    }
                                    case (3):
                                        {
                                            system("cls");
                                        printf("\n\n\n\n\n            # ) Entrer le niveau (p,1,2,3,4 ou 5): ");
                                        scanf(" %c",&nv);
                                        affiche_nv(nom_fich,nv);
                                        printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                        getchar();
                                        getchar();
                                        break;
                                    }
                                    case(4):
                                        {
                                        system("cls");
                                        printf("\n\n\n\n\n           # ) Entrer la classe de l'eleve : ");
                                        scanf("%s",classe);
                                        printf("            # ) Entrer le nom de l'eleve : ");
                                        scanf(" %[^\n]s",nom);
                                        printf("            # ) Entrer le prenom de l'eleve : ");
                                        scanf(" %[^\n]s",prenom);
                                        F=Ouvrir(nom_fich,'a');
                                        if (Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch)){
                                                if (classe[0]=='p' || classe[0]=='P')
                                                    tn=0;
                                                else{
                                                    if (classe[0]=='1' || classe[0]=='2' ){
                                                        tn=TeM-6;
                                                    }
                                                    else if (classe[0]=='3' || (classe[0]=='4' || classe[0]=='5') )
                                                        tn=TeM;
                                                    else
                                                        printf("\n              Erreur ! La classe n'existe pas !!!\n\n");
                                                }
                                                    getnotes(ch,tn,no);
                                                    id=getId(ch);
                                                    if (classe[0]!= 'p' && classe[0]!= 'P')
                                                        Rech_matieres(classe,matieres);
                                                    g=ch[strlen(nom)+strlen(prenom)+Tl+Tc+Tid];
                                                    printf("\n          ----------------------------\n");
                                                    printf("               Nom    :   %s           \n",nom);
                                                    printf("               Prenom :   %s           \n",prenom);
                                                    printf("               Classe :   %s           \n",classe);
                                                    printf("               Id     :   %d           \n",id);
                                                    printf("               genre  :   %c           \n",g);
                                                    printf("\n          ----------------------------\n");
                                                    if (tn!=0){
                                                    printf("            Voici les notes de %s %s dans les differentes matieres etudiees \n",nom,prenom);
                                                    for (int x=0;x<tn/2;x++){
                                                    printf("            %s  : %d  \n",matieres[x],no[x]);
                                                    }
                                                    }
                                                }
                                                else
                                                    printf("\n\n\n              Erreur ! L'eleve ne figure pas dans la liste des eleves de %s\n\n",classe);
                                                fermer(F);
                                                printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                getchar();
                                                getchar();
                                                break;
                                                }
                                                case (5):{
                                                    retour3=1;
                                                    break;
                                                }
                                    }
                                    } while (!retour3);
                                    break;
                                }
                                case (3):
                                {
                                    system("cls");
                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                    scanf("%s",classe);
                                    printf("            # ) Entrer le nom de l'eleve : ");
                                    scanf(" %[^\n]s",nom);
                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                    scanf(" %[^\n]s",prenom);
                                    do{
                                    printf("            # ) Entrer le genre de l'eleve : ");
                                    scanf(" %c",&g);
                                    if ((g!='m' && g!='M') && (g!='F'&& g!='f'))
                                        printf("\n\n\n         ERREUR!!!   Le genre n'est que (M ou m) ou (F ou f) !!!!\n\n\n");
                                    }while((g!='m' && g!='M') && (g!='F'&& g!='f'));
                                    if (classe[0]!='p' && classe[0]!='P' )
                                        Saisie_note(classe,note,no);
                                    else
                                        strcpy(note,"");
                                    if(!insert(nom_fich,classe,nom,prenom,note,g,no)){
                                        printf("\n\n\n\n\n          Erreur ! L'eleve %s %s existe deja !\n",nom,prenom);
                                    }
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    break;
                                }
                                case (4):
                                {
                                    system("cls");
                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                    scanf("%s",classe);
                                    printf("            # ) Entrer le nom de l'eleve : ");
                                    scanf(" %[^\n]s",nom);
                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                    scanf(" %[^\n]s",prenom);
                                    if(!Suppression(nom_fich,classe,nom,prenom)){
                                        printf("\n\n\n\n\n          Erreur ! L'eleve %s %s n'existe pas !\n",nom,prenom);
                                    }
                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                    getchar();
                                    break;
                                }
                                case (5):
                                    {
                                        retour4=0;
                                        do{
                                            system("cls");
                                            printf("\n\n\n\n            Que voulez vous mettre a jour    ?  \n\n");
                                            printf("              --------------------------------------------------------------------------\n");
                                            printf("             |       1 ) ---> Mise a jour d'une notes  .                                |\n");
                                            printf("             |       2 ) ---> Mise a jour d'un nom.                                     |\n");
                                            printf("             |       3 ) ---> Mise a jour d'une classe                                  |\n");
                                            printf("             |       4 ) ---> Retour au menu principal .                                |\n");
                                            printf("              --------------------------------------------------------------------------\n\n");
                                            printf("                   Votre choix : ");
                                            scanf("%d",&choix6);
                                            system("cls");
                                            switch (choix6){
                                            case (1):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer le nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    printf("            # ) Entrer le nom de la matiere : ");
                                                    scanf(" %[^\n]s",matiere);
                                                    do {printf("            # ) Entrer la nouvelle note sur 20 : ");
                                                    scanf(" %d",&not);
                                                    if (not>20)
                                                        printf("            \n Erreur ! La note ne peut pas depasser 20 !!\n");
                                                    } while(not>20);
                                                    if (MaJ_Notes(nom_fich,classe,nom,prenom,matiere,not))
                                                        printf("             La note a ete modifie ! vous pouvez consulter le parcours dans la recheche pour voir la modification !");
                                                     printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                     getchar();
                                                     getchar();
                                                     break;
                                                }
                                            case (2):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer la classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer l'ancien nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom_an);
                                                    printf("            # ) Entrer l'ancien prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom_an);
                                                    printf("            # ) Entrer le nouveau nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le nouveau prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    if (MaJ_Noms(nom_fich,classe,nom_an,prenom_an,nom,prenom))
                                                        printf("            Le nom et le prenom ont ete modifie ! vous pouvez consulter le parcours dans la recheche pour voir la modification !");
                                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                    getchar();
                                                    getchar();
                                                    break;
                                                }
                                            case (3):
                                                {
                                                    system("cls");
                                                    printf(" \n\n\n\n\n            # ) Entrer l'ancienne classe de l'eleve : \n");
                                                    scanf("%s",classe_an);
                                                    printf("             # ) Entrer la nouvelle classe de l'eleve : ");
                                                    scanf("%s",classe);
                                                    printf("            # ) Entrer le nom de l'eleve : ");
                                                    scanf(" %[^\n]s",nom);
                                                    printf("            # ) Entrer le prenom de l'eleve : ");
                                                    scanf(" %[^\n]s",prenom);
                                                    if (MaJ_classe(nom_fich,classe_an,nom,prenom,classe))
                                                        printf("            La classe a ete modifie ! vous pouvez consulter la liste des eleves pour voir la modification !");
                                                    printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                    getchar();
                                                    getchar();
                                                    break;
                                                }
                                            case (4):
                                                {
                                                    retour4=1;
                                                    break;
                                                }
                                            }
                                        }while(!retour4);
                                        break;
                                    }

                case(6):{ // archiver
                    if (nouv_fich){ // nouv archive
                        system("cls||clear");
                        do{
                            printf("\n\n            --> Veuillez saisir le nom de votre fichier archive : ");
                            scanf("%s",nom_archive);
                            if (strcmp(nom_archive,nom_fich)==0)
                                printf("\n\n\n          ERREUR !!!! Le nom de l'archive ne peut pas etre le meme nom du fichier !! vous allez perdre de donnees comme ca !!\n\n\n");
                        }while(strcmp(nom_archive,nom_fich)==0);
                        chargement_init_Archive(nom_archive);
                        printf("\n\n\n\n           -->Mission accomplie ! votre archive a ete cree avec succes :) !");
                        printf("\n\n            -->Merci  d'etre patientes :) ");
                        nouv_fich=0;
                    }
                    else{
                            MAJ_Archive(nom_archive);
                            printf("\n\n\n\n           -->Mission accomplie ! votre archive a ete modifiee avec succes :) !");
                            printf("\n\n            -->Merci  d'etre patientes :) ");
                    }
                        printf("\n\n\n\n           Appuyer sur ENTRER pour Commencer la manipulation de l'archive ! \n\n\n ");
                        getchar();
                        getchar();
                        system("cls");
                        bool retour5=0;
                        passage=0;
                        do{

                                    system("cls");
                                    printf("\n\n\n\n            Que voulez vous  faire   ?  \n\n");
                                    printf("              ----------------------------------------------------------------------------------------\n");
                                    printf("             |       1 ) ---> Afficher l'historique d'un eleve pendant toutes ses annees scolaires .  |\n");
                                    printf("             |       2 ) ---> Lister les eleves selon une moyenne seuille .                           |\n");
                                    printf("             |       3 ) ---> Un Transfert d'un etudiant   .                                          |\n");
                                    printf("             |       4 ) ---> Passer a l'annee prochaine   .                                          |\n");
                                    printf("             |       5 ) ---> Retour au menu principal .                                              |\n");
                                    printf("              ----------------------------------------------------------------------------------------\n\n");
                                    printf("                   Votre choix : ");
                                    scanf("%d",&choix8);
                                    system("cls");
                                    switch (choix8){
                                        case(1): // historique d'1 eleve
                                        {           system("cls");
                                                    printf("\n\n\n               #) Voulez vous faire un recherche par    :          \n");
                                                    printf("\n                    1/ NOM / PRENOM / CLASSE         \n");
                                                    printf("\n                    2/ IDENTIFIANT                   \n");
                                                    printf("\n\n                     Votre choix : ");
                                                    int u;
                                                    scanf("%d",&u);
                                                    system("cls");
                                                    if (u==1){
                                                        printf("\n               Nom    :              ");
                                                        scanf(" %[^\n]s",nom);
                                                        printf("\n               Prenom :              ");
                                                        scanf(" %[^\n]s",prenom);
                                                        printf("\n               Classe :              ");
                                                        scanf("%s",classe);
                                                        F=Ouvrir(nom_fich,'a');
                                                        if (Recherche(nom_fich,classe,nom,prenom,&b,&p,&pos,ch)){
                                                            id=getId(ch);
                                                        }
                                                        else
                                                            printf("\n\n\n          Erreur !! L'eleve %s %s n'existe pas dans la classe %s",nom,prenom,classe);
                                                        fermer(F);
                                                    }
                                                    else
                                                    {
                                                        printf("\n               Id     :              ");
                                                        scanf("%d",&id);
                                                    };
                                                    system("cls");
                                                    bool t=rechercher_Arb(id,&r,0);
                                                    if (t) affichage_Archive2(nom_archive,id);
                                                    else {printf("\n --  Cet eleve n'appartient pas a l'ecole !                -- ");
                                                          printf("\n --  veuillez verifier la validite des donnees precedentes -- ");

                                                       };

                                                       printf("\n\n\n\n           Appuyer sur ENTRER pour Revenir au menu de l'archive ! \n\n\n ");
                                                       getchar();
                                                       getchar();



                                               break;
                                        }
                                        case(2):{         system("cls");
                                                          printf("\n\n #) Vous avez le choix de faire un listage des eleves ayant obtenus une moyenne superieure a une valeur precisee");
                                                          printf("\n---> veuillez entrer une valeur : ");
                                                          int valeur;
                                                          scanf("%d",&valeur);
                                                          system("cls");
                                                          printf("\n\n #) Lister selon  :  ");
                                                          printf("\n 1/ cette annee ");
                                                          printf("\n 2/ toutes les annees precedentes depuis la creation de l'archive");
                                                          printf("\n\n\---> veuillez entrer votre choix : ");
                                                          int temp;
                                                          scanf("%d",&temp);
                                                          if (temp==1){
                                                            A=ouvrir_Archive(nom_archive,'A');
                                                           lister_eleves_annee_courante(valeur);
                                                           fermer_Archive(A);
                                                          }
                                                          else {
                                                           lister_eleves(valeur,entete_Archive(A,2));
                                                          };
                                                          printf("\n\n\n\n           Appuyer sur ENTRER pour Revenir au menu de l'archive ! \n\n\n ");
                                                          getchar();
                                                          getchar();
                                                           break;
                                                          }
                                        case(3):
                                            {
                                                system("cls");
                                                Transfert(nom_fich,nom_archive,cmp_annee);
                                                printf("\n\n            Appuyer sur ENTRER pour continuer ! \n\n\n ");
                                                getchar();
                                                getchar();
                                    break;
                                            }
                                        case(4): {
                                            system("cls");
                                            strcpy(old_file,nom_fich);
                                            do{
                                                printf("\n\n\n\n            Entrer un nom pour le fichier de la nouvelle annee ! ( pour sauvegarder l'ancien en cas ou vous on aurez besoin ! ) : ");
                                                scanf("%s",nom_fich);
                                                if (strcmp(old_file,nom_fich)==0)
                                                    printf("\n\n\n          ERREUR !!!! Le nom du nouveau ne peut pas etre le meme nom du ancien fichier !! vous allez perdre de donnees comme ca !!\n\n\n");
                                            }while(strcmp(old_file,nom_fich)==0);
                                            Chargement(old_file,nom_fich);
                                            cmp_annee++;// incrementer l'annee
                                            printf("\n\n\n\n           Appuyer sur ENTRER pour passer a l'annee suivante ! ! \n\n\n ");
                                            getchar();
                                            getchar();
                                            system("cls");
                                            passage=1;
                                            n_nonprep=0;
                                            break;
                                        }
                                        case(5):
                                        {
                                         retour5=1;
                                         break;
                                        }
//

               } }while(!retour5 && !passage);
               break;
              }
              case (7):
              {
                  arret=1;
                  break;
              }
              }
              }while(!arret && cmp_annee<5);
              system("cls");
            if (cmp_annee==5 ){
                printf("\n\n\n          ----->> Fin simulation <<-----      \n");
            }
            else{//cad , l'utilisateur a arreter le programme lui meme
                printf("\n\n\n\n            Vous allez quitter le programme ? Pas de soucie ,nous allons tout sauvegarder pour vous afin que vous puissiez revenir et manipuler le fichier plutard ! \n\n\n");
                printf("\n\n\n\n            L'operation de sauvegarde peut prendre quelque secondes, patientez SVP ...  \n\n\n");
                Sauvegarde_arb();
                Sauvegarde_index();
                if (cmp_annee>1)
                    Sauvegarde_indexA();
                printf("\n\n\n\n            WHOLAA ! L'operation de sauvegarde a ete faite avec succes !!  ...  \n\n\n");
                printf("                    $$$$ $$ $ $$$$ \n");
                printf("                    $  $  $ $ $    \n");
                printf("                    $$$   $$$ $$$$ \n");
                printf("                    $  $    $ $    \n");
                printf("                    $$$$  $$$ $$$$ \n\n\n");
            }
            printf("\n\n\n\n           Appuyer sur ENTRER pour QUITTER le programme ! \n\n\n ");
            getchar();
            getchar();
            system("cls");
            break;
        }
        case (3):{
              system("cls");
              printf("                    $$$$ $$ $ $$$$ \n");
              printf("                    $  $  $ $ $    \n");
              printf("                    $$$   $$$ $$$$ \n");
              printf("                    $  $    $ $    \n");
              printf("                    $$$$  $$$ $$$$ \n\n\n");

              printf("\n\n\n\n           Appuyer sur ENTRER pour QUITTER le programme ! \n\n\n ");
              getchar();
              getchar();
              system("cls");

        }
    }
    fclose(f2);
    fclose(f3);
    fclose(f1);

    return 0;
}
