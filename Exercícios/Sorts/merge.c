/*************************************************************************************************
(                _                USP-Universidade de São Paulo                 _                )
(    __   ___.--'_`.  ICMC-Instituto de Ciências Matemáticas e de Computação  .'_`--.___   __    )
(   ( _`.'. -   'o` ) ====================================================== ( 'o`   - .`.'_ )   )
(   _\.'_'      _.-'           Bacharelado em Ciências de Computação          `-._      `_`./_   )
(  ( \`. )    //\`                            2020/2                            '/\\    ( .'/ )  )
(   \_`-'`---'\\__, ========================================================== ,__//`---'`-'_/   )
(    \`        `-\           __    __                                           /-´        ´/    )
(     `                     ( 0)__( 0)  Gabriela Satie Faria Nishimi                       ´     )
(                           /   ''   \     ____________________                                  )
(                          ( ´´  3 ´´ )  < Número USP: 11892820)                                 )
(                           \        /                                                           )
(                           /___>o<__\                                                           )
(                           uuu    uuu                                                           )
( Programa: Merge Sort 3-Vias                                                                    )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READLINE_BUFFER 4096

// Struct que guarda o nome e o preço dos livros
typedef struct{
    char *name;
    double price;
} BOOK;

// Função de leitura das entradas: número de livros e livros+preços
char *readLine(FILE *stream){
	char *string = NULL;
	int i = 0;

	do{
		if(i%READLINE_BUFFER == 0) {
			string = (char *) realloc(string, (i/READLINE_BUFFER + 1)*READLINE_BUFFER);
		}
		string[i] = fgetc(stream);
		i++;
	}while(string[i-1] != '\n' && string[i-1] != EOF);

	if(i >= 2 && string[i-2] == '\r'){
		string[i-2] = '\0';
		string = (char *) realloc(string, i-1);
	}else{
		string[i-1] = '\0';
		string = (char *) realloc(string, i);
	}

	return string;
}

// Função de divisão das strings em livros e preços
void splitStrings(int N, char **books, BOOK *library){
    for(int i = 0; i < N; i++){
        char *token = strtok(books[i], ","); // Nomes dos livros
        library[i].name = token;
        token = strtok(NULL, ","); // Preços dos livros
        library[i].price = atof(token+1);
    }
}

// Função de leitura da entrada (livros+preços)
void readBooks(int N, char **books, BOOK *library){
    for(int i = 0; i < N; i++){
        books[i] = readLine(stdin);
    }

    splitStrings(N, books, library);
}

// Função merge utilizada pelo Merge Sort para juntar os vetores ordenados
void merge(BOOK *library, int start, int div1, int div2, int end){
    // Vetor auxiliar que guardará os valores ordenados
    BOOK *aux = (BOOK *) malloc((end-start+1)*sizeof(BOOK));

    int start1 = start; // Início da primeira parte
    int start2 = div1 + 1; // Início da segunda parte
    int start3 = div2 + 1; // Início da terceira parte
    int startAux = 0; // Posição atual do vetor auxiliar

    /*
        Todos os whiles abaixo são utilizados para encontrar os menores valores
        e colocá-los nas posições corretas em cada sub-vetor:
        
        1- Percorre os 3 sub-vetores;
        2- Percorre os 2 primeiros sub-vetores;
        3- Percorre os 2 últimos sub-vetores;
        4- Percorre os 2 sub-vetores das extremidades;
        5- Percorre o primeiro sub-vetor;
        6- Percorre o segundo sub-vetor;
        7- Percorre o terceiro sub-vetor.
    */
    
    while(start1 <= div1 && start2 <= div2 && start3 <= end){
        if(library[start1].price <= library[start2].price){
            aux[startAux] = (library[start1].price <= library[start3].price) ? (library[start1++]) : (library[start3++]);
        }else{
            aux[startAux] = (library[start2].price <= library[start3].price) ? (library[start2++]) : (library[start3++]);
        }
        startAux++;
    }

    while(start1 <= div1 && start2 <= div2){
        aux[startAux] = (library[start1].price <= library[start2].price) ? (library[start1++]) : (library[start2++]);
        startAux++;
    }

    while(start2 <= div2 && start3 <= end){
        aux[startAux] = (library[start2].price <= library[start3].price) ? (library[start2++]) : (library[start3++]);
        startAux++;
    }

    while(start1 <= div1 && start3 <= end){
        aux[startAux] = (library[start1].price <= library[start3].price) ? (library[start1++]) : (library[start3++]);
        startAux++;
    }

    while(start1 <= div1){
        aux[startAux] = library[start1++];
        startAux++;
    }
    
    while(start2 <= div2){
        aux[startAux] = library[start2++];
        startAux++;
    }

    while(start3 <= end){
        aux[startAux] = library[start3++];
        startAux++;
    }

    // Copiando o vetor auxiliar (ordenado) no vetor original
    for(int i = start, j = 0; i <= end; i++, j++){
        library[i] = aux[j];
    }

    free(aux);
}

// Função Merge Sort, que utiliza a estratégia de divisão e conquista para ordenar um vetor
void mergeSort(BOOK *library, int start, int end){
    if(end < start) return;

    for(int i = start; i <= end; i++){
        printf("%.2f ", library[i].price);
    }
    printf("\n");

    // Separação das condições de return para impressão dos vetores unitários
    if(end == start) return;
    
    // Divisões do vetor
    int div1 = start + (end-start)/3;
    int div2 = ((end-div1)%2 != 0) ? (div1 + (end-div1)/2 + 1) : (div1 + (end-div1)/2); // Somando 1 se a divisão não for exata

    // Chamada recursiva do Merge Sort em 3 partes
    mergeSort(library, start, div1);
    mergeSort(library, div1+1, div2);
    mergeSort(library, div2+1, end);

    // Junção e ordenação das partes do vetor
    merge(library, start, div1, div2, end);
}

// Função de impressão dos nomes dos livros e seus respectivos preços
void printBooks(int N, BOOK *library){
    printf("\n");
    for(int i = 0; i < N; i++){
        printf("%s, R$%.2f\n", library[i].name, library[i].price);
    }
}

// Função que libera a memória utilizada para o vetor de chars e para a struct
void freeArrays(int N, char **books, BOOK *library){
    for(int i = 0; i < N; i++){
        free(books[i]);
    }
    free(books);
    free(library);
}

int main(int argc, char *argv[]){
    // Leitura da quantidade de livros
    char *tempN = readLine(stdin);
    int N = atoi(tempN);
    free(tempN);

    BOOK *library = (BOOK *) malloc(N*sizeof(BOOK));
    char **books = (char **) malloc(N*sizeof(char *));
    
    // Chamada das funções principais
    readBooks(N, books, library);
    mergeSort(library, 0, N-1);
    printBooks(N, library);
    freeArrays(N, books, library);
    
    return 0;
}