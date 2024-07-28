#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crimedata.h"


CrimeData* hashTable[TABLE_SIZE];

// Função para remover BOM lido em ACEGUA
void remove_bom(char *line)
{
    // UTF-8 BOM é 0xEF, 0xBB, 0xBF
    if (line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF')
	{
        memmove(line, line + 3, strlen(line) - 2);
    }
}


void parse_line(char *line, int mes, CrimeData *data)
{
    char *token = strtok(line, ";");
    strcpy(data->cidade, token);

    data->mes = mes; // Atribui o mês correto para todas as linhas do arquivo
    data->homicidioDoloso = atoi(strtok(NULL, ";"));
    data->totalVitimas_Doloso = atoi(strtok(NULL, ";"));
    data->latrocinio = atoi(strtok(NULL, ";"));
    data->furto = atoi(strtok(NULL, ";"));
    data->abigeato = atoi(strtok(NULL, ";"));
    data->furtoVeiculo = atoi(strtok(NULL, ";"));
    data->roubo = atoi(strtok(NULL, ";"));
    data->rouboVeiculo = atoi(strtok(NULL, ";"));
    data->estelionato = atoi(strtok(NULL, ";"));
    data->delito_ArmaMunicao = atoi(strtok(NULL, ";"));
    data->entorpecentesPosse = atoi(strtok(NULL, ";"));
    data->entorpecentesTrafico = atoi(strtok(NULL, ";"));
    data->vitimasLatrocinio = atoi(strtok(NULL, ";"));
    data->vitimasLesaoMorte = atoi(strtok(NULL, ";"));
    data->totalVitimasCVLI = atoi(strtok(NULL, ";"));
}

CrimeData* readData(const char *filename, int mes, int *count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
	{
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char line[1000];
    int capacity = MAX_SIZE;
    CrimeData *data = (CrimeData*) calloc(capacity, sizeof(CrimeData));
    if (!data)
	{
        fclose(file);
        perror("Erro ao alocar memória");
        return NULL;
    }

    *count = 0;

    while (fgets(line, sizeof(line), file))
	{
    	remove_bom(line);
    	if (line[strlen(line) - 1] == '\n')
		{
        	line[strlen(line) - 1] = '\0';
    	}
        if ((*count) >= capacity)
		{
            while ((*count) >= capacity)
			{
                capacity += INCREASE;
            }

            CrimeData *temp = (CrimeData*) realloc(data, capacity * sizeof(CrimeData));
            if (temp)
			{
                data = temp;
            } else {
                perror("Erro ao alocar memória");
                free(data);
                fclose(file);
                capacity -= INCREASE;
                return NULL;
            }
        }

        parse_line(line, mes, &data[*count]);
        (*count)++;
    }

    fclose(file);
    return data;
}

int cmpData(const void *a, const void *b)
{
    const CrimeData *data1 = (const CrimeData*) a;
    const CrimeData *data2 = (const CrimeData*) b;
    int cmp = strcmp(data1->cidade, data2->cidade);
    if (cmp == 0)
	{
        return data1->mes - data2->mes;
    }
    return cmp;
}

void sortData(CrimeData *data, int totalCount)
{
    qsort(data, totalCount, sizeof(CrimeData), cmpData);
}

void initHash()
{
    memset(hashTable, 0, sizeof(hashTable)); // inicializa a hash com valores nulos
}

unsigned int hash(const char *str, int mes)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
	{
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}
	
    hash = ((hash << 5) + hash) + mes; // Adiciona o mês à função hash

    return hash % TABLE_SIZE;
}

void insertHash(CrimeData *data)
{
    unsigned int index = hash(data->cidade, data->mes);
//    printf("Inserindo cidade: %s, mês: %d no índice %d\n", data->cidade, data->mes, index);

    // Alocar um novo CrimeData para a tabela hash
    CrimeData *newData = (CrimeData *)malloc(sizeof(CrimeData));
    if (!newData)
	{
        perror("Erro ao alocar memória para novo CrimeData na hash");
        return;
    }
    memcpy(newData, data, sizeof(CrimeData));
    newData->next = NULL;

    // Verifica se há algum dado já na posição da tabela hash
    if (hashTable[index] == NULL)
	{
        hashTable[index] = newData;
    }
	else
	{
        // Verifica se já existe um nó com a mesma cidade e mês na lista encadeada
        CrimeData *current = hashTable[index];
        while (current != NULL)
		{
            if (strcmp(current->cidade, newData->cidade) == 0 && current->mes == newData->mes)
			{
//                printf("Cidade %s, mês %d já existe na tabela hash. Inserindo na lista encadeada.\n", newData->cidade, newData->mes);
                // Insere o novo dado no final da lista encadeada
                CrimeData *temp = current->next;
                current->next = newData;
                newData->next = temp;
                return;
            }
            current = current->next;
        }
        // Se não encontrou um nó com a mesma cidade e mês, insere no início da lista encadeada
        newData->next = hashTable[index];
        hashTable[index] = newData;
    }

//    printf("Inserção concluída para cidade: %s, mês: %d no índice %d\n", newData->cidade, newData->mes, index);
}

int countCollisions()
{
    int collisions = 0;
    for (int i = 0; i < TABLE_SIZE; ++i)
	{
        CrimeData *data = hashTable[i];
        if (data != NULL && data->next != NULL)
		{
            // Incrementa se houver mais de um elemento na lista encadeada
            while (data->next != NULL)
			{
                collisions++;
                data = data->next;
            }
        }
    }
    return collisions;
}

CrimeData* searchHash(const char *cidade, int mes, int *cmp)
{
    unsigned int index = hash(cidade, mes);
    CrimeData *data = hashTable[index];
    *cmp = 0;
//    printf("Procurando por %s no mês %d na hash table no índice %d\n", cidade, mes, index);

    // Percorre apenas a lista encadeada do índice hash correspondente
    while (data != NULL)
	{
        (*cmp)++;
        if (strcmp(data->cidade, cidade) == 0 && data->mes == mes)
		{
//            printf("Encontrado na lista encadeada do índice %d\n", index);
            return data;
        }
        data = data->next;
    }

    // Retorna NULL se não encontrou o dado correspondente
    return NULL;
}



void freeData(CrimeData *data, int count)
{
    if (data)
	{
        free(data);  // Apenas libera o array de CrimeData
    }
}

void freeHashTable()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
	{
        CrimeData *current = hashTable[i];
        while (current)
		{
            CrimeData *next = current->next;
            free(current);
            current = next;
        }
        hashTable[i] = NULL;
    }
}


