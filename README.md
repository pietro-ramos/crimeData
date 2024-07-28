# Projeto de Análise de Dados de Crimes

Este projeto, desenvolvido na disciplina de Estrutura de Dados do Instituto Federal do Rio Grande do Sul, é um programa em C que realiza a leitura, armazenamento e busca de dados de crimes utilizando estruturas de dados como arrays e tabelas hash. Ele é capaz de carregar dados de arquivos CSV, ordenar esses dados para buscas binárias e armazená-los em uma tabela hash para buscas eficientes.

## Funcionalidades

- **Leitura de dados**: Lê dados de múltiplos arquivos CSV.
- **Armazenamento**: Armazena os dados em um array para busca binária e em uma tabela hash para buscas rápidas.
- **Busca**: Implementa busca binária e busca usando tabela hash.
- **Análise de Desempenho**: Conta o número de comparações realizadas em ambas as buscas e o número de colisões na tabela hash.

## Estrutura do Projeto

- `main.c`: Contém a função `main` e funções relacionadas ao menu interativo.
- `crimeData.h`: Contém as definições de estruturas de dados e declarações de funções.
- `crimeData.c`: Contém as implementações das funções de manipulação de dados, busca e análise.

## Como Compilar

O projeto foi desenvolvido utilizando Dev C++, caso interesse, baixe o programa [aqui](https://www.bloodshed.net) e execute o .dev, apos, utilize os atalhos para compilaçao e execuçao.
Para compilar o projeto, use o `gcc` ou outro compilador C compatível. Aqui está um exemplo de comando para compilar o projeto usando `gcc`:

```sh
gcc -o crimeAnalysis main.c crimeData.c
```

## Como Executar

Após a compilação, execute o programa gerado (`crimeAnalysis`). O programa exibirá um menu interativo com as seguintes opções:

```sh
./crimeAnalysis
```

## Menu Interativo

- **0 - SAIR**: Sai do programa.
- **1 - BUSCAR TUDO COM BSEARCH**: Realiza busca binária para todos os dados e exibe o total e o máximo de comparações.
- **2 - BUSCAR TUDO COM TABELA HASH**: Realiza busca usando tabela hash para todos os dados e exibe o total e o máximo de comparações.
- **3 - PROCURAR UM COM BSEARCH**: Procura um registro específico usando busca binária.
- **4 - PROCURAR UM COM TABELA HASH**: Procura um registro específico usando tabela hash.
- **5 - DADOS DA MEMÓRIA**: Exibe informações sobre o uso de memória do array e da tabela hash.

## Estruturas de Dados

### CrimeData

A estrutura `CrimeData` é usada para armazenar informações sobre crimes. Ela inclui os seguintes campos:

- `char cidade[100]`: Nome da cidade.
- `int mes`: Mês do registro.
- `int homicidioDoloso`, `int totalVitimas_Doloso`, `int latrocinio`, `int furto`, `int abigeato`, `int furtoVeiculo`, `int roubo`, `int rouboVeiculo`, `int estelionato`, `int delito_ArmaMunicao`, `int entorpecentesPosse`, `int entorpecentesTrafico`, `int vitimasLatrocinio`, `int vitimasLesaoMorte`, `int totalVitimasCVLI`: Campos para diferentes tipos de crimes.
- `struct CrimeData *next`: Ponteiro para o próximo elemento na lista encadeada (usado na tabela hash).

## Funções Principais

### Leitura e Armazenamento

- `CrimeData* readData(const char *filename, int mes, int *count)`: Lê dados de um arquivo CSV.
- `void sortData(CrimeData *data, int totalCount)`: Ordena os dados para busca binária.
- `void initHash()`: Inicializa a tabela hash.
- `void insertHash(CrimeData *data)`: Insere um registro na tabela hash.

### Busca

- `CrimeData* bSearch(CrimeData *data, int count, const char *cidade, int mes, int *cmp)`: Realiza uma busca binária.
- `CrimeData* searchHash(const char *cidade, int mes, int *cmp)`: Realiza uma busca na tabela hash.

### Análise de Desempenho

- `void searchAllBSearch(CrimeData *data, int totalCount)`: Realiza busca binária para todos os dados e conta o número de comparações.
- `void searchAllHashTable()`: Realiza busca na tabela hash para todos os dados e conta o número de comparações.
- `int countCollisions()`: Conta o número de colisões na tabela hash.

## Exemplo de Uso

```sh
./crimeAnalysis
===== MENU =====
0 - SAIR
1 - BUSCAR TUDO COM BSEARCH
2 - BUSCAR TUDO COM TABELA HASH
3 - PROCURAR UM COM BSEARCH
4 - PROCURAR UM COM TABELA HASH
5 - DADOS DA MEMÓRIA
Escolha uma opção:
```

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
