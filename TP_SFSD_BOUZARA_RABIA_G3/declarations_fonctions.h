#ifndef DECLARATIONS_FONCTIONS_H_INCLUDED
#define DECLARATIONS_FONCTIONS_H_INCLUDED

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

// 0- Les Constantes  :
// --------------------
#define Max 999 // taille d'un bloc
#define Tl 4    //taille de longueur ( Nom+prenom+tableau de notes )
#define Tid 5   //taille du numéro d'identifacation
#define Tc 3  //taille du numéro de classe
#define TeM 16 //taille max du tableau de notes ( il peut etre 10 , on va regler le probleme avec la taille )
#define TNf 30  //taille du nom de fichier
#define max_str 30 // taille maximale d'un nom/prenom
#define max_Id 10000 //id sur 4 car
#define nb_names 600 //nombre de nom dans le fichier
#define max_index 50
#define max_e 100 // le max de la chaine
#define Max_eleve 600 // le max des eleves
#define max_classe 30 // le max d'eleve dans une classe
#define Note 20 // la note max
#define max_tof 50 // le max de la table d'index tof
#define max_tof2 500 // le max d'un bloc de l'arbre
#define Max_arb 3000
#define t_p_max 120
#define max_file 30
#define max_str2 15
#define A_MAX 25
#define nb_annee 6
#define max_a 100
#define n_ans 5

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/



// 1- Déclarations et Défintions des structures du fichier initial:
// ----------------------------------------------------------------

typedef struct Tbloc_TOVC   //Structure d'un bloc
{
    char tab[Max+1];// pour \0
    int pos_libre;
}Tbloc_TOVC;

//***********************************************

typedef struct Enreg    //la structure de l'enregistrement dans lequel on va mettre des infos avant d'inserer la donnée
{
    char taille[Tl];
    char Num_Id[Tid];
    char eff;
    char num_classe[Tc];
    char Nom[max_str];
    char Prenom[max_str];
    char genre;
    char Notes[TeM];
}Enreg;

//***********************************************

typedef struct type_entete //Structure de l'entete
{
    int num_dernier ;       //numéro du dernier bloc
    int nb_car_inser;       //nombre de caracteres inserés
    int nb_car_suppr;       //nombre de caractere supprimés
}   type_entete ;

//***********************************************

typedef struct TOVC // définition du fichier TOVC
{
    FILE * f ;
    type_entete entete ;
}   TOVC ;

// 2- Déclarations et Défintions des structures du fichier Archive:
// ----------------------------------------------------------------
// Structure d'un enregistrement dans le fichier d'archivage
typedef struct Archive_Enreg{
    int id;
    char nom[max_str2];
    char prenom[max_str2];
    float moyennes[nb_annee];
    int annee_scolaire; //preparatoire , 1ere , 2eme ......
}A_Enreg;

// Structure d'un bloc du fichier archivage
typedef struct Archive_bloc{
 A_Enreg tab[A_MAX];
 int NB;
}A_bloc;

typedef struct Archive_entete {
int nb_eleve;//compteur d'insertions
}A_entete;

typedef struct Archive_fichier{
  FILE* f;
  A_entete entete;
}A_TOF;

/***  Index de l'archive  ***/
typedef struct A_indice{
int id_min;       // identifiant min
int id_max;       // identifiant min
float moy_min[n_ans]; // les moyennes min correspandantes a l'annee
float moy_max[n_ans]; // les moyennes max correspandantes a l'annee
}A_indice;
typedef struct Archive_index {
A_indice tab[max_a]; // on discute la taille !
int NB;
}Archive_index;

// 3- Déclarations et Défintions de la table d'index du fichier initial:
// ---------------------------------------------------------------------


typedef struct Index_enreg {  // Structure d'un enreg de fich Index non dense
     char classe[Tc];
     char min[max_str]; // premier nom dans la classe
     char max[max_str]; // dernier nom dans la classe (ordonnées alphabétiquement)
     int bmin; // bloc minimale de la classe
     int bmax; // bloc max
     int pmin; // position minimale (dans bmin)
     int pmax; // position maximale (dans bmax)
} In_enreg;
// tableau Index :
 typedef struct index {
     In_enreg T[max_index];
 } Index;


// 4- Déclarations et Défintions de la table d'index des étudiants utilisée dans le chargement intial :
// ----------------------------------------------------------------------------------------------------

 typedef struct etudiant{ //Le tableau d'index d'etudiants , juste pour le chargement
    char Nom_cmplt[max_str*2];
    int b;
    int p;
}etudiant;


