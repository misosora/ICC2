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
( Programa: Cocktail Sort                                                                        )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define READLINE_BUFFER 4096

// Função de leitura da entrada do usuário
// Neste caso, dos nomes e suas respectivas cidades e quantidades
char *readLine(FILE *stream) {
	char *string = NULL;
	int i = 0;

	do{
		if(i % READLINE_BUFFER == 0){
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

// Função que lê as strings digitadas de acordo com
// as quantidades especificadas
void readInput(int quant, char **strings){
	for(int i = 0; i < quant; i++){
		char *temp = readLine(stdin);
		strcpy(strings[i], temp);
		free(temp);
	}
}

// Função que aloca uma matriz de chars
void allocArray(char **array, int qtd){
	for(int i = 0; i < qtd; i++){
		array[i] = (char *) malloc(200*sizeof(char));
	}
}

// Função que separa as strings digitadas em nomes e cidades
void separateNamesCities(char **namesCities, char **names, char **cities, int quantNames){
	for(int i = 0; i < quantNames; i++){
		char *token = strtok(namesCities[i], ".");
		strcpy(names[i], token);
		strcat(names[i], ".");
		
		token = strtok(NULL, ".");
		strcpy(cities[i], token+1);
	}
}

// Função que retorna a posição do nome a ser procurado no total de nomes
int findPosition(char **names, char *searchName, int quantNames){
	for(int i = 0; i < quantNames; i++){
		if(!strcmp(names[i], searchName)){
			return i;
		}
	}
	return -1; // Se o nome não for encontrado
}

// Função que procura pelos nomes especificados e suas respectivas cidades,
// colocando 1 no array de resultados quando o nome for de São Carlos e
// 0 quando não for
void compareSearch(char **names, char **cities, char **searchNames, int quantNames, int quantSearches, int *cityResults){
	int pos = 0;
	
	for(int i = 0; i < quantSearches; i++){
		pos = findPosition(names, searchNames[i], quantNames);
		
		if(pos != -1){
			if(!strcmp(cities[pos], "Sao Carlos")){
				cityResults[i] = 1;
			}else{
				cityResults[i] = 0;
			}
		}
	}
}

// Função que troca a posição de duas strings
void swap(char *str1, char *str2){
	char *temp = (char *) malloc(200*sizeof(char));
	strcpy(temp, str1);
	strcpy(str1, str2);
	strcpy(str2, temp);
	
	free(temp);
}

// Função que converte todos os chars de uma string para minúsculos
// e os guarda em um array auxiliar
void tolowerString(char *string, char *aux){
	strcpy(aux, string);
	for(int i = 0; aux[i]; i++){
		aux[i] = tolower(aux[i]);
	}
}

// Cocktail Sort: Função de ordenamento do array de strings,
// realizando um bubble sort duplo ao percorrer o array do fim ao início
// e do início ao fim alternadamente
void cocktailSort(char **array, int qtd){
	int done;
	char *aux1 = (char *) malloc(200*sizeof(char));
	char *aux2 = (char *) malloc(200*sizeof(char));
	
	do{
		done = 0;

		for(int i = qtd-1; i > 0; i--){
			tolowerString(array[i-1], aux1);
			tolowerString(array[i], aux2);
			
			// Comparação das strings totalmente minúsculas
			if(strcmp(aux1, aux2) > 0){
				swap(array[i-1], array[i]);
				done = 1;
			}
		}
		for(int i = 1; i < qtd; i++){
			tolowerString(array[i-1], aux1);
			tolowerString(array[i], aux2);
			
			// Comparação das strings totalmente minúsculas
			if(strcmp(aux1, aux2) > 0){
				swap(array[i-1], array[i]);
				done = 1;
			}
		}

	}while(done);

	free(aux1);
	free(aux2);
}

// Função de impressão do array ordenado e dos resultados obtidos
// na função compareSearch
void printSortedArray(char **array, int qtdArray, int qtdResults, int *results){
	cocktailSort(array, qtdArray);
	
	for(int i = 0; i < qtdArray; i++){
		printf("%s\n", array[i]);
	}
	for(int i = 0; i < qtdResults; i++){
		printf("%d\n", results[i]);
	}
}

// Função que libera a memória alocada para uma matriz de chars
void freeArray(char **array, int qtd){
	for(int i = 0; i < qtd; i++){
		free(array[i]);
	}
	free(array);
}

int main(int argc, char *argv[]){
	int quantNames, quantSearches, *cityResults;
	char **namesCities, **names, **cities, **searchNames;
	
	// Leitura da quantidade de nomes + cidades
	char *aux = readLine(stdin);
	quantNames = atoi(aux);
	free(aux);
	
	// Alocação das matrizes de chars e leitura dos nomes + cidades
	namesCities = (char **) malloc(quantNames*sizeof(char *));
	names = (char **) malloc(quantNames*sizeof(char *));
	cities = (char **) malloc(quantNames*sizeof(char *));
	allocArray(namesCities, quantNames);
	allocArray(names, quantNames);
	allocArray(cities, quantNames);
	readInput(quantNames, namesCities);
	
	// Leitura da quantidade de nomes para busca
	aux = readLine(stdin);
	quantSearches = atoi(aux);
	free(aux);
	
	// Alocação das matrizes de chars e leitura dos nomes para busca
	searchNames = (char **) malloc(quantSearches*sizeof(char *));
	cityResults = (int *) malloc(quantSearches*sizeof(int));
	allocArray(searchNames, quantSearches);
	readInput(quantSearches, searchNames);
	
	// Chamada das funções principais:
	// - Separar os nomes das pessoas dos nomes das cidades
	// - Buscar pelos nomes procurados e suas respectivas cidades, comparando à desejada
	// - Imprimir o array de nomes ordenado e os resultados da busca
	separateNamesCities(namesCities, names, cities, quantNames);
	compareSearch(names, cities, searchNames, quantNames, quantSearches, cityResults);
	printSortedArray(names, quantNames, quantSearches, cityResults);
	
	// Liberação da memória utilizada
	freeArray(namesCities, quantNames);
	freeArray(names, quantNames);
	freeArray(cities, quantNames);
	freeArray(searchNames, quantSearches);
	free(cityResults);
	return 0;
}
