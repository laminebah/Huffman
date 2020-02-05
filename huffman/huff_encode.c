
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];
int k = 0;
void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {

    int c;

    /* A COMPLETER ... */
    //printf("Programme non realise (ConstruireTableOcc)\n");
	for (int j = 0; j < 256; j ++)
		TableOcc -> tab [j] = 0;
 	c = fgetc(fichier);
    while (c != EOF) {
        /* A COMPLETER ... */
		TableOcc -> tab [c] += 1;
        c = fgetc(fichier);
    };


    int i;
   for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    /* A COMPLETER */
    //printf("Programme non realise (InitHuffman)\n");
	fap mafap = creer_fap_vide ();
	for (int i = 0; i < 256 ; i ++) {
		if (TableOcc -> tab [i] != 0) {
			Arbre z = ArbreVide ();
			z = NouveauNoeud (ArbreVide(),(char)i,ArbreVide());
			mafap = inserer (mafap,z,TableOcc -> tab [i]);
			k += 1;
		}
	}
	
    return mafap;
}

Arbre ConstruireArbre(fap file) {
    /* A COMPLETER */
    //printf("Programme non realise (ConstruireArbre)\n");
	//tant qu'il y au moins deux noeud dans la fap
	int px,py,pz;
	Arbre x,y,z;
	x = ArbreVide ();
	y = ArbreVide ();
	z = ArbreVide ();
	while (k >= 2) {
		file = extraire (file,&x,&px);
		file = extraire (file,&y,&py);
		z = NouveauNoeud (x,'#',y);
		file = inserer (file,z,px+py);
		k -= 1;
	}
	file = extraire (file,&z,&pz);
    return z;
}

/* -Cette fonction est récursive et construit un code pour chacune de ses feuilles
   -Pour chaque feuille f de l'arbre :
		si a est la racine alors on s'intéresse au chemin de a à f
			A chaque fois qu'on se dirige à gauche on prend 0 et à droite on prend 1
		Cette sequence de bit constitue le code de la feuille
*/
void codeTab (Arbre huff , int code [],int lg) {
	if (EstVide(huff))
		exit (1);
	if (FilsGauche(huff) == NULL && FilsDroit(huff) == NULL) {
		HuffmanCode [huff -> etiq].lg = lg;
		for (int i = 0; i < lg ; i ++) 
			HuffmanCode [huff -> etiq].code [i] = code [i];
	} else {
		code [lg] = 0;
		codeTab (FilsGauche (huff),code,lg + 1);
		code [lg] = 1;
		codeTab (FilsDroit (huff),code,lg + 1);
	}
}
//fonction qui affiche les symboles et pour chaque symbole son code
void afficheCode () {
	for (int i = 0 ; i < 256 ; i ++) {
		if (HuffmanCode [i].lg != 0) {
			printf ("%c: %d :",(char)i,HuffmanCode [i].lg);
			for (int j = 0; j < HuffmanCode [i].lg ; j ++)
				printf ("%d",HuffmanCode[i].code[j]);
			printf ("\n");
		}
	}
}
//fonction qui construit les codes en faisant appel à codeTab
void ConstruireCode(Arbre huff) {
	int code [256],lg = 0;
	codeTab (huff,code,lg);
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    
	BFILE *Fout;
	int c;
	int err=0;
	EcrireArbre(fic_out, ArbreHuffman);
	Fout=bstart(fic_out,"w");
	assert (Fout != NULL);
	
	while ((c = fgetc (fic_in))!= EOF) {
		for (int i = 0 ; i < HuffmanCode[c].lg && err != -1; i ++) {
			err = bitwrite (Fout,HuffmanCode[c].code[i]);
			
		}
		
	}
	bstop(Fout);
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);
	afficheCode ();//affichage de la table des codes
    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
