#include "dec_functions.h"
#include<stdio.h>
#include<stdbool.h>
// 1- La machine abstraite du fichier initial  :
// ---------------------------------------------
//**Ouvrir*****************************************************
TOVC* Ouvrir(char nom[TNf],char mode){
    TOVC *p= malloc(sizeof(TOVC));
    if (mode=='a'|| mode=='A'){
        p->f=fopen(nom,"rb+");
        if (p->f==NULL) perror("erreur");
        fread(&(p->entete),sizeof(type_entete),1,p->f);
    }else //le mode est n ou N; nouveau fichier
    {
        p->f=fopen(nom,"wb+");
        if (p->f==NULL) perror ("erreur");
        (p->entete).num_dernier=0;
        (p->entete).nb_car_inser=0;
        (p->entete).nb_car_suppr=0;

        fwrite(&(p->entete),sizeof(type_entete),1,p->f);
    }
    return p;
}

//**Fermer*****************************************************
//fermer le fichier et sauvegarder l'entete
void fermer (TOVC *p){
    fseek(p->f,0,SEEK_SET);
    fwrite(&(p->entete),sizeof(type_entete),1,p->f);
    fclose(p->f);
}

//**LireDir****************************************************
//lecture d'un bloc dans un buffer
void lireDir(TOVC *p ,int i,Tbloc_TOVC *buf){
    fseek(p->f,sizeof(type_entete)+(i-1)*sizeof(Tbloc_TOVC),SEEK_SET);
    fread(buf,sizeof(Tbloc_TOVC),1,p->f);
}

//**EcrireDir**************************************************
//ecriture d'un buffer dans un bloc

void ecrireDir(TOVC *p,int i,Tbloc_TOVC *buf){
    fseek(p->f,sizeof(type_entete)+sizeof(Tbloc_TOVC)*(i-1),SEEK_SET);
    fwrite(buf,sizeof(Tbloc_TOVC),1,p->f);
}

//**entete*****************************************************
//me donne la caracteristique numero i de l'entete
int entete(TOVC *p,int i){
    switch (i){
        case 1 :{
             return ((p->entete).num_dernier);//retourne le numéro du dernier bloc
             break;
        }
        case 2 :{
             return ((p->entete).nb_car_inser);//retourne le nombre de caractere inseres
             break;
        }
        case 3 :{
             return ((p->entete).nb_car_suppr);//retourne le nombre de caracteres supprimés
             break;
        }
    }
}

//**aff_entete*************************************************

