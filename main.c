#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "crimedata.h"

void printMemoryInfo(int arraySize, int hashTableSize) {
    printf("Tamanho do array utilizado no bsearch: %d elementos\n", arraySize);
    printf("Tamanho total em bytes do array utilizado no bsearch: %lu bytes\n", (unsigned long)(arraySize * sizeof(CrimeData)));

    printf("Tamanho da tabela de dispers�o: %d elementos\n", TABLE_SIZE);
    printf("Tamanho total em bytes da tabela de dispers�o: %lu bytes\n", (unsigned long)(TABLE_SIZE * sizeof(CrimeData*)));
}

void searchAllBSearch(CrimeData *data, int totalCount) {
    int cmpTotal = 0;
    int cmpMax = 0;

    for (int i = 0; i < totalCount; ++i) {
        CrimeData key;
        strncpy(key.cidade, data[i].cidade, sizeof(key.cidade) - 1);
        key.cidade[sizeof(key.cidade) - 1] = '\0';
        key.mes = data[i].mes;

        int cmp = 0;  // Reinicializa cmp para cada busca individual
        CrimeData *found = bSearch(data, totalCount, key.cidade, key.mes, &cmp);
        cmpTotal += cmp;
        
        if (cmp > cmpMax) {
            cmpMax = cmp;
        }
    }
    
    printf("Total de compara��es realizadas com bsearch: %d\n", cmpTotal);
    printf("M�ximo de compara��es para encontrar um objeto: %d\n", cmpMax);
}


