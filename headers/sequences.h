#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "global.h"
#include "start.h"

#define DESCRIPTION_SIZE 154
#define HEADER_SIZE 274
#define SEQ_LINE_SIZE 60



typedef struct sequences {
    char *specialHeader;
    char name[17];                  // le nom de la sequence sans le '>' initial, termine par '\0'
    enum chromosome_t chromosome;
    int start;                      // numero de base de debut
    int end;                        // et de fin
    char *description;              // copie de la chaine "description"
    char *sequence;                 // toute la sequence sur {A,C,G,T}  sans '\n' terminee par '\0'
    struct sequences *next;         // pour faire une liste chainee. NULL si dernier.
} Sequences;

void printSeq(Sequences *s);
void printAllSeq(Sequences *s);
enum chromosome_t str2enum(char *chr);
char *enum2str(enum chromosome_t chr);
void freeSeq(Sequences *s);
void parseHeader(FILE *fd, Sequences *seq);
void getSeq(FILE *fd, Sequences *seq, char *singleLine);
Sequences *readSeq(FILE *fd, Options *args);
long totalLengthSequences(Sequences *seq);
char *parseSpecialHeader(FILE *fd);
void printLoading();

#endif