// affecter le valeur nb, dans le champs i de l'entete
void aff_entete(TOVC *p,int i,int nb){
    switch (i){
        case 1:{
            p->entete.num_dernier=nb;
            break;
        }
        case 2 :{
            p->entete.nb_car_inser=nb;
            break;
        }
        case 3 :{
            p->entete.nb_car_suppr=nb;
            break;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
// 2- La machine abstraite du fichier archive  :
// ----------------------------------------------
A_TOF* ouvrir_Archive(char nom[30],char mode){
    A_TOF *p= malloc(sizeof(A_TOF));
    if (mode=='a'|| mode=='A'){
        p->f=fopen(nom,"rb+");
        if (p->f==NULL) perror("erreur");
        fread(&(p->entete),sizeof(A_entete),1,p->f);
    }else
    {
        p->f=fopen(nom,"wb+");
        if (p->f==NULL) perror ("erreur");
        (p->entete).nb_eleve=0;
        fwrite(&(p->entete),sizeof(A_entete),1,p->f);
    }
    return p;
}

void fermer_Archive (A_TOF *p){
    fseek(p->f,0,SEEK_SET);
    fwrite(&(p->entete),sizeof(A_entete),1,p->f);
    fclose(p->f);
}

void lireDir_Archive(A_TOF *p ,int i,A_bloc *buf){
    fseek(p->f,sizeof(A_entete)+(i-1)*sizeof(A_bloc),SEEK_SET);
    fread(buf,sizeof(A_bloc),1,p->f);
}

void ecrireDir_Archive(A_TOF *p,int i,A_bloc *buf){
    fseek(p->f,sizeof(A_entete)+sizeof(A_bloc)*(i-1),SEEK_SET);
    fwrite(buf,sizeof(A_bloc),1,p->f);
}

int entete_Archive(A_TOF *p){
             return ((p->entete).nb_eleve);
}

void aff_entete_Archive(A_TOF *p,int nb){
             p->entete.nb_eleve=nb;
}

void affichage_Archive(char name[30]){
    A=ouvrir_Archive(name,'A');
    int b=(entete_Archive(A)-1)/(A_MAX+1)+1; // b == NB bloc
    float T[6];
    for (int i=1;i<=b;i++){
        lireDir_Archive(A,i,&A_buf);
        for (int k=0;k<A_buf.NB;k++){
            for(int s=0;s<6;s++){
             if (s<A_buf.tab[k].annee_scolaire){
                T[s]=A_buf.tab[k].moyennes[s];
            }
            else T[s]=0;
            };
            printf("\n L'identifiant : %d",A_buf.tab[k].id);
            printf("\n Le nom complet : %s %s",A_buf.tab[k].nom,A_buf.tab[k].prenom);
            printf("\n Tableau des moyennes : %f | %f | %f | %f | % f ",T[0],T[1],T[2],T[3],T[4]);
        }
    }
    fermer_Archive(A);
}

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/



// 3- Les opérations sur l'arbre :
// -------------------------------
// on va utilisé un arbre de recherche binaire pour sauvegarder les clés et pour l'exploiter plutard dans la création et la mise a jour de l'archive
//--------------------------------------------------------------------------------------------------------------------------------------------------


///a ) la recherche dans l'arbre binaire:
///-------------------------------------
bool rechercher_Arb( int id ,TNoeud **r ,bool Afficher ) // rechercher le num_id dans l'arbre , et retourner l'@ de cette neoud
{
   int trouv = 0 ;
    TNoeud *p; p=Id_tree;
   while ( !trouv && p != NULL )
	if ( Info(p) == id ) {trouv = 1;
	 *r=p;
	 if (Afficher){
     printf("\nId             : %d",Info(p));
     printf("\nNom            : %s",Nom(p));
     printf("\nPrenom         : %s",Prenom(p));
     printf("\nMoyenne        : %f",Moyenne(p));
     printf("\nSupp           : %d",Supp(p));
     printf("\nAnnee_scolaire : %d",Annee_scolaire(p));
     printf("\nFG             : %d",fg(p));
     printf("\nFd             : %d",fd(p));

     };


}

	else {
	   if ( id < Info(p) )
		p = fg(p);
	   else
		p = fd(p);
	}
return trouv;
}


//******************************************************

///b ) L'insertion dans l'abre binaire :
///------------------------------------

void inserer_Arb( int val,char *nom[20],char *prenom[20],int annee_scolaire,float moyenne,bool supp,TNoeud **r )	// insertion récursive dans l'arbre
{
   TNoeud *n;
   int v, b;

   if ( *r == NULL ) {
	*r = CreerNoeud();
	 Aff_Info(*r,val);
	 Aff_nom(*r,*nom);
	 Aff_prenom(*r,*prenom);
	 AFF_annee(*r,annee_scolaire);
	 Aff_moyenne(*r,moyenne);
	 Aff_Supp(*r,supp);
	 Aff_fg(*r,NULL);
	 Aff_fd(*r,NULL);
   }
   else {
	if ( val < Info(*r) ) {
	   n = fg(*r);
	   inserer_Arb(val,nom,prenom,annee_scolaire,moyenne,supp,&n);
	   Aff_fg( *r, n );
     	}
	else // if ( val < Info(*r) )
	   if ( val > Info(*r) ) {
       n = fd( *r );
       inserer_Arb(val,nom,prenom,annee_scolaire,moyenne,supp,&n);
       Aff_fd( *r, n );
	   }  // if ( val > Info(*r) )
   }}

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

// 4- La machine abstraite du fichier index  :
// --------------------------------------------

TOF* ouvrir2(char nom[30],char mode){
    TOF *p= malloc(sizeof(TOF));
    if (mode=='a'|| mode=='A'){
        p->f=fopen(nom,"rb+");
        if (p->f==NULL) perror("erreur");
        fread(&(p->entete),sizeof(t_entete),1,p->f);
    }else //le mode est n ou N; nouveau fichier
    {
        p->f=fopen(nom,"wb+");
        if (p->f==NULL) perror ("erreur");
        (p->entete).nb_bloc=0;
        (p->entete).nb_enreg=0;
        fwrite(&(p->entete),sizeof(t_entete),1,p->f);
    }
    return p;
}
//fermer le fichier et sauvegarder l'entete
void fermer2 (TOF *p){
    fseek(p->f,0,SEEK_SET);
    fwrite(&(p->entete),sizeof(t_entete),1,p->f);//ici on a sauvegarder l'entete
    fclose(p->f);
}
//lecture d'un bloc dans un buffer
void lireDir2(TOF *p ,int i,tbloc_TOF *buf){
    fseek(p->f,sizeof(t_entete)+(i-1)*sizeof(tbloc_TOF),SEEK_SET);
    fread(buf,sizeof(tbloc_TOF),1,p->f);
}
//ecriture d'un buffer dans un bloc

void ecrireDir2(TOF *p,int i,tbloc_TOF *buf){
    fseek(p->f,sizeof(t_entete)+sizeof(tbloc_TOF)*(i-1),SEEK_SET);
    fwrite(buf,sizeof(tbloc_TOF),1,p->f);
}
//me donne la caracteristique numero i de l'entete
int entete2(TOF *p,int i){
    switch (i){
        case 1 :{
             return ((p->entete).nb_bloc);//retourne le nombre de blocs dans tout le fichier.
             break;
        }
        case 2 :{
             return ((p->entete).nb_enreg);//retourne le nombre de blocs dans tout le fichier.
             break;
        }
    }
}
// affecter le valeur nb, dans le champs i de l'entete
void aff_entete2(TOF *p,int i,int nb){
    switch (i){
        case 1:{
            p->entete.nb_bloc=nb;
            break;
        }
        case 2 :{
            p->entete.nb_enreg=nb;
        }
    }
}

// 5- La machine abstraite du fichier arbre  :
// --------------------------------------------

TOF2* ouvrir3(char nom[30],char mode){
    TOF2 *p= malloc(sizeof(TOF2));
    if (mode=='a'|| mode=='A'){
        p->f=fopen(nom,"rb+");
        if (p->f==NULL) perror("erreur");
        fread(&(p->entete),sizeof(t_entete2),1,p->f);
    }else //le mode est n ou N; nouveau fichier
    {
        p->f=fopen(nom,"wb+");
        if (p->f==NULL) perror ("erreur");
        (p->entete).nb_bloc=0;
        (p->entete).nb_enreg=0;
        fwrite(&(p->entete),sizeof(t_entete2),1,p->f);
    }
    return p;
}
//fermer le fichier et sauvegarder l'entete
void fermer3 (TOF2 *p){
    fseek(p->f,0,SEEK_SET);
    fwrite(&(p->entete),sizeof(t_entete2),1,p->f);//ici on a sauvegarder l'entete
    fclose(p->f);
}
//lecture d'un bloc dans un buffer
void lireDir3(TOF2 *p ,int i,tbloc_TOF2 *buf){
    fseek(p->f,sizeof(t_entete2)+(i-1)*sizeof(tbloc_TOF2),SEEK_SET);
    fread(buf,sizeof(tbloc_TOF2),1,p->f);
}
//ecriture d'un buffer dans un bloc

void ecrireDir3(TOF2 *p,int i,tbloc_TOF2 *buf){
    fseek(p->f,sizeof(t_entete2)+sizeof(tbloc_TOF2)*(i-1),SEEK_SET);
    fwrite(buf,sizeof(tbloc_TOF2),1,p->f);
}
//me donne la caracteristique numero i de l'entete
int entete3(TOF2 *p,int i){
    switch (i){
        case 1 :{
             return ((p->entete).nb_bloc);//retourne le nombre de blocs dans tout le fichier.
             break;
        }
        case 2 :{
             return ((p->entete).nb_enreg);//retourne le nombre de blocs dans tout le fichier.
             break;
        }
    }
}
// affecter le valeur nb, dans le champs i de l'entete
void aff_entete3(TOF2 *p,int i,int nb){
    switch (i){
        case 1:{
            p->entete.nb_bloc=nb;
            break;
        }
        case 2 :{
            p->entete.nb_enreg=nb;
        }
    }
}
// 6- Les Opérations   :
// ---------------------

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 1 ) Chargement initial                                                  */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//*-*-*-*-*-*-*-*-*-*
//1.a ) REMPLISSAGE |
//*-*-*-*-*-*-*-*-*-*
// cette procedure assure le remplissage des enregistrements e selon les critéres proposées.


float calcule_moyenne(int no[TeM/2],int tn){
    int s=0;
    int tot=tn/2;
    for(int t=0;t<(tn/2);t++){
        s+=no[t];
    }
    float moy = s/(float)tot;
    return moy;
}

Enreg Remplissage(int i){
    int Id,Annee,Salle,N1,N2,N3,Tn,lon;
    Enreg e;
    TNoeud *p;
    char FName[max_str]="";
    char Name[max_str]="";
    char Gender;
    char ch[max_str]="";
    char ch1[max_str]="";
    char NameGend[max_str]="";
    e.eff='0';
    Annee=i/100;
    Salle= ((i%100)/25)+1;
    char cara1,cara2;
    if (Annee==0)
        cara1='p';
    else
        cara1=Annee+'0';
    cara2=Salle+'0';
    e.num_classe[0]=cara1;
    e.num_classe[1]=cara2;
    e.num_classe[2]='\0';
    bool stop=0;
    while (!stop){
        Id=Random(max_Id);
        if (!(rechercher_Arb(Id,&p,0))){
            stop=1;
        }
    }
    sprintf(e.Num_Id,"%d",Id);
    if (Id<10 &&Id>=0){
        strcpy(ch,"000");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<100 && Id >=10){
        strcpy(ch,"00");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<1000 && Id >=100){
        strcpy(ch,"0");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    N1=Random(nb_names)+1;
    N2=Random(nb_names)+1;
    Rech_F(f1,N1,NameGend);
    Rech_F(f2,N2,FName);
    ExtractName(NameGend,Name);
    Gender=ExtractGender(NameGend);
    strcpy(e.Nom,FName);
    strcpy(e.Prenom,Name);
    e.genre=Gender;
    int no[TeM/2];
    strcpy(e.Notes,"");
    if (Annee==0)
        Tn=0;
    else{
        if (Annee==1 || Annee==2)
            Tn=TeM -6; //4 signifie 3 matieres a enlever , anglais français et hisroire geo
        else
            Tn=TeM;
        GetMarks(e.Notes,Tn,no);
    }
    float moy;

    e.Notes[Tn]='\0';
    lon=strlen(Name)+strlen(FName)+1+Tn+2;//1:genre;2:les deux caracteres speciaux entre nom-prenom et prenom-genre
    e.taille[0]=lon /100 +'0';
    e.taille[1]=(lon /10)%10 +'0';
    e.taille[2]=lon %10 +'0';
    e.taille[3]='\0';
    moy=calcule_moyenne(no,Tn);
    bool supp=0;
    inserer_Arb(Id,e.Nom,e.Prenom,Annee,moy,supp,&Id_tree);
    return e;
}

//Les sous fonctions de REMPLISSAGE:
//----------------------------------
///*-*-*-*-*-*
///a ) RANDOM:
///*-*-*-*-*-*

//Genere un nombre aleatoire

int Random(int a) // fonction qui retourne un entier aleatoire compris entre 0 et a
{
    srand(time(NULL)+rand());
    return (rand()%a);
}

///*-*-*-*-*-*
///b ) RECH_F:
///*-*-*-*-*-*

//cette procedure rechereche le n'ieme element dans le fichier

void Rech_F(FILE *f,int n,char ch[max_str]){
    bool trouv=0;
    int cmp=0;
    char cr;
    strcpy(ch,"");
    while (!trouv){
        if (cmp == n-1){
            trouv=1;
            while ((cr=fgetc(f)) != ';' && cr != EOF){
                  strncat(ch,&cr,1);

            }

        }
        else{
            if (fgetc(f) == ';') cmp++;
        }
    }
    fseek(f,0,SEEK_SET);

}

///*-*-*-*-*-*-*-*-
///c ) EXTRACTNAME:
///*-*-*-*-*-*-*-*-

//cette procedure donne la partie prenom

void ExtractName(char NameGend[max_str],char Name[max_str]){
    int i=0;
    char cr;
    strcpy(Name,"");
    while ((cr=NameGend[i])!=','){
        strncat(Name,&cr,1);
        i++;
    }
}

///*-*-*-*-*-*-*-*-*-
///d ) EXTRACTGENDER:
///*-*-*-*-*-*-*-*-*-

//cette procedure donne la partie genre
int ExtractGender (char NameGend[max_str]){
    int l=strlen(NameGend);
    return (NameGend[l-1]);
}


///*-*-*-*-*-*-*
///e ) GETMARKS:
///*-*-*-*-*-*-*

//cette procedure genere aleatoirement les notes selon leurs nombre
void GetMarks(char notes[TeM],int Tn,int no[TeM/2]){
    int n;
    int k=0;
    strcpy(notes,"");
    for(int i=0;i<Tn ;i=i+2){
        n=Random(Note+1);
        notes[i]=(n/10)+'0';
        notes[i+1]=(n%10)+'0';
        no[k]=n;
        k++;
    }
    notes[Tn]='\0';
}


///*-*-*-*-*-*-*-*-*-*-
///f ) CACLULE_Moyenne:
///*-*-*-*-*-*-*-*-*-*-



//*-*-*-*-*-*-*-*-*-*
//1.b ) RECH_POS    |
//*-*-*-*-*-*-*-*-*-*

/*apres le remplissage de l'enregistrement e, il faut rechercher ou l'inserer car le fichier doit
etre ordonné et on genere aleatoirement les données*/

void Rech_pos(Enreg e,char lc[Tc],char classe[Tc],int *b,int *p,int b1,int p1,bool *decalage,int *pos,int *pos2){
    int c,pmin,pmax,bmin,bmax;
    char min[max_str];
    char max[max_str];
    char fname[max_str*2];
    if (b1 != 0){
        if (strcasecmp(classe,lc)!=0){
            *decalage=0;
            *b=b1;
            *p=p1;// la position libre
        }
        else{
            if (classe[0]=='p')
                c=classe[1]-'0';
            else
                c=classe[1]-'0'+((classe[0]-'0')*10);
            Rech_Index(c,min,max,&bmin,&pmin,&bmax,&pmax,pos);
            GetFullName(e,fname);
            Rech_pos_index(fname,lc,classe,b1,p1,b,p,pos2,decalage,max);
        }
    }
    else{
        *b=1;
        *p=0;
        *decalage=0;
    }
}

//Les sous fonctions de RECH_POS:
//-------------------------------
///*-*-*-*-*-*-*-*
///a ) RECH_INDEX:
///*-*-*-*-*-*-*-*

//une recherche dichotomique à l'interieur de l'index des classes pour avoir les extrimités de chaque classe dans le fichier


void Rech_Index ( int c, char min[2*max_str], char max[2*max_str], int *bmin, int *pmin, int *bmax, int *pmax,int *pos){
    int j=4*(c/10)+(c%10)-1;
    strcpy(min,tab_index.T[j].min);
    strcpy(max,tab_index.T[j].max);
    *bmin=tab_index.T[j].bmin;
    *bmax=tab_index.T[j].bmax;
    *pmin=tab_index.T[j].pmin;
    *pmax=tab_index.T[j].pmax;
    *pos=j;
}


///*-*-*-*-*-*-*-*-*-*
///b ) RECH_POS_INDEX
///*-*-*-*-*-*-*-*-*-*

void Rech_pos_index(char fname[max_str*2],char lc[Tc],char classe[Tc],int b1,int p1,int *b,int *p,int *pos,bool *decalage,char max[max_str*2]){
    if (strcasecmp(lc,classe)!=0){
        *pos=0;
        *decalage=0;
        *b=b1;
        *p=p1;
    }
    else{
        if(strcasecmp(fname,max)>0){
            *decalage=0;
            *b=b1;
            *p=p1;
            *pos=nb-1;
        }
        else{
            int bi=0;
            int bs=nb-1;
            bool arret=0;
            int m;
            while (!arret){
                m=(bi+bs)/2;
                if (strcasecmp(index_classe[m].Nom_cmplt,fname)<0){
                    bi=m+1;
                }
                else
                    bs=m-1;
                if (bi>bs){
                    arret=1;
                }
            }
            *decalage=1;
            *pos=bi;
            *b=index_classe[bi].b;
            *p=index_classe[bi].p;
        }
    }
}
//a- GetFullName
// une petite fonction qui rend le nom et prenom complet d'un enregistrement

void GetFullName(Enreg e,char *ch[max_str]){
    char space=' ';
    strcpy(ch,e.Nom);
    strncat(ch,&space,1);
    strcat(ch,e.Prenom);
}



//b - getLongueur:
void getLongueur(char ch[max_e],int *longueur){
// elle lit la longueur complete de la chaine
    char l[Tl];
    int k=0;
    strcpy(l,"");
    for (int f=1;f<4;f++){
        strncat(l,&(ch[k]),1);
        k++;
    }
    int m=atoi(l);
    m+=Tl-1+Tc+Tid;
    *longueur=m;
}

//c- getname:

// elle lit le nom complet d'aprés la position i et j

void getName(char ch[max_e],char FN[max_str*2] ){
    char space=' ';
    char Nom[max_str]="";
    char cr;
    int k=Tl+Tid+Tc-2;
    char prenom[max_str]="";
    while ((cr=ch[k])!='$'){
            strncat(Nom,&cr,1);
            k++;
        }

    strncat(Nom,&space,1);
    k++;
    while ((cr=ch[k])!='$'){
        strncat(prenom,&cr,1);
        k++;
    }
    strcpy(FN,Nom);
    strcat(FN,prenom);
}

//*-*-*-*-*-*-*-*-*-*
//1.a ) INSERT      |
//*-*-*-*-*-*-*-*-*-*
// cette procedure assure l'insertion  des enregistrements e selon l'endroit retenu de la recherche


void Insertion (Enreg e,int b,int p,bool decalage,int pos ,char lc[Tc],char classe[Tc],int *b1,int *p1,int pos2){
    char ch [max_e];
    char Name[2*max_str];
    int k,m;
    int l;
    bool arret;
    char dollar='$';
    char euro='/';
    int StopM;
    char c;
    //la conversion de e vers string
    strcpy(ch,e.taille);
    strcat(ch,e.Num_Id);
    strncat(ch,&(e.eff),1);
    strcat(ch,e.num_classe);
    strcat(ch,e.Nom);
    strncat(ch,&dollar,1);
    strcat(ch,e.Prenom);
    strncat(ch,&dollar,1);
    strncat(ch,&(e.genre),1);
    strcat(ch,e.Notes);
    strncat(ch,&euro,1);
    GetFullName(e,Name);
    int bb,pp;
    bb=b;
    pp=p;
    if (!decalage){
        ecrirechaine(ch,&b,&p);
        if(strcasecmp(lc,classe)!=0){// le cas du fichier vide sera aussi traité ici car lc != classe
                strcpy(tab_index.T[Nbe].min,Name);
                strcpy(tab_index.T[Nbe].max,Name);
                tab_index.T[Nbe].bmax=b;
                tab_index.T[Nbe].bmin=bb;
                tab_index.T[Nbe].pmax=p;
                tab_index.T[Nbe].pmin=pp;
                strcpy(tab_index.T[Nbe].classe,e.num_classe);//peut etre changer par des numeros pour faciliter la recherche plutard si on ajoute d'autre classes
                Nbe++;
                strcpy(index_classe[0].Nom_cmplt,Name);
                index_classe[0].b=bb;
                index_classe[0].p=pp;
                nb=1;
        }
        else{//modification seulement du max
            strcpy(tab_index.T[Nbe-1].max,Name);
            tab_index.T[Nbe-1].bmax=b;
            tab_index.T[Nbe-1].pmax=p;
            strcpy(index_classe[nb].Nom_cmplt,Name);
            index_classe[nb].b=bb;
            index_classe[nb].p=pp;
            nb++;
        }
    }
    else{
        if (tab_index.T[pos].bmin==b && tab_index.T[pos].pmin==p){
            //changement du min de l'index
            strcpy(tab_index.T[pos].min,Name);
            //on ne modifie pas bmin et pmin car ils seront les memes pour la donnée précendente
        }
        if (b<*b1){
            buf.pos_libre=*p1;
            ecrireDir(F,*b1,&buf);
            lireDir(F,b,&buf);
        }
        k=0;// le copie de la chaine
        m=0;//mettre la donnee dans la chaine
        l=strlen(ch);
        arret=0;
        StopM=0;
        int y=nb;
        while (y>pos2){
            index_classe[y].p=index_classe[y-1].p+l;
            index_classe[y].b=index_classe[y-1].b;
            if (index_classe[y].p>=Max){
                index_classe[y].p=(index_classe[y].p)%Max;
                index_classe[y].b=(index_classe[y].b)+1;
            }
            strcpy(index_classe[y].Nom_cmplt,index_classe[y-1].Nom_cmplt);
            y--;
        }
        index_classe[y].b=b;
        index_classe[y].p=p;
        strcpy(index_classe[y].Nom_cmplt,Name);
        while(!arret){
            c=ch[k];
            if (b<*b1 || (b==*b1 && p<*p1) ){
                ch[m]=buf.tab[p];
                m=(m+1)%l;
            }
            else
                StopM++;
            buf.tab[p]=c;
            k=(k+1)%l;
            p++;
            if (p>=Max){
                p=0;
                buf.pos_libre=Max;
                buf.tab[Max]='\0';
                ecrireDir(F,b,&buf);
                b++;
                lireDir(F,b,&buf);
            }
            if (m==k && StopM==l)
                arret=1;
        }
        tab_index.T[pos].bmax=b;
        tab_index.T[pos].pmax=p-1;
        // decalage de la table d'index d'etudiants :
        nb++;
        //ecrireDir(F,*b1,&buf);
    }
    *b1=b;
    *p1=p;//pos libre
}

//les sous modules de Insertion :
//-------------------------------
///*-*-*-*-*-*-*-*-*
///a ) ECRIRECHAINE:
///*-*-*-*-*-*-*-*-*

void ecrirechaine(char ch[max_e],int *b,int *p){
    int l=strlen(ch);
    int r=*b;
    int q=*p;
    for (int i=0;i<l;i++){
        buf.tab[q]=ch[i];
        q++;
        if (q==Max){
            buf.pos_libre=Max;
            buf.tab[Max]='\0';
            ecrireDir(F,r,&buf);
            r++;
            q=0;
        }
    }
    *b=r;
    *p=q;
}
//*-*-*-*-*-*-*-*-*-*
//CHARGEMENT_INIT   |
//*-*-*-*-*-*-*-*-*-*
//le module principal
void Chargement_init(char nom_fichier[max_file]){
    char LC[Tc]="";
    char classe[Tc];
    bool decalage;
    int pos;
    int b=1;
    int b1=0;
    int p=0;
    int p1=0;
    int pos2;
    Enreg e;
    F=Ouvrir(nom_fichier,'N');
    for (int i=0;i<600 ;i++){
        e=Remplissage(i);
        strcpy(classe,e.num_classe);
        Rech_pos(e,LC,classe,&b,&p,b1,p1,&decalage,&pos,&pos2);
        Insertion(e,b,p,decalage,pos,LC,classe,&b1,&p1,pos2);
        strcpy(LC,classe);
    }
    buf.pos_libre=p1;
    ecrireDir(F,b1,&buf);
    aff_entete(F,1,b1);
    aff_entete(F,3,0);
    aff_entete(F,2,(b1-1)*Max+p1);
    fermer(F);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                        ( 2 ) Recherche                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//*-*-*-*-*-*-*-*-*-*
//a ) RECHERCHE     |
//*-*-*-*-*-*-*-*-*-*

bool Recherche(char nom_fichier[max_file],char classe[Tc],char nom [max_str],char prenom [max_str],int *b,int *p, int *pos,char ch[max_e]){
    int bmin,bmax,pmin,pmax;
    char max[max_str*2];
    char min[max_str*2];
    char FN[max_str*2];
    int c;
    F=Ouvrir(nom_fichier,'a');
    if (classe[0]=='p')
                c=classe[1]-'0';
            else
                c=classe[1]-'0'+((classe[0]-'0')*10);
    Rech_Index(c,min,max,&bmin,&pmin,&bmax,&pmax,pos);
    int i=bmin;
    int j=pmin;
    int l;
    bool arret=0;
    bool trouve=0;
    char Nfull[max_str*2];
    char space =' ';
    bool eff;
    strcpy(Nfull,nom);
    strncat(Nfull,&space,1);
    strcat(Nfull,prenom);
    lireDir(F,i,&buf);
    while((!arret && !trouve)&& (i<bmax || (i==bmax && j<pmax))){
        LireChaine(i,j,ch);
        getName(ch,FN);
        if (strcasecmp(FN,Nfull)==0){
            getEff(ch,&eff);
            if (eff==0)
                trouve=1;
            else
                arret=1;
        }
        else{
            if(strcasecmp(FN,Nfull)>0)
                arret=1;
            else{
                getLongueur(ch,&l);
                j+=l;
                if (j>=Max){
                    j=j%Max;
                    i++;
                    lireDir(F,i,&buf);
                }
            }
        }
    }
    *b=i;
    *p=j;
    return trouve;
    fermer(F);
}


//les sous modules de la recherche  :
//-----------------------------------
///*-*-*-*-*-*-*-*
///a ) LIRECHAINE:
///*-*-*-*-*-*-*-*

void LireChaine(int i,int j,char ch[max_e]){
    lireDir(F,i,&buf);
    strcpy(ch,"");
    int cmp=0;
    char cr;
    while ((cr=buf.tab[j])!='/'){
        strncat(ch,&cr,1);
        j++;
        if (j==Max){
            j=0;
            i++;
            lireDir(F,i,&buf);
        }
        cmp++;
    }
    strncat(ch,&cr,1);
    ch[cmp+1]='\0';


}

///*-*-*-*-*-*
///a ) GETEFF:
///*-*-*-*-*-*
void getEff(char ch[max_e],bool *eff){
    if ((ch[Tl+Tid-2])=='0')
        *eff=0;
    else
        *eff=1;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                        ( 3 ) Insertion                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


Enreg remplir(char classe[Tc],char nom[max_str],char prenom[max_str],char note[TeM],char genre,int not[TeM/2]){
    Enreg e;
    e.eff='0';
    e.genre=genre;
    strcpy(e.Nom,nom);
    strcpy(e.Prenom,prenom);
    strcpy(e.Notes,note);
    e.Notes[strlen(note)]='\0';
    bool stop=0;
    char ch[Tid];
    TNoeud *p;
    int Id;
    while (!stop){
        Id=Random(max_Id);
        if (!(rechercher_Arb(Id,&p,0))){
            stop=1;
        }
    }
    bool supp=0;
    sprintf(e.Num_Id,"%d",Id);
    if (Id<10 &&Id>=0){
        strcpy(ch,"000");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<100 && Id >=10){
        strcpy(ch,"00");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<1000 && Id >=100){
        strcpy(ch,"0");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    strcpy(e.num_classe,classe);
    int lon=strlen(nom)+strlen(prenom)+1+strlen(note)+2;//1:genre;2:les deux caracteres speciaux entre nom-prenom et prenom-genre
    e.taille[0]=lon /100 +'0';
    e.taille[1]=(lon /10)%10 +'0';
    e.taille[2]=lon %10 +'0';
    e.taille[3]='\0';
    float moy;
    int Annee=classe[0]-'0';
    moy = calcule_moyenne(not,(strlen(note))/2);
    inserer_Arb(Id,e.Nom,e.Prenom,Annee,moy,supp,&Id_tree);
    return e;
}


void insert(char nom_fichier[max_file], char classe[Tc],char nom[max_str],char prenom[max_str],char note[TeM],char genre){
    int b,p,pos,o;
    int StopM;
    bool arret;
    char ch [max_e];
    Enreg e;
    int k,m;
    char c;
    int not[TeM/2];
    int l;
    char dollar='$';
    char euro='/';
    char FN[max_str*2];
    F=Ouvrir(nom_fichier,'a');
    if (!Recherche(nom_fichier,classe,nom,prenom,&b,&p,&pos,ch)){
        e=remplir(classe,nom,prenom,note,genre,not);
        e.Notes[strlen(note)]='\0';
        GetFullName(e,FN);
        //la conversion de e vers string
        strcpy(ch,e.taille);
        strcat(ch,e.Num_Id);
        strncat(ch,&(e.eff),1);
        strcat(ch,e.num_classe);
        strcat(ch,e.Nom);
        strncat(ch,&dollar,1);
        strcat(ch,e.Prenom);
        strncat(ch,&dollar,1);
        strncat(ch,&(e.genre),1);
        strcat(ch,e.Notes);
        strncat(ch,&euro,1);
        //printf("[%s]",ch);
        lireDir(F,entete(F,1),&buf2);
        int last=buf2.pos_libre;
        lireDir(F,b,&buf);
        if (pos==(Nbe-1)&& strcasecmp(FN,tab_index.T[pos].max)>0){
            ecrirechaine(ch,&b,&p);
            strcpy(tab_index.T[pos].max,FN);
            tab_index.T[pos].bmax=b;
            tab_index.T[pos].pmax=p;

        }
        else{

            if (strcasecmp(FN,tab_index.T[pos].min)<0){
                strcpy(tab_index.T[pos].min,FN);
            }
            k=0;// le copie de la chaine
            m=0;//mettre la donnee dans la chaine
            l=strlen(ch);
            arret=0;
            StopM=0;
            while(!arret){
            c=ch[k];
            if (b<entete(F,1) || (b==entete(F,1) && p<last )){
                ch[m]=buf.tab[p];
                m=(m+1)%l;
            }
            else
                StopM++;
            buf.tab[p]=c;
            k=(k+1)%l;
            p++;
            if (p>=Max){
                p=0;
                buf.pos_libre=Max;
                buf.tab[Max]='\0';
                ecrireDir(F,b,&buf);
                b++;
                lireDir(F,b,&buf);
            }
            if (m==k && StopM==l)
                arret=1;
        }
        tab_index.T[pos].pmax=tab_index.T[pos].pmax+l;
        if (tab_index.T[pos].pmax>=Max){
            tab_index.T[pos].pmax=(tab_index.T[pos].pmax)%Max;
            tab_index.T[pos].bmax=1+tab_index.T[pos].bmax;
        }
        for (int u=(pos+1);u<(Nbe-1);u++){
            tab_index.T[u].pmin=tab_index.T[u].pmin+l;
            if (tab_index.T[u].pmin>=Max){
                tab_index.T[u].pmin=(tab_index.T[u].pmin)%Max;
                tab_index.T[u].bmin=tab_index.T[u].bmin+1;
            }
            tab_index.T[u].pmax=tab_index.T[u].pmax+l;
            if (tab_index.T[u].pmax>=Max){
                tab_index.T[u].pmax=(tab_index.T[u].pmax)%Max;
                tab_index.T[u].bmax=tab_index.T[u].bmax+1;
            }
        }
        if (pos!=Nbe-1){
        tab_index.T[Nbe-1].pmin=tab_index.T[Nbe-1].pmin+l;
            if (tab_index.T[Nbe-1].pmin>=Max){
                tab_index.T[Nbe-1].pmin=(tab_index.T[Nbe-1].pmin)%Max;
                tab_index.T[Nbe-1].bmin=tab_index.T[Nbe-1].bmin+1;
            }
            tab_index.T[Nbe-1].pmax=p;
            tab_index.T[Nbe-1].bmax=b;
        }
        }
        buf.pos_libre=p;
        ecrireDir(F,b,&buf);
        aff_entete(F,1,b);
        aff_entete(F,2,entete(F,2)+l);

    }
    fermer(F);
}



/*****************/
/**Rech_matieres**/
/*****************/
typedef struct tab_ch{
    char matiere[max_str];
}tab_ch;
void Rech_matieres(char classe[Tc],tab_ch matieres[TeM/2]){
    bool trouve=0;
    char cr;
    bool arret =0;
    char m[max_str];
    while (!trouve){
        if (fgetc(f3)==';' && fgetc(f3)==classe[0])
            trouve=1;
    }
    fgetc(f3);
    int nb_mat=fgetc(f3)-'0';
    fgetc(f3);
    int k=0;
    for(int u=0;u<nb_mat;u++){
        strcpy(m,"");
        while((cr=fgetc(f3))!=',' && cr!=';')
            strncat(m,&cr,1);
        strcpy(matieres[k].matiere,m);
        k++;

    }

}

/*****************/
/**Saisie_notes***/
/*****************/

void Saisie_note(char classe[Tc],char note[TeM],int not[TeM/2]){
    tab_ch matieres[TeM/2];
    int tn;
    int no;
    if (classe[0]=='p'){
        note="";
    }
    else{
        if (classe[0]=='1' || classe[0]=='2')
            tn=TeM-6;
        else
            tn=TeM;
        Rech_matieres(classe,matieres);
        int k=0;
        printf("    # ) Veuillez saisir vos notes en ordre :\n\n");
        for (int y=0;y<(tn/2);y++){
                do{
                    printf("        --> %s : ",matieres[y].matiere);
                    scanf("%d",&no);
                    if (no>20){
                        printf("             ERREUR de saisie ! La note ne peut pas depacer 20 !!\n\n");
                    }
                    not[y]=no;
                }while (no>20);
            note[k]=no/10+'0';
            k++;
            note[k]=no%10+'0';
            k++;
        }
        note[tn]='\0';

    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 4 ) Mise à Jour                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
// 1 ) Mise à Jour des notes
//--------------------------

///a.1 ) MàJ Note
// cette fonction fait une mise à jour d'une note d'une matiere , seulement pour une, elle peut etre modifiée plut tard pour plusieurs notes
void MaJ_Notes(char nom_fichier[max_file],char classe[Tc],char Nom [max_str],char Prenom[max_str],char matiere[max_str],int note){
    int b,p,pos;
    int n,k;
    int tn ;
    int no[TeM/2];
    char ch[max_e];
    int id;
    float moy;
    TNoeud *m;
    F=Ouvrir(nom_fichier,'a');

    if (classe[0]=='p'|| classe[0]=='P'){
        printf("\n Les eleves de prescolaire n'ont pas des notes ! \n");
    }
    else{
        if (Recherche(nom_fichier,classe,Nom,Prenom,&b,&p,&pos,ch)){
            LireChaine(b,p,ch);
            lireDir(F,b,&buf);
            n=num_matiere(matiere,classe);
            k=p+Tl+Tid+Tc+strlen(Nom)+strlen(Prenom)+n*2-1;
            if (k>=Max){
                k=k%Max;
                b++;
                lireDir(F,b,&buf);
            }
            buf.tab[k]=note/10+'0';
            k++;
            if (k>=Max){
                k=0;
                ecrireDir(F,b,&buf);
                b++;
                lireDir(F,b,&buf);
            }
            buf.tab[k]=note%10+'0';
            ecrireDir(F,b,&buf);
            tn=TeM;
            if(classe[0]=='1' || classe[0]=='2')
                tn=TeM-6;
            int r=k=Tl+Tid+Tc+strlen(Nom)+strlen(Prenom)+n*2-1;
            ch[r]=note/10+'0';
            ch[r+1]=note%10+'0';
            getnotes(ch,tn,no);
            moy=calcule_moyenne(no,tn);
            id=getId(ch);
            rechercher_Arb(id,&m,0);
            Aff_moyenne(m,moy);
        }
    }
    fermer(F);
}


//les sous modules de la MàJ:
//---------------------------


/***************/
/**num_matiere**/
/***************/
int num_matiere(char matiere[],char classe[Tc]){
    bool trouve=0;
    int cmp=0;
    char cr;
    bool arret =0;
    char m[max_str];
    while (!trouve){
        if (fgetc(f3)==';' && fgetc(f3)==classe[0])
            trouve=1;
    }
    fgetc(f3);
    fgetc(f3);
    fgetc(f3);
    while (!arret){
        strcpy(m,"");
        while((cr=fgetc(f3))!=',')
            strncat(m,&cr,1);
        cmp++;
        if (strcasecmp(m, matiere)==0)
            arret=1;
    }
    return cmp;

}



/***************/
/*****GetId****/
/***************/

int getId(char ch[max_e]){
    int k=Tl-1;
    char id[Tid];
    strcpy(id,"");
    for (int i=0;i<(Tid-1);i++){
        strncat(id,&(ch[k]),1);
        k++;
    }
    return (atoi(id));

}


/***************/
/***Getnotes****/
/***************/
void getnotes(char ch[max_e],int tn,int no[TeM/2]){
    int     k=Tl+Tid+Tc-2;
    int cmp=0;
    bool arret=0;
    while (!arret){
        if (ch[k]=='$')
            cmp++;
        if (cmp==2)
            arret=1;
        k++;
    }
    k++;
    char note[TeM]="";
    while (ch[k]!='/'){
            strncat(note,&ch[k],1);
            k++;
    }
    int j=0;
    for (int h=0;h<tn;h=h+2){
        no[j]=(note[h]-'0')*10+note[h+1]-'0';
        j++;
    }
}
// 2 ) Mise à Jour des noms
//-------------------------

///a.2 ) MàJ Nom
//en cas d'erreur dans la saisie du nom  ou du prenom; pour éviter les problemes des décalges ves le haut ou vers le bas , on vas supprimer l'étudiant totalement et le réinserer
void MaJ_Noms(char nom_fichier[max_file],char classe[Tc],char Nom [max_str],char Prenom[max_str],char Nom_nv [max_str],char Prenom_nv[max_str]){
    int b,p,pos;
    char ch[max_e];
    char ch1[max_e];
    F=Ouvrir(nom_fichier,'a');
    if (Recherche(nom_fichier,classe,Nom,Prenom,&b,&p,&pos,ch)){
        Suppr (nom_fichier,ch,b,p,pos,Nom,Prenom);
        if (!Recherche(nom_fichier,classe,Nom_nv,Prenom_nv,&b,&p,&pos,ch1)){
            modifier_nom(ch,Nom_nv,Prenom_nv,classe,Nom,Prenom);
            inserer(ch,b,p,pos,Nom_nv,Prenom_nv);

        }

    }
    fermer(F);
}
//Ses sous modules :
void modifier_nom(char ch[max_e],char Nom[max_str],char Prenom[max_str],char classe[Tc],char Nom_an[max_str],char Prenom_an[max_str]){
    Enreg e;
    strcpy(e.Nom,Nom);
    strcpy(e.Prenom,Prenom);
    e.eff='0';
    char dollar='$';
    char euro='/';
    int cpt=0;
    int k=0;
    while (cpt!=2){
        if (ch[k]=='$')
            cpt++;
        k++;
    }
    e.genre=ch[k];
    k++;
    strcpy(e.Notes,"");
    int y=0;
    while(ch[k]!='/'){
        strncat(e.Notes,&ch[k],1);
        k++;
        y++;
    }
    e.Notes[y]='\0';
    int l;
    l=strlen(e.Notes)+strlen(Nom)+strlen(Prenom)+3;
    e.taille[0]=l /100 +'0';
    e.taille[1]=(l /10)%10 +'0';
    e.taille[2]=l %10 +'0';
    e.taille[3]='\0';
    int Id=getId(ch);
    sprintf(e.Num_Id,"%d",Id);
    if (Id<10 &&Id>=0){
        strcpy(ch,"000");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<100 && Id >=10){
        strcpy(ch,"00");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    else if (Id<1000 && Id >=100){
        strcpy(ch,"0");
        strcat(ch,e.Num_Id);
        strcpy(e.Num_Id,ch);
    }
    strcpy(e.num_classe,"");
    strcpy(e.num_classe,classe);
    //la conversion de e vers string
    strcpy(ch,"");
    strcpy(ch,e.taille);
    strcat(ch,e.Num_Id);
    strncat(ch,&(e.eff),1);
    strcat(ch,e.num_classe);
    strcat(ch,e.Nom);
    strncat(ch,&dollar,1);
    strcat(ch,e.Prenom);
    strncat(ch,&dollar,1);
    strncat(ch,&(e.genre),1);
    strcat(ch,e.Notes);
    strncat(ch,&euro,1);
}

void inserer (char ch[max_e],int b,int p,int pos,char nom[max_str],char prenom[max_str]){
    int o;
    int StopM;
    bool arret;
    int k,m;
    char c;
    int not[TeM/2];
    int l;
    char dollar='$';
    char euro='/';
    char space =' ';
    char FN[max_str*2];
        strcpy(FN,nom);
        strncat(FN,&space,1);
        strcat(FN,prenom);
        lireDir(F,entete(F,1),&buf2);
        int last=buf2.pos_libre;
        lireDir(F,b,&buf);
        if (pos==(Nbe-1)&& strcasecmp(FN,tab_index.T[pos].max)>0){
            ecrirechaine(ch,&b,&p);
            strcpy(tab_index.T[pos].max,FN);
            tab_index.T[pos].bmax=b;
            tab_index.T[pos].pmax=p;

        }
        else{
            if (strcasecmp(FN,tab_index.T[pos].min)<0){
                strcpy(tab_index.T[pos].min,FN);
            }
            k=0;// le copie de la chaine
            m=0;//mettre la donnee dans la chaine
            l=strlen(ch);
            arret=0;
            StopM=0;
            while(!arret){
                c=ch[k];
                if (b<entete(F,1) || (b==entete(F,1) && p<last )){
                    ch[m]=buf.tab[p];
                    m=(m+1)%l;
                }
                else
                    StopM++;
                buf.tab[p]=c;
                k=(k+1)%l;
                p++;
                if (p>=Max){
                    p=0;
                    buf.pos_libre=Max;
                    buf.tab[Max]='\0';
                    ecrireDir(F,b,&buf);
                    b++;
                    lireDir(F,b,&buf);
            }
                if (m==k && StopM==l)
                    arret=1;
            }
            tab_index.T[pos].pmax=tab_index.T[pos].pmax+l;
            if (tab_index.T[pos].pmax>=Max){
                tab_index.T[pos].pmax=(tab_index.T[pos].pmax)%Max;
                tab_index.T[pos].bmax=1+tab_index.T[pos].bmax;
            }
            for (int u=(pos+1);u<(Nbe-1);u++){
                tab_index.T[u].pmin=tab_index.T[u].pmin+l;
            if (tab_index.T[u].pmin>=Max){
                tab_index.T[u].pmin=(tab_index.T[u].pmin)%Max;
                tab_index.T[u].bmin=tab_index.T[u].bmin+1;
            }
            tab_index.T[u].pmax=tab_index.T[u].pmax+l;
            if (tab_index.T[u].pmax>=Max){
                tab_index.T[u].pmax=(tab_index.T[u].pmax)%Max;
                tab_index.T[u].bmax=tab_index.T[u].bmax+1;
            }
        }
        if (pos!=Nbe-1){
        tab_index.T[Nbe-1].pmin=tab_index.T[Nbe-1].pmin+l;
            if (tab_index.T[Nbe-1].pmin>=Max){
                tab_index.T[Nbe-1].pmin=(tab_index.T[Nbe-1].pmin)%Max;
                tab_index.T[Nbe-1].bmin=tab_index.T[Nbe-1].bmin+1;
            }
            tab_index.T[Nbe-1].pmax=p;
            tab_index.T[Nbe-1].bmax=b;
        }
        }
        buf.pos_libre=p;
        ecrireDir(F,b,&buf);
        aff_entete(F,1,b);
        aff_entete(F,2,entete(F,2)+l);

    }


// 3 ) Mise à Jour des classes
//----------------------------

// c'est aussi une suppression et re-insertion de la donnée aprés la modification de la classe.
// c'est clair qu'on ne modifie pas l'année , mais seulement le numéro de la salle.
void MaJ_classe(char nom_fichier[max_file],char classe_an[Tc],char Nom [max_str],char Prenom[max_str],char classe_nv[Tc]){
    int b,p,pos;
    char ch[max_e];
    char ch2[max_e];
    F=Ouvrir(nom_fichier,'a');
    if (classe_an[0]==classe_nv[0]){
        if (Recherche(nom_fichier,classe_an,Nom,Prenom,&b,&p,&pos,ch)){
            Suppr (ch,b,p,pos,Nom,Prenom);
            printf("\n[%s]\n",ch);
            if (!Recherche(nom_fichier,classe_nv,Nom,Prenom,&b,&p,&pos,ch2)){
                ch[Tl+Tid]=classe_nv[1];
                printf("\n[%s]\n",ch);
                inserer(ch,b,p,pos,Nom,Prenom);

            }

        }
    }
    else
        printf("\n Erreur ! Impossible de changer l'annee scolaire !!");
    fermer(F);
}



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 5 ) Suppression                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

bool Suppression (char nom_fichier[max_file], char classe[Tc],char nom[max_str],char prenom[max_str]){
    int b,p,pos;
    char ch[max_e];
    bool supp;
    int k;
    char space=' ';
    char FN[max_str*2];
    int l;
    TNoeud *m;
    char FullN[max_str*2];
    F=Ouvrir(nom_fichier,'a');
    if (Recherche(nom_fichier,classe,nom,prenom,&b,&p,&pos,ch)){
        //LireChaine(b,p,ch);
        int id=getId(ch);
    printf("%s]",ch);
        rechercher_Arb(id,&m,0);
        Aff_Supp(m,1);
        k=p+Tl+Tid-2;
        if (k>=Max){
            k=k%Max;
            b++;
            lireDir(F,b,&buf);
        }
        lireDir(F,b,&buf);
        buf.tab[k]='1';
        ecrireDir(F,b,&buf);
        supp=1;
        // modification de l'index;
        strcpy(FullN,nom);
        strncat(FullN,&space,1);
        strcat(FullN,prenom);
        l=strlen(ch);
        aff_entete(F,3,entete(F,3)+l);
        if (strcasecmp(nom,tab_index.T[pos].min)==0){//cad le premier en classe est celui qu'on va supprimer => on doit le modifier par son suivant
            LireChaine(b,p,ch);
            p+=l;
            if (p>=Max){
                p=p%Max;
                b++;
            }
            LireChaine(b,p,ch);
            getName(ch,FN);
            tab_index.T[pos].bmin=b;
            tab_index.T[pos].pmin=p;
            strcpy(tab_index.T[pos].min,FN);
        }
    }
    else
        supp=0;
    fermer(F);

    return supp;
}

void Suppr (char nom_fichier[max_file],char ch[max_e],int b,int p,int pos,char nom[max_str],char prenom[max_str]){
    int k;
    char space=' ';
    char FN[max_str*2];
    int l;
    TNoeud *m;
    F=Ouvrir(nom_fichier,'a');
    char FullN[max_str*2];
        int id=getId(ch);
        rechercher_Arb(id,&m,0);
        Aff_Supp(m,1);
        k=p+Tl+Tid-2;
        if (k>=Max){
            k=k%Max;
            b++;
        }
        lireDir(F,b,&buf);
        buf.tab[k]='1';
        ecrireDir(F,b,&buf);
        // modification de l'index;
        strcpy(FullN,nom);
        strncat(FullN,&space,1);
        strcat(FullN,prenom);
        getLongueur(ch,&l);
        aff_entete(F,3,entete(F,3)+l);
        if (strcasecmp(nom,tab_index.T[pos].min)==0){//cad le premier en classe est celui qu'on va supprimer => on doit le modifier par son suivant
            LireChaine(b,p,ch);
            p+=l;
            if (p>=Max){
                p=p%Max;
                b++;
            }
            LireChaine(b,p,ch);
            getName(ch,FN);
            tab_index.T[pos].bmin=b;
            tab_index.T[pos].pmin=p;
            strcpy(tab_index.T[pos].min,FN);
        }
        fermer(F);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                      ( 6 ) Affichage                                                           */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


void Affichage1(char nom_fichier[max_file]){ //cette fonction va afficher tous les Classes, les noms, les prénoms et les Ids des eleves:
    int b,p;
    char ch[max_e];
    int l;
    bool eff;
    char FN[max_str*2];
    char ID[Tid];
    char classe[Tc];
    F=Ouvrir(nom_fichier,'a');
    int id;
    for (int i=0;i<Nbe;i++){
        b=tab_index.T[i].bmin;
        p=tab_index.T[i].pmin;

        strcpy(classe,tab_index.T[i].classe);
        /*if (atoi(tab_index.T[i].classe)<10){
            classe[0]='p';
        }*/
        printf("    # ) La Classe : %s\n",classe);

        while(b<tab_index.T[i].bmax || (b==tab_index.T[i].bmax && p<tab_index.T[i].pmax)){
            LireChaine(b,p,ch);
            l=strlen(ch);
            getEff(ch,&eff);
            if (eff==0){
                getName(ch,FN);
                id=getId(ch);
                printf("        * %s  :  [%d]  \n",FN,id);
            }
            p+=l;
            if (p>=Max){
                p=p%Max;
                b++;
                // la lecture est faite dans lirechaine donc pas besoin d'effectuer une lecture.
            }

        }
    }
    fermer(F);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                 ( 7 ) Sauvegardes et Chargements                                               */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


///----------------------------------
/// a- La Sauvegarde de l'index     :
///----------------------------------
void Sauvegarde_index(){
    int b=1;
    int p=0;
    f_index=ouvrir2("index",'N');// à revoir si c N ou A  !!!
    for (int i=0;i<Nbe;i++){
        strcpy(buf_index.tab[p].classe,tab_index.T[i].classe);
        buf_index.tab[p].bmax=tab_index.T[i].bmax;
        buf_index.tab[p].bmin=tab_index.T[i].bmin;
        buf_index.tab[p].pmax=tab_index.T[i].pmax;
        buf_index.tab[p].pmin=tab_index.T[i].pmin;
        strcpy(buf_index.tab[p].max,tab_index.T[i].max);
        strcpy(buf_index.tab[p].min,tab_index.T[i].min);
        p++;
        if (p==max_tof){
            p=0;
            ecrireDir2(f_index,b,&buf_index);
            b++;
        }
    }
    ecrireDir2(f_index,b,&buf_index);
    aff_entete2(f_index,1,b);
    aff_entete2(f_index,2,Nbe);
    fermer2(f_index);
}


///----------------------------------
/// b- La Chargement de l'index     :
///----------------------------------

void Chargement_index(){
    int i=1;
    int j=0;
    int k=0;
    f_index=ouvrir2("index",'A');
    lireDir2(f_index,i,&buf_index);
    for (k=0;k<entete2(f_index,2);k++){
        tab_index.T[k].bmax=buf_index.tab[j].bmax;
        tab_index.T[k].bmin=buf_index.tab[j].bmin;
        tab_index.T[k].pmax=buf_index.tab[j].pmax;
        tab_index.T[k].pmin=buf_index.tab[j].pmin;
        strcpy(tab_index.T[k].classe,buf_index.tab[j].classe);
        strcpy(tab_index.T[k].max,buf_index.tab[j].max);
        strcpy(tab_index.T[k].min,buf_index.tab[j].min);
        j++;
        if (j==max_index){
            j=0;
            i++;
            lireDir2(f_index,i,&buf_index);
        }
    }
    Nbe=entete2(f_index,2);
    fermer2(f_index);
}


///----------------------------------
/// b- La Sauvegarde de l'arbre     :
///----------------------------------

void inordre( TNoeud *r )	// pour mettre les valeur de l'arbre dans le tableau tab_arb
{
   if ( r != NULL ) {
	inordre( fg(r) );
	tab_arb[ia].id=Info(r);
	tab_arb[ia].annee_scolaire=Annee_scolaire(r);
	tab_arb[ia].moyenne=Moyenne(r);
	strcpy(tab_arb[ia].prenom,Prenom(r));
	strcpy(tab_arb[ia].nom,Nom(r));
	tab_arb[ia].supp=Supp(r);
	ia++;
 	//printf(" %d ", Info(r));
 	inordre( fd(r) );
   }
}
void Sauvegarde_arb(){
    int b=1;
    int p=0;
    f_arb=ouvrir3("arbre",'N');// à revoir si c N ou A  !!!
    inordre(Id_tree);
    nb_eleve=ia;
    for (int i=0;i<nb_eleve;i++){
        buf_arb.tab[p].id=tab_arb[i].id;
        buf_arb.tab[p].annee_scolaire=tab_arb[i].annee_scolaire;
        buf_arb.tab[p].moyenne=tab_arb[i].moyenne;
        strcpy(buf_arb.tab[p].nom,tab_arb[i].nom);
        strcpy(buf_arb.tab[p].prenom,tab_arb[i].prenom);
        buf_arb.tab[p].supp=tab_arb[i].supp;
        p++;
        if (p==max_tof2){
            p=0;
            ecrireDir3(f_arb,b,&buf_arb);
            b++;
        }
    }
    ecrireDir3(f_arb,b,&buf_arb);
    aff_entete3(f_arb,1,b);
    aff_entete3(f_arb,2,nb_eleve);
    fermer3(f_arb);
}


///----------------------------------
/// c- La Chargement de l'arbre     :
///----------------------------------

void Chargement_arb(){
    int i=1;
    int j=0;
    int k=0;
    f_arb=ouvrir3("arbre",'A');
    lireDir3(f_arb,i,&buf_arb);
    for (k=0;k<entete3(f_arb,2);k++){
        tab_arb[k].id=buf_arb.tab[j].id;
        tab_arb[k].annee_scolaire=buf_arb.tab[j].annee_scolaire;
        tab_arb[k].moyenne=buf_arb.tab[j].moyenne;
        strcpy(tab_arb[k].nom,buf_arb.tab[j].nom);
        strcpy(tab_arb[k].prenom,buf_arb.tab[j].prenom);
        tab_arb[k].supp=buf_arb.tab[j].supp;
        j++;
        if (j==max_tof2){
            j=0;
            i++;
            lireDir3(f_arb,i,&buf_arb);
        }
    }
    nb_eleve=entete3(f_arb,2);
    fermer3(f_arb);
    for (int t=0;t<nb_eleve;t++){
        inserer_Arb(tab_arb[t].id,tab_arb[t].nom,tab_arb[t].prenom,tab_arb[t].annee_scolaire,tab_arb[t].moyenne,tab_arb[t].supp,&Id_tree);

    }
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                 ( 8 ) Chargement ( Non Initial )                                               */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/**MàJ_AS_arb**/
/**************/
void MaJ_As_arb(TNoeud *p){
    if (p!=NULL){
        MaJ_As_arb(fg(p));
        if (!Supp(p) &&(Annee_scolaire(p)<6))
            AFF_annee(p,Annee_scolaire(p)+1);
        MaJ_As_arb(fd(p));
    }
}


/**keep_index**/
/**************/

void keep_index(){//pour garder l'ancie index afin qu'on puisse le parcourir !
    for (int i=0 ;i<Nbe;i++){
        tab_index2.T[i].bmax=tab_index.T[i].bmax;
        tab_index2.T[i].bmin=tab_index.T[i].bmin;
        tab_index2.T[i].pmin=tab_index.T[i].pmin;
        tab_index2.T[i].pmax=tab_index.T[i].pmax;
        strcpy(tab_index2.T[i].classe,tab_index.T[i].classe);
        strcpy(tab_index2.T[i].max,tab_index.T[i].max);
        strcpy(tab_index2.T[i].min,tab_index.T[i].min);
    }
    Nbe2=Nbe;
}

/**Inscription_prepas**/
/**********************/
void Inscription_prepas(char fichier_new[max_file]){
    char LC[Tc]="";
    char classe[Tc];
    bool decalage;
    int pos;
    int b=1;
    int b1=0;
    int p=0;
    int p1=0;
    int pos2;
    Enreg e;
    Nbe=0;
    F=Ouvrir(fichier_new,'n');
    for (int i=0;i<100 ;i++){
        e=Remplissage(i);
        Id_prep[i].id=atoi(e.Num_Id);
        nb_prepa++;
        strcpy(classe,e.num_classe);
        Rech_pos(e,LC,classe,&b,&p,b1,p1,&decalage,&pos,&pos2);
        Insertion(e,b,p,decalage,pos,LC,classe,&b1,&p1,pos2);
        strcpy(LC,classe);
    }
    buf.pos_libre=p1;
    ecrireDir(F,b1,&buf);
    aff_entete(F,1,b1);
    aff_entete(F,3,0);
    aff_entete(F,2,(b1-1)*Max+p1);
    fermer(F);
}

/**LireChaine2**/
/***************/

//SPECIAL POUR F1 (utilise buf2 directement)

void LireChaine2(int *i,int *j,char ch[max_e]){
    int ii=*i;
    int jj=*j;
    lireDir(F1,*i,&buf2);
    strcpy(ch,"");
    int cmp=0;
    char cr;
    //printf("[%s]",buf2);
    while ((cr=buf2.tab[jj])!='/'){
        strncat(ch,&cr,1);
        jj++;
        if (jj==Max){
            jj=0;
            ii++;
            lireDir(F1,ii,&buf2);
        }
        cmp++;
    }
    strncat(ch,&cr,1);
    ch[cmp+1]='\0';
    jj++;
    if (jj==Max){
        jj=0;
        ii++;
        lireDir(F1,ii,&buf2);
    }
    *i=ii;
    *j=jj;


}
/**Modifier**/
/***********/
void modifier(char ch[max_e]){
    char annee=ch[Tl+Tid-1];
    int Ta;
    TNoeud *p;
    int no[TeM/2];
    char notes[TeM];
    float moy;
    int Tn;
    int id;
    if (ch[Tl+Tid-1]=='1'||ch[Tl+Tid-1]=='2'){
        Ta=TeM-6;
    }
    else if (ch[Tl+Tid-1]=='3'||ch[Tl+Tid-1]=='4'){
        Ta=TeM;
    }
    else
        Ta=0;
    if (annee=='p'){
        ch[Tl+Tid-1]='1';
    }
    else{
        ch[Tl+Tid-1]=ch[Tl+Tid-1]+1;
    }
    if (ch[Tl+Tid-1]=='1' || ch[Tl+Tid-1]=='2')
        Tn=TeM-6;
    else if ((ch[Tl+Tid-1]=='3' || ch[Tl+Tid-1]=='4') || ch[Tl+Tid-1]=='5')
        Tn=TeM;
    GetMarks(notes,Tn,no);
    moy=calcule_moyenne(no,Tn);
    int l;
    getLongueur(ch,&l);
    l=l-(Tl+Tid+Tc-2)-Ta-1+Tn;
    ch[0]=l /100 +'0';
    ch[1]=(l /10)%10 +'0';
    ch[2]=l %10 +'0';
    id=getId(ch);
    rechercher_Arb(id,&p,0);
    Aff_moyenne(p,moy);
    int k=strlen(ch)-Ta-1;
    int y=k;
    int u=0;
    for (int u=0;u<Tn;u++){
        ch[k]=notes[u];
        k++;
    }
    ch[y+Tn]='/';
    ch[y+Tn+1]='\0';

}

/**Passage**/
/***********/

void Passage(char fichier_old[max_file],char fichier_new[max_file]){
    F=Ouvrir(fichier_new,'A');
    F1=Ouvrir(fichier_old,'A');
    int pos1=0;
    int b=entete(F,1);
    lireDir(F,b,&buf);
    int p=buf.pos_libre;
    if (p>=Max){
        p=p%Max;
        b++;
        lireDir(F,b,&buf);
    }
    int pos=Nbe;
    char ch[max_e];
    tab_index.T[pos].pmin=p;
    tab_index.T[pos].bmin=b;
    strcpy(tab_index.T[pos].min,tab_index2.T[pos1].min);
    strcpy(tab_index.T[pos].max,tab_index2.T[pos1].max);
    int b1=1;
    int p1=0;
    bool eff;
    lireDir(F1,b1,&buf2);
    for (int i=0;i<(Nbe2-4);i++){
            //printf("[%d,%d]",tab_index2.T[pos1].bmax ,tab_index2.T[pos1].pmax);
        while(b1<tab_index2.T[pos1].bmax || (b1==tab_index2.T[pos1].bmax && p1<tab_index2.T[pos1].pmax)){
                //printf("[%d,%d]",b,p);
            LireChaine2(&b1,&p1,ch);//spéciale pour F1 || va aussi modifier b1 et p1 pour le passage direct
            getEff(ch,&eff);
            if (!eff){
                modifier(ch);
                ecrirechaine(ch,&b,&p);
            }
        }
        tab_index.T[pos].bmax=b;
        tab_index.T[pos].pmax=p;
        pos1++;
        /*p++;
        if (p==Max){
            p=0;
            b++;
        }*/
        pos++;
        if (pos1<Nbe2){
            tab_index.T[pos].bmin=b;
            tab_index.T[pos].pmin=p;
            tab_index.T[pos].bmax=b;
            strcpy(tab_index.T[pos].max,tab_index2.T[pos1].max);
            strcpy(tab_index.T[pos].min,tab_index2.T[pos1].min);

        }

    }
    buf.pos_libre=p;
    ecrireDir(F,b,&buf);
    aff_entete(F,1,b);
    aff_entete(F,2,b*Max-(Max-p));
    aff_entete(F,3,0);
    fermer(F);
    fermer(F1);
    int z=0;
    for(int o=0;o<6;o++){
        for (int e=1;e<5;e++){
            if (o==0)
                tab_index.T[z].classe[0]='p';
            else
                tab_index.T[z].classe[0]=o+'0';
            tab_index.T[z].classe[1]=e+'0';
            tab_index.T[z].classe[2]='\0';
            z++;
        }
    }
    Nbe=24;
}


/**CHARGEMENT**/
/**************/

void Chargement(char fichier_old[max_file],char fichier_new[max_file]){
    keep_index();
    MaJ_As_arb(Id_tree);
    Inscription_prepas(fichier_new);
    Passage(fichier_old,fichier_new);
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                   ( 9 ) Réorganisation                                                         */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// En cas de plusieurs suppressions , le facteur de chargement va augmenter et va dépasser un certain seuil => Il fay réorganiser le fichier .
void Reorganisation (char old_file [max_file],char new_file[max_file]){
    keep_index();
    F1=Ouvrir(old_file,'a');
    F=Ouvrir(new_file,'n');
    int pos1=0;
    int b=1;
    char ch[max_e];
    int p=0;
    int pos=0;
    tab_index.T[pos].pmin=0;
    tab_index.T[pos].bmin=1;
    strcpy(tab_index.T[pos].min,tab_index2.T[pos1].min);
    strcpy(tab_index.T[pos].max,tab_index2.T[pos1].max);
    int b1=1;
    int p1=0;
    bool eff;
    lireDir(F1,b1,&buf2);
    for (int i=0;i<Nbe2;i++){
        while(b1<tab_index2.T[pos1].bmax || (b1==tab_index2.T[pos1].bmax && p1<tab_index2.T[pos1].pmax)){
            LireChaine2(&b1,&p1,ch);//spéciale pour F1 || va aussi modifier b1 et p1 pour le passage direct
            getEff(ch,&eff);
            if (!eff){
                ecrirechaine(ch,&b,&p);
            }
        }
        tab_index.T[pos].bmax=b;
        tab_index.T[pos].pmax=p;
        pos1++;
        pos++;
        if (pos1<Nbe2){
            tab_index.T[pos].bmin=b;
            tab_index.T[pos].pmin=p;
            tab_index.T[pos].bmax=b;
            strcpy(tab_index.T[pos].max,tab_index2.T[pos1].max);
            strcpy(tab_index.T[pos].min,tab_index2.T[pos1].min);

        }

    }
    buf.pos_libre=p;
    ecrireDir(F,b,&buf);
    aff_entete(F,1,b);
    aff_entete(F,2,b*Max-(Max-p));
    aff_entete(F,3,0);
    fermer(F);
    fermer(F1);
    int z=0;
    for(int o=0;o<6;o++){
        for (int e=1;e<5;e++){
            if (o==0)
                tab_index.T[z].classe[0]='p';
            else
                tab_index.T[z].classe[0]=o+'0';
            tab_index.T[z].classe[1]=e+'0';
            tab_index.T[z].classe[2]='\0';
            z++;
        }
    }
    Nbe=24;

}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                       ( 10 ) Archive                                                           */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// note : MAJ l'index se fait par un decalage dans les 2 tab moy_min / moy_max


void chargement_init_Archive(char nomf[30]){ //Faire le chargement initiale de l'archive a partir un arbre pres remplit
int i=1,j=0;
TNoeud *p;
p=Id_tree;
float moy_min=0,moy_max=0;
A=ouvrir_Archive(nomf,'N');
Inordre_Archive(p,&i,&j,&moy_min,&moy_max);
if (j==0){
    i--;
}
else{   A_buf.NB=j;
        A_Index.tab[i-1].id_min=A_buf.tab[0].id;
        A_Index.tab[i-1].id_max=A_buf.tab[j-1].id;
        A_Index.tab[i-1].moy_min[0]=moy_min;
        A_Index.tab[i-1].moy_max[0]=moy_max;
        ecrireDir_Archive(A,i,&A_buf);
};
aff_entete_Archive(A,(i-1)*10+j+1);
A_Index.NB=i;
fermer_Archive(A);

}
/****************************************/
void Inordre_Archive(TNoeud *p,int *i,int *j, float *moy_min , float *moy_max){//parcour et traitement inordre de l'arbre pour remlpir l'archive
     if ( p != NULL ) {
	Inordre_Archive( fg(p),i,j,moy_min,moy_max);
	A_buf.tab[*j].id=Info(p);
	strcpy(&A_buf.tab[*j].nom,&Nom(p));
	strcpy(&A_buf.tab[*j].prenom,&Prenom(p));
	A_buf.tab[*j].annee_scolaire=Annee_scolaire(p);
	int w = Annee_scolaire(p);
	A_buf.tab[*j].moyennes[w]=Moyenne(p);
	for(int s=0;s<Annee_scolaire(p);s++){
        A_buf.tab[*j].moyennes[s]=Random(110)/(float)11;
	};
	if (Moyenne(p) < (*moy_min)) (*moy_min)=Moyenne(p);
	else{ if ( Moyenne(p) > (*moy_max)) (*moy_max)=Moyenne(p) ;};
	(*j)++;
	if ((*j) == 10 ){ // taux de chargement
        A_Index.tab[*i-1].id_min=A_buf.tab[0].id;
        A_Index.tab[*i-1].id_max=A_buf.tab[9].id;
        A_Index.tab[*i-1].moy_min[0]=(*moy_min);
        A_Index.tab[*i-1].moy_max[0]=(*moy_max);
        A_buf.NB=10; // 10 enreg par bloc initialement
        ecrireDir_Archive(A,*i,&A_buf);
        *j=0;
        (*i)++;
        };
	Inordre_Archive( fd(p),i,j,moy_min,moy_max);
   }
}
void MAJ_Archive(int anne){
    int i,j,b;
    TNoeud *p=Id_tree;
    A=ouvrir_Archive("Archive",'A');
    /***mise a jour les anciens etudiants : icnrementant leur annee scolaire et ajoutant leurs moyennes  et MAJ l'index ***/
    MAJ_Les_anciens(p,&i,&j,&anne);
    /*** Ajouter_Les_nouveaux : 100 eleves dans les classes preparatoies  et MAJ l'index***/
    for(b=0;b<100;b++){
        Rech_A_Index(Id_prep[b],&i,&j);
        Inserer_Archive(Id_prep[b],i,j,anne);
    }
    fermer_Archive(A);
};

void MAJ_Les_anciens(TNoeud *p,int *i,int *j,int *anne){ //parcour et traitement inordre de l'arbre pour remlpir l'archive
     if ( p != NULL ) {
	 MAJ_Les_anciens( fg(p),i,j,anne);

	if (Annee_scolaire(p)>0){
    if ((Annee_scolaire(p)<6) && (Supp(p)==0)){
	       A_buf.tab[*j].annee_scolaire=Annee_scolaire(p);
	       int w = Annee_scolaire(p);
	       A_buf.tab[*j].moyennes[w]=Moyenne(p);
            if (Moyenne(p) < A_Index.tab[*i-1].moy_min[*anne] )  A_Index.tab[*i-1].moy_min[*anne]=Moyenne(p);
            else if ( Moyenne(p) >  A_Index.tab[*i-1].moy_max[*anne])  A_Index.tab[*i-1].moy_max[*anne]=Moyenne(p);
    };
        (*j)++;
        if (*j == A_buf.NB ){
        ecrireDir_Archive(A,*i,&A_buf);
         *j=0;
        (*i)++;
        lireDir_Archive(A,*i,&A_buf);
        };
	}
	MAJ_Les_anciens( fd(p),i,j,anne);
   }
}
//////////////////////
void Rech_A_Index(int id,int *i, int *j){ // a develloper !!!!!! poue etre globale with switch i ...
*i=0;
*j=-1;
bool stop = 0;
while ( !stop  && (*i) < A_Index.NB ){
if ((id >= A_Index.tab[*i].id_min) && (id <= A_Index.tab[*i].id_max)){
stop=1;
}
(*i)++;
};
lireDir_Archive(A,*i,&A_buf);
stop = 0;
while ( !stop  && (*j) < A_buf.NB ){
(*j)++;
if (id <= A_buf.tab[*j].id){
stop=1;
};
};
}


void Inserer_Archive(int id,int i,int j,int anne){ // on a 5 annee 2020 , 2021 , 2022 .. 0 correspend a l'annee 1 dana la simulation etc...
    // printf("\n DANS INSERE_ARCHIVE , entete nb = %d",entete_Archive(A));
     TNoeud *r=CreerNoeud();
     bool t=rechercher_Arb(id,&r,0);
    // printf("\n the location of id : i= %d  j= %d \n",i,j);
     if (t){
     bool cont=1;
     int k,b=(entete_Archive(A)-1) / 25 +1;
     A_Enreg x,e;
     e.id=id;
     e.annee_scolaire=Annee_scolaire(r);
     e.moyennes[Annee_scolaire(r)]=Moyenne(r);
     for (k=0;k<Annee_scolaire(r);k++ ){
        e.moyennes[k]=Random(110)/(float)11;
     };
     strcpy(&e.nom,&Nom(r));
     strcpy(&e.prenom,&Prenom(r));
     while ( (cont) && (i <= b )){
            //MAJ  l'index
        int g=e.annee_scolaire;
        if (e.id<A_Index.tab[i-1].id_min) A_Index.tab[i-1].id_min=e.id;
        else if (e.id>A_Index.tab[i-1].id_max) A_Index.tab[i-1].id_max=e.id;
        if (e.moyennes[g]<A_Index.tab[i-1].moy_min[anne]) A_Index.tab[i-1].moy_min[anne]=e.moyennes[g];
        else if (e.moyennes[g]>A_Index.tab[i-1].moy_max[anne]) A_Index.tab[i-1].moy_max[anne]=e.moyennes[g];
            //MAJ  l'index
      lireDir_Archive(A,i,&A_buf);
      x=A_buf.tab[A_buf.NB-1];
      k=A_buf.NB-1;
     while (k>j){
     A_buf.tab[k]=A_buf.tab[k-1];
     k--;
        };
A_buf.tab[j]=e;
// k=A_buf.NB;
if ( A_buf.NB < A_MAX ){
A_buf.NB=A_buf.NB + 1;
if (j<A_buf.NB-1)  A_buf.tab[A_buf.NB-1]=x;
ecrireDir_Archive(A,i,&A_buf);
cont=0;
}
else{
ecrireDir_Archive(A,i,&A_buf);
i++;
j=0;
e=x;
};
};
// si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement e
if (i>b){
A_buf.tab[0]=e;
A_buf.NB=1;
ecrireDir_Archive(A,i,&A_buf);
aff_entete_Archive(A,entete_Archive(A)+1); // on sauvegarde le num du dernier bloc dans l'entete 1
};
};}