// 5- Déclaration et Défintion de la table des nouveaux élèves préscolaires , sera utilisée dans l'achivage :
// ----------------------------------------------------------------------------------------------------------

typedef struct tab_prepa{
    int id;
}tab_p;
tab_p t_prepa[t_p_max];


// 6- Déclaration et Défintion de l'arbre qui sera utilisée pour assurer l'unicité des Identifiants ainsi que l'achivage :
// -----------------------------------------------------------------------------------------------------------------------

typedef struct noeud {
		int id; //numéro d’identification
		char nom[max_str2];
		char prenom[max_str2];
		int annee_scolaire;
		float moyenne;
		bool supp;
		struct noeud *fg;
		struct noeud *fd;
	} TNoeud;


// 7- Déclaration et Défintion du fichier Index:
// ---------------------------------------------

typedef struct enreg_index {
    char classe[Tc];
    char min[max_str*2];
    char max [max_str*2];
    int bmin;
    int bmax;
    int pmin;
    int pmax;
}enreg_index;
typedef struct tbloc_TOF{
    enreg_index tab[max_tof];
}tbloc_TOF;
typedef struct t_entete{
    int nb_bloc;
    int nb_enreg;
}t_entete;
typedef struct TOF{
    FILE *f;
    t_entete entete;
}TOF;


// 8- Déclaration et Défintion du fichier Arbre:
// ---------------------------------------------

typedef struct enreg_arb {
        int id; //numéro d’identification
		char nom[max_str2];
		char prenom[max_str2];
		int annee_scolaire;
		float moyenne;
		bool supp;
}enreg_arb;
typedef struct tbloc_TOF2{
    enreg_arb tab[max_tof2];
}tbloc_TOF2;
typedef struct t_entete2{
    int nb_bloc;
    int nb_enreg;
}t_entete2;
typedef struct TOF2{
    FILE *f;
    t_entete2 entete;
}TOF2;

// 9- Déclaration et Défintion de la table qui représente l'arbre :
// ----------------------------------------------------------------

typedef struct enr{
        int id; //numéro d’identification
		char nom[max_str];
		char prenom[max_str];
		int annee_scolaire;
		float moyenne;
		bool supp;
}enr;

//structure du tableau qui cherche la matiere (sera utilisé dans la MaJ)
typedef struct tab_ch{
    char matiere[max_str];
}tab_ch;

// 10- Les Variables Globales:
// ---------------------------
//--------------------------------------------

// Déclaration  fichiers :
//------------------------

TOVC *F; // fichier principal
TOVC *F1; // fichier de l'annee precedente
FILE *f1; // fichier des noms
FILE *f2; // fichier des prenoms
FILE *f3; // fichier des matiéres
TOF *f_index;// fichier d'index des classes
TOF2 *f_arb;// fichier d'index des classes
A_TOF *A; //fichier archive
//--------------------------------------------

// Déclaration  Buffers :
//------------------------

Tbloc_TOVC buf;
Tbloc_TOVC buf2;

tbloc_TOF buf_index;
tbloc_TOF2 buf_arb;

A_bloc A_buf;
A_bloc A_buf2;
//--------------------------------------------

//les deux tables d'index
//-----------------------

Index tab_index;
int Nbe=0;
Index tab_index2;
int Nbe2;
Archive_index A_Index ;
//--------------------------------------------

// racine de l'arbre
//------------------

TNoeud *Id_tree = NULL;
//--------------------------------------------

// table d'index des étudiants
//----------------------------

int nb=0; // nb d'elements dans la table d'index des eleves d'une classe
etudiant index_classe[max_classe] ; //declaration de la structure
//--------------------------------------------

// table des prépas
//-----------------
tab_p Id_prep[t_p_max];
int nb_prepa=0;


enr tab_arb[Max_arb];// tableau de l'arbre pour mettre les valeur de l'arbre dedans pour faciliter le chargement et la sauvegarde
int nb_eleve;
int ia=0;

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

// 11- La machine abstraite du fichier initial  :
// ----------------------------------------------

TOVC* Ouvrir(char nom[TNf],char mode);
void fermer (TOVC *p);
void lireDir(TOVC *p ,int i,Tbloc_TOVC *buf);
void ecrireDir(TOVC *p,int i,Tbloc_TOVC *buf);
int entete(TOVC *p,int i);
void aff_entete(TOVC *p,int i,int nb);

