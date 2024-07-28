#ifndef CRIMEDATA_H
#define CRIMEDATA_H

#define TABLE_SIZE 8000
#define MAX_SIZE 500
#define INCREASE 50

typedef struct CrimeData {
    char cidade[100];
    int mes;
    int homicidioDoloso;
    int totalVitimas_Doloso;
    int latrocinio;
    int furto;
    int abigeato;
    int furtoVeiculo;
    int roubo;
    int rouboVeiculo;
    int estelionato;
    int delito_ArmaMunicao;
    int entorpecentesPosse;
    int entorpecentesTrafico;
    int vitimasLatrocinio;
    int vitimasLesaoMorte;
    int totalVitimasCVLI;
    struct CrimeData *next;
} CrimeData;

extern CrimeData* hashTable[TABLE_SIZE];

void parse_line(char *line, int mes, CrimeData *data);
CrimeData* readData(const char *filename, int mes, int *count);
void initHash();
void endHash();
int cmpData(const void *a, const void *b);
unsigned int hash(const char *str, int mes);
void insertHash(CrimeData *data);
CrimeData* searchHash(const char *cidade, int mes, int *cmp);
void sortData(CrimeData *data, int totalCount);
void freeData(CrimeData *data, int count);
void freeHashTable();
CrimeData* bSearch(CrimeData *data, int totalCount, const char *cidade, int mes, int *cmp);
char* get_string_input(char* prompt);
int get_int_input(char* prompt);
void remove_bom(char *line);
int countCollisions();

#endif

