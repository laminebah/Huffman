#include <stdio.h>
#include <stdlib.h>

#include "huffman_code.h"
#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

struct code_char HuffmanCode[256];

void coder (Arbre ArbreHuffman,int code [],int lg) {
	if (EstVide(ArbreHuffman))
		exit (1);
	if (FilsGauche(ArbreHuffman) == NULL && FilsDroit(ArbreHuffman) == NULL) {
		HuffmanCode [ArbreHuffman -> etiq].lg = lg;
		for (int i = 0; i < lg ; i ++) 
			HuffmanCode [ArbreHuffman -> etiq].code [i] = code [i];
	} else {
		code [lg] = 0;
		coder (FilsGauche (ArbreHuffman),code,lg + 1);
		code [lg] = 1;
		coder (FilsDroit (ArbreHuffman),code,lg + 1);
	}
}

int compareTab (int bbit [],int k) {
	for (int i = 0; i < 256 ; i ++) {
		if (HuffmanCode [i].lg != 0 && HuffmanCode[i].lg == k) {
			int j = 0;
			while (j < k && HuffmanCode [i].code [j] == bbit [j]) j ++;
			if (j == k) return i;
		}
	}
	return -1;
}
void Decoder(FILE *fichier_encode, Arbre ArbreHuffman) {
      AfficherArbre(ArbreHuffman);
    /* A COMPLETER */
    //printf("Programme non realise (Decoder)\n");
	int b,code [256],lg = 0,i=0,bbit[256],k = 0,j=0,c;
	int *bit = malloc (sizeof (int) * 256);
	if (bit == NULL) exit(1);

	coder (ArbreHuffman,code,lg);
	BFILE *fin = bstart (fichier_encode,"r");
	assert (fin != NULL);
	while ((b=bitread (fin)) != -1) {
		if (i >= sizeof(bit)) {
			if ((bit=realloc(bit,sizeof(int)*2*i)) == NULL) exit (1);
		}
		bit[i] = b;
		//printf ("%d",bit[i]);
		i ++;
	}
	//copie du résultat dans un fichier decode.txt
	FILE *fdecode = fopen ("decode.txt","w");
	if (fdecode == NULL) exit (1);
	while (j < i) {
		bbit[k] = bit [j]; 
		j ++; k ++;
		while ((c=compareTab (bbit,k)) == -1) {
			bbit [k] = bit [j];
			k ++; j ++;
		}
		printf ("%c",c);
		fprintf (fdecode,"%c",c);
		k = 0;
	 }
	printf("\n");
	fclose (fdecode);
	free (bit);
	bstop (fin);
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");

    Arbre ArbreHuffman = LireArbre(fichier_encode);

    Decoder(fichier_encode, ArbreHuffman);

    fclose(fichier_encode);

    return 0;
}