// 12- La machine abstraite du fichier archive  :
// ----------------------------------------------

A_TOF* ouvrir_Archive(char nom[max_str],char mode);
void fermer_Archive (A_TOF *p);
void lireDir_Archive(A_TOF *p ,int i,A_bloc *buf);
void ecrireDir_Archive(A_TOF *p,int i,A_bloc *buf);
int entete_Archive(A_TOF *p);
void aff_entete_Archive(A_TOF *p,int nb);
void affichage_Archive(char name[max_str]);

// 13- La machine abstraite de l'arbre  :
// --------------------------------------

#define CreerNoeud()  malloc( sizeof(TNoeud) )
#define LibererNoeud(p) free((p))
#define Aff_Info(p,v) (p)->id = (v)
#define Aff_nom(p,v) strcpy((p)->nom,&v)
#define Aff_prenom(p,v) strcpy((p)->prenom,&v)
#define AFF_annee(p,v) (p)->annee_scolaire = (v)
#define Aff_moyenne(p,v) (p)->moyenne = (v)
#define Aff_fg(p,q)  (p)->fg = (q)
#define Aff_fd(p,q)  (p)->fd = (q)
#define Aff_Supp(p,v) (p)->supp = (v)
#define Info(p)  (p)->id
#define Nom(p) (p)->nom
#define Prenom(p) (p)->prenom
#define Moyenne(p) (p)->moyenne
#define Annee_scolaire(p) (p)->annee_scolaire
#define Supp(p) (p)->supp
#define fg(p)  (p)->fg
#define fd(p)  (p)->fd

// 14- La machine abstraite du fichier index  :
// --------------------------------------------

TOF* ouvrir2(char nom[30],char mode);
void fermer2 (TOF *p);
void lireDir2(TOF *p ,int i,tbloc_TOF *buf);
void ecrireDir2(TOF *p,int i,tbloc_TOF *buf);
int entete2(TOF *p,int i);
void aff_entete2(TOF *p,int i,int nb);


// 15- La machine abstraite du fichier arbre  :
// --------------------------------------------

TOF2* ouvrir3(char nom[30],char mode);
void fermer3 (TOF2 *p);
void lireDir3(TOF2 *p ,int i,tbloc_TOF2 *buf);
void ecrireDir3(TOF2 *p,int i,tbloc_TOF2 *buf);
int entete3(TOF2 *p,int i);
void aff_entete3(TOF2 *p,int i,int nb);


// 16- Les Opérations   :
// ----------------------
// 16.1- Les opérations sur l'arbre :
// ----------------------------------
// on va utilisé un arbre de recherche binaire pour sauvegarder les clés et pour l'exploiter plutard dans la création et la mise a jour de l'archive
//--------------------------------------------------------------------------------------------------------------------------------------------------


///a ) la recherche dans l'arbre binaire:
///-------------------------------------
bool rechercher_Arb( int id ,TNoeud **r ,bool Afficher ); // rechercher le num_id dans l'arbre , et retourner l'@ de cette neoud
///b ) L'insertion dans l'abre binaire :
///------------------------------------

void inserer_Arb( int val,char *nom[20],char *prenom[20],int annee_scolaire,float moyenne,bool supp,TNoeud **r );	// insertion récursive dans l'arbre


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 1 ) Chargement initial                                                  */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
float calcule_moyenne(int no[TeM/2],int tn);
Enreg Remplissage(int i);
int Random(int a);
void Rech_F(FILE *f,int n,char ch[max_str]);
void ExtractName(char NameGend[max_str],char Name[max_str]);
int ExtractGender (char NameGend[max_str]);
void GetMarks(char notes[TeM],int Tn,int no[TeM/2]);
void Rech_pos(Enreg e,char lc[Tc],char classe[Tc],int *b,int *p,int b1,int p1,bool *decalage,int *pos,int *pos2);
void Rech_Index ( int c, char min[2*max_str], char max[2*max_str], int *bmin, int *pmin, int *bmax, int *pmax,int *pos);
void Rech_pos_index(char fname[max_str*2],char lc[Tc],char classe[Tc],int b1,int p1,int *b,int *p,int *pos,bool *decalage,char max[max_str*2]);
void GetFullName(Enreg e,char *ch[max_str]);
void getLongueur(char ch[max_e],int *longueur);
void getName(char ch[max_e],char FN[max_str*2] );
void Insertion (Enreg e,int b,int p,bool decalage,int pos ,char lc[Tc],char classe[Tc],int *b1,int *p1,int pos2);
void ecrirechaine(char ch[max_e],int *b,int *p);
void Chargement_init(char nom_fichier[max_file]);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                        ( 2 ) Recherche                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

