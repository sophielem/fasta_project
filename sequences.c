#include "headers/sequences.h"
#include "headers/global.h"


/* Affichage d'une sequence */
void printSeq(Seq *s) {

	char *nomchromo[]={"I","II","III","MT","MTR","AB325691"};
	printf("%s %s[%i-%i] %s",s->nom,nomchromo[s->chromosome],s->deb,s->fin,s->description);
	if((strlen(s->sequence)) != s->fin - s->deb + 1)
		printf(" tronquee a %li bases\n", strlen(s->sequence));
	else
	    printf("\n");
}



/* Affichage de toutes les sequences */
void printAllSeq(Seq *s) {

  if(s!=NULL){
      printAllSeq(s->suiv);
      printSeq(s);
      /* inverser ces deux lignes pour afficher la liste a l'envers.
	 Utile si construite par ajouts en tete : on aura l'ordre
	 du fichier. */
    }
}

/*converti le type du chromosome dans sa valeur enum */
enum chromosome_t conversionEnum(char * chr){

	enum chromosome_t chromosome;

	if(strcmp(chr,"I") == 0) chromosome = I;
	if(strcmp(chr,"II") == 0) chromosome = II;
	if(strcmp(chr,"III") == 0) chromosome = III;
	if(strcmp(chr,"MT") == 0) chromosome = MT;
	if(strcmp(chr,"MTR") == 0) chromosome = MTR;
	if(strcmp(chr,"AB325691") == 0) chromosome = AB325691;

	return chromosome;
}


/* Libère récursivement la liste chaînée */
void freeSeq(Seq *s){

	if(s == NULL) return;
	else{
		freeSeq(s->suiv);
		free(s->sequence);
		free(s->description);
		free(s);
	}
}


/* Parse l'entête des séquences */
void parseHeader(FILE *fd, Seq *seq){

	int i;
	char str[HEADER_SIZE + 1], chromosome[40];

	fgets(str, HEADER_SIZE + 1, fd);// récupère 1ère ligne

	for(i=0; i<5; i++){ //lit les 5 premiers champs de la 1ère ligne
		switch(i){
			case 0: strcpy(seq->nom , strtok(str," ")); // printf("%s",seq->sequence);Récupère le nom
				break;
			case 2: strcpy(chromosome, strtok(NULL," ")); // Récupère le champ chromosome pour le parser après
				break;
			default: strtok(NULL," ");
		}
	}

	//parse l'entête
	strtok(NULL,"\"");
	seq->description = (char *)malloc(DESC_SIZE + 1);
	strcpy(seq->description , strtok(NULL,"\""));


	//parse le champ "chromosome"
	strtok(chromosome,":");
	strtok(NULL,":");
	seq->chromosome = conversionEnum(strtok(NULL,":")); //type
	seq->deb = atoi(strtok(NULL,":")); //debut
	seq->fin = atoi(strtok(NULL,":")); //fin
	strtok(NULL,":");
}


/* Récupère la séquence nucléotidique et l'enregistre dans la structure */
void getSeq(FILE *fd, Seq *seq, char *singleLine){

	char *newSeq;
	char c = (char)fgetc(fd);
	do{
		ungetc(c, fd);
		fgets(singleLine, SEQ_LINE_SIZE + 1, fd);
		c = (char)fgetc(fd);
		if(c != '\n') ungetc(c,fd); // Enlève le retour à la ligne
		newSeq = (char *) realloc(seq->sequence, strlen(seq->sequence) + strlen(singleLine) + 1);
		if(newSeq == NULL){
			printf("\nError realloc\n");
			exit(EXIT_FAILURE);
		} else seq->sequence = newSeq;
		strcat(seq->sequence, singleLine);
		if(seq->sequence[strlen(seq->sequence) - 1] == '\n') seq->sequence[strlen(seq->sequence) - 1] = '\0';
		c = (char)fgetc(fd); // Vérifie si la ligne suivante est un nouvel entête ou la suite de la séquence
	} while(c != '>' && !feof(fd));
	ungetc(c, fd);
}




/* Parse un fichier FASTA en allouant dynamiquement une liste chaînée */
Seq * readSeq(FILE *fd){

	char c;
	char singleLine[SEQ_LINE_SIZE + 2];

	// Allocations dynamiques
	Seq *seq = (Seq *) malloc(sizeof(Seq));
	seq->sequence = (char *)malloc(SEQ_LINE_SIZE + 1);

	if(feof(fd)) return NULL; // Fin de fichier: fin de la liste chaînée

	c = (char)fgetc(fd);
	if(c == '>'){
		parseHeader(fd, seq);
		//if(DEBUG_SEQUENCES) printf("Nom: %s\nChromosome: %d\ndebut: %d\nfin: %d\nDescription: %s\n",seq->nom,seq->chromosome,seq->deb,seq->fin,seq->description);
		getSeq(fd, seq, singleLine);
		//if(DEBUG_SEQUENCES) printf("%s\n",seq->sequence);
	}
	seq->suiv = readSeq(fd); // Appel récursif pour remplir la liste chaînée

	return seq;
}