CrimeData* bSearch(CrimeData *data, int count, const char *cidade, int mes, int *cmp)
{
    int left = 0;
    int right = count - 1;
    *cmp = 0;

    while (left <= right)
	{
        int mid = left + (right - left) / 2;

        CrimeData key;
        strncpy(key.cidade, cidade, sizeof(key.cidade) - 1);
        key.cidade[sizeof(key.cidade) - 1] = '\0';
        key.mes = mes;

        (*cmp)++;
        int cmpResult = cmpData(&key, &data[mid]);

        if (cmpResult == 0)
		{
            return &data[mid];
        } 
		else if (cmpResult < 0)
		{
            right = mid - 1;
        }
		else
		{
            left = mid + 1;
        }
    }

    return NULL;
}

char* get_string_input(char* prompt)
{
    static char buffer[100];
    printf("%s", prompt);

    if (fgets(buffer, sizeof(buffer), stdin))
	{
        size_t len = strlen(buffer);

        // Remove o caractere de nova linha (\n) se presente
        if (len > 0 && buffer[len - 1] == '\n')
		{
            buffer[len - 1] = '\0';
        }
		else
		{
            // Limpa o buffer de entrada para evitar caracteres adicionais
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        return buffer;
    }
	else
	{
        printf("Entrada inválida. Tente novamente.\n");

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return NULL;
    }
}

int get_int_input(char* prompt)
{
    int input;
    char buffer[100];  // Buffer para armazenar a entrada do usuário
    char c;

    while (1)
	{
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin))
		{
            // Sscanf para extrair um número inteiro do buffer
            if (sscanf(buffer, "%d", &input) == 1)
			{
                return input;
            }
			else
			{
                printf("Entrada inválida. Tente novamente.\n");
            }
        }
		else
		{
            printf("Erro na leitura. Tente novamente.\n");
        }

        // Limpa o buffer de entrada para evitar caracteres adicionais
        while ((c = getchar()) != '\n' && c != EOF);
    }
}