bool Recherche(char nom_fichier[max_file],char classe[Tc],char nom [max_str],char prenom [max_str],int *b,int *p, int *pos,char ch[max_e]);
void LireChaine(int i,int j,char ch[max_e]);
void getEff(char ch[max_e],bool *eff);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                        ( 3 ) Insertion                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

Enreg remplir(char classe[Tc],char nom[max_str],char prenom[max_str],char note[TeM],char genre,int not[TeM/2]);
void insert(char nom_fichier[max_file], char classe[Tc],char nom[max_str],char prenom[max_str],char note[TeM],char genre);
void Rech_matieres(char classe[Tc],tab_ch matieres[TeM/2]);
void Saisie_note(char classe[Tc],char note[TeM],int not[TeM/2]);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 4 ) Mise à Jour                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
// 1 ) Mise à Jour des notes
//--------------------------
void MaJ_Notes(char nom_fichier[max_file],char classe[Tc],char Nom [max_str],char Prenom[max_str],char matiere[max_str],int note);
int num_matiere(char matiere[],char classe[Tc]);
int getId(char ch[max_e]);
void getnotes(char ch[max_e],int tn,int no[TeM/2]);

// 2 ) Mise à Jour des noms
//-------------------------

void MaJ_Noms(char nom_fichier[max_file],char classe[Tc],char Nom [max_str],char Prenom[max_str],char Nom_nv [max_str],char Prenom_nv[max_str]);
void modifier_nom(char ch[max_e],char Nom[max_str],char Prenom[max_str],char classe[Tc],char Nom_an[max_str],char Prenom_an[max_str]);
void inserer (char ch[max_e],int b,int p,int pos,char nom[max_str],char prenom[max_str]);

// 3 ) Mise à Jour des classes
//----------------------------

void MaJ_classe(char nom_fichier[max_file],char classe_an[Tc],char Nom [max_str],char Prenom[max_str],char classe_nv[Tc]);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 5 ) Suppression                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
bool Suppression (char nom_fichier[max_file], char classe[Tc],char nom[max_str],char prenom[max_str]);
void Suppr (char nom_fichier[max_file],char ch[max_e],int b,int p,int pos,char nom[max_str],char prenom[max_str]);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 6 ) Affichage                                                           */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void Affichage1(char nom_fichier[max_file]);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                 ( 7 ) Sauvegardes et Chargements                                               */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// ( 1 )- Sauvegarde et Chargement de l'index :
void Sauvegarde_index();
void Chargement_index();

// ( 2 )- Sauvegarde et Chargement de l'arbre :
void inordre( TNoeud *r );
void Sauvegarde_arb();
void Chargement_arb();


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                 ( 8 ) Chargement ( Non Initial )                                               */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void MaJ_As_arb(TNoeud *p);
void keep_index();
void Inscription_prepas(char fichier_new[max_file]);
void LireChaine2(int *i,int *j,char ch[max_e]);
void modifier(char ch[max_e]);
void Passage(char fichier_old[max_file],char fichier_new[max_file]);
void Chargement(char fichier_old[max_file],char fichier_new[max_file]);


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                   ( 9 ) Réorganisation                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void Reorganisation (char old_file [max_file],char new_file[max_file]);
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                       ( 10 ) Archive                                                           */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void chargement_init_Archive(char nomf[30]); //Faire le chargement initiale de l'archive a partir un arbre pres remplit
void Inordre_Archive(TNoeud *p,int *i,int *j, float *moy_min , float *moy_max);//parcour et traitement inordre de l'arbre pour remlpir l'archive
void MAJ_Archive(int anne);
void MAJ_Les_anciens(TNoeud *p,int *i,int *j,int *anne);//parcour et traitement inordre de l'arbre pour remlpir l'archive
void Rech_A_Index(int id,int *i, int *j);
void Inserer_Archive(int id,int i,int j,int anne);


#endif // DECLARATIONS_FONCTIONS_H_INCLUDED