void searchAllHashTable() {
    int cmpTotal = 0;
    int cmpMax = 0;
    int cmpMaxIndex = -1;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        CrimeData *data = hashTable[i];

        while (data) {
            int cmp = 0;  // Reinicializa o cmp para cada busca individual
            searchHash(data->cidade, data->mes, &cmp);
            cmpTotal += cmp;
            data = data->next;

            if (cmp > cmpMax) {
                cmpMax = cmp;
                cmpMaxIndex = i;
            }
        }
    }
    int collisions = countCollisions();
    printf("Total de compara��es realizadas com tabela hash: %d\n", cmpTotal);
    printf("M�ximo de compara��es para encontrar um objeto na tabela hash: %d\n", cmpMax);
    printf("�ndice com o maior n�mero de compara��es: %d\n", cmpMaxIndex);
    printf("N�mero de colis�es na tabela hash: %d\n", collisions);
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    const char *filenames[] = {
        "data/mes01.csv", "data/mes02.csv", "data/mes03.csv", "data/mes04.csv",
        "data/mes05.csv", "data/mes06.csv", "data/mes07.csv", "data/mes08.csv",
        "data/mes09.csv", "data/mes10.csv", "data/mes11.csv", "data/mes12.csv"
    };
    int numFiles = sizeof(filenames) / sizeof(filenames[0]);

    int totalCount = 0;
    CrimeData *data = NULL;
    initHash();

    // Leitura e inser��o de dados dos arquivos CSV
    for (int i = 0; i < numFiles; ++i) {
        int count;
        int mes = i + 1; // Definir o m�s baseado no �ndice do arquivo
        CrimeData *newData = readData(filenames[i], mes, &count);
        if (newData) {
            CrimeData *temp = realloc(data, (totalCount + count) * sizeof(CrimeData));
            if (temp) {
                data = temp;
                memcpy(data + totalCount, newData, count * sizeof(CrimeData));
                for (int j = 0; j < count; ++j) {
                    insertHash(&newData[j]);
                }
                totalCount += count;
                free(newData);
            } else {
                perror("Erro ao realocar mem�ria");
                freeData(data, totalCount);
                return 1;
            }
        } else {
            perror("Erro ao carregar os dados");
            freeData(data, totalCount);
            return 1;
        }
    }

    // Ordenar os dados para busca bin�ria
    sortData(data, totalCount);

    // Menu interativo
    int option;
    do {
        printf("\n===== MENU =====\n");
        printf("0 - SAIR\n");
        printf("1 - BUSCAR TUDO COM BSEARCH\n");
        printf("2 - BUSCAR TUDO COM TABELA HASH\n");
        printf("3 - PROCURAR UM COM BSEARCH\n");
        printf("4 - PROCURAR UM COM TABELA HASH\n");
        printf("5 - DADOS DA MEM�RIA\n");
        option = get_int_input("Escolha uma op��o: ");

        switch (option) {
            case 0:
                printf("Saindo...\n");
                freeData(data, totalCount); // Libera a mem�ria do array de CrimeData
                freeHashTable(); // Libera a mem�ria da hash table
                break;
            case 1:
                searchAllBSearch(data, totalCount);
                break;
            case 2:
                searchAllHashTable();
                break;
            case 3: {
                int mes, cmp;
                char* cidade = get_string_input("Digite o nome da cidade: ");
                mes = get_int_input("Digite o m�s (1-12): ");
                CrimeData key;
                strncpy(key.cidade, cidade, sizeof(key.cidade) - 1);
                key.cidade[sizeof(key.cidade) - 1] = '\0';
                key.mes = mes;
                CrimeData *found = bSearch(data, totalCount, key.cidade, key.mes, &cmp);
                if (found) {
                    printf("Dados encontrados:\n");
			        printf("Cidade: %s\n", found->cidade);
			        printf("M�s: %d\n", found->mes);
			        printf("Homic�dio Doloso: %d\n", found->homicidioDoloso);
			        printf("Total V�timas Doloso: %d\n", found->totalVitimas_Doloso);
			        printf("Latroc�nio: %d\n", found->latrocinio);
			        printf("Furto: %d\n", found->furto);
			        printf("Abigeato: %d\n", found->abigeato);
			        printf("Furto Ve�culo: %d\n", found->furtoVeiculo);
			        printf("Roubo: %d\n", found->roubo);
			        printf("Roubo Ve�culo: %d\n", found->rouboVeiculo);
			        printf("Estelionato: %d\n", found->estelionato);
			        printf("Delito Arma/Muni��o: %d\n", found->delito_ArmaMunicao);
			        printf("Entorpecentes Posse: %d\n", found->entorpecentesPosse);
			        printf("Entorpecentes Tr�fico: %d\n", found->entorpecentesTrafico);
			        printf("V�timas Latroc�nio: %d\n", found->vitimasLatrocinio);
			        printf("V�timas Les�o Morte: %d\n", found->vitimasLesaoMorte);
			        printf("Total V�timas CVLI: %d\n", found->totalVitimasCVLI);
			        printf("N�mero de compara��es: %d\n", cmp);
                } else {
                    printf("Dados n�o encontrados\n");
                }
                break;
            }
            case 4: {
                int mes, cmp;
                char* cidade = get_string_input("Digite o nome da cidade: ");
                mes = get_int_input("Digite o m�s (1-12): ");
                CrimeData *found = searchHash(cidade, mes, &cmp);
                if (found) {
                    printf("Dados encontrados:\n");
			        printf("Cidade: %s\n", found->cidade);
			        printf("M�s: %d\n", found->mes);
			        printf("Homic�dio Doloso: %d\n", found->homicidioDoloso);
			        printf("Total V�timas Doloso: %d\n", found->totalVitimas_Doloso);
			        printf("Latroc�nio: %d\n", found->latrocinio);
			        printf("Furto: %d\n", found->furto);
			        printf("Abigeato: %d\n", found->abigeato);
			        printf("Furto Ve�culo: %d\n", found->furtoVeiculo);
			        printf("Roubo: %d\n", found->roubo);
			        printf("Roubo Ve�culo: %d\n", found->rouboVeiculo);
			        printf("Estelionato: %d\n", found->estelionato);
			        printf("Delito Arma/Muni��o: %d\n", found->delito_ArmaMunicao);
			        printf("Entorpecentes Posse: %d\n", found->entorpecentesPosse);
			        printf("Entorpecentes Tr�fico: %d\n", found->entorpecentesTrafico);
			        printf("V�timas Latroc�nio: %d\n", found->vitimasLatrocinio);
			        printf("V�timas Les�o Morte: %d\n", found->vitimasLesaoMorte);
			        printf("Total V�timas CVLI: %d\n", found->totalVitimasCVLI);
			        printf("N�mero de compara��es: %d\n", cmp);
                } else {
                    printf("Dados n�o encontrados\n");
                }
                break;
            }
            case 5:
                printMemoryInfo(totalCount, TABLE_SIZE);
                break;
            default:
                printf("Op��o inv�lida\n");
                break;
        }
    } while (option != 0);
    
    return 0;
}

