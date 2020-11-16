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
( Programa: Revisando Alocação Dinâmica, Recursão e Arquivos                                     )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READLINE_BUFFER 4096

// Função de leitura da entrada do usuário
// Nesse caso, o nome do arquivo a ser aberto e seu conteúdo
char *readLine(FILE *stream) {
	char *string = NULL;
	int i = 0;

	do {
		if (i % READLINE_BUFFER == 0) {
			string = (char *) realloc(string, (i / READLINE_BUFFER + 1)*READLINE_BUFFER);
		}
		string[i] = fgetc(stream);
		i++;
	} while (string[i-1] != '\n' && string[i-1] != EOF);

	if (i >= 2 && string[i-2] == '\r') {
		string[i-2] = '\0';
		string = (char *) realloc(string, i-1);
	} else {
		string[i-1] = '\0';
		string = (char *) realloc(string, i);
	}

	return string;
}

// Função de leitura dos dados do arquivo
void readFile(FILE *fp, int *rows, int *columns, int *x, int *y){
	char *temp = readLine(fp);
	char *token;
	
	token = strtok(temp, " ");
	(*rows) = atoi(token);
	token = strtok(NULL, " ");
	(*columns) = atoi(token);
	
	free(temp);
	temp = readLine(fp);

	token = strtok(temp, " ");
	(*x) = atoi(token);
	token = strtok(NULL, " ");
	(*y) = atoi(token);

	free(temp);
}

// Função de leitura do labirinto contido no arquivo
void readMaze(FILE *fp, int rows, int columns, char **maze, int *people, int *paths){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			fread(&maze[i][j], sizeof(char), 1, fp);
			
			// Em caso de pulo de linha, lê-se o próximo caractere
			if(maze[i][j] == '\n'){
				fread(&maze[i][j], sizeof(char), 1, fp);
			}
			if(maze[i][j] == '#'){
				(*people) += 1; // Contando a quantidade de pessoas no labirinto
			}
			if(maze[i][j] == '.'){
				(*paths) += 1; // Contando a quantidade de caminhos disponíveis
			}
		}
	}
}

// Função recursiva para encontrar o caminho para fora do labirinto
int findPath(int rows, int columns, char **maze, int x, int y){
	// Se a posição atual for a saída
	if(x < 0 || y < 0 || x > (rows-1) || y > (columns-1)){
		return 1;
	}
	
	// Se a posição atual não for um caminho possível
	if(maze[x][y] != '.'){
		return 0;
	}
	
	// Marcação do caminho percorrido
	maze[x][y] = '*';
	
	// Checando os caminho disponíveis na ordem estabelecida:
	// Cima, Direita, Baixo, Esquerda
	if(findPath(rows, columns, maze, x-1, y)){
		return 1;
	}
	if(findPath(rows, columns, maze, x, y+1)){
		return 1;
	}
	if(findPath(rows, columns, maze, x+1, y)){
		return 1;
	}
	if(findPath(rows, columns, maze, x, y-1)){
		return 1;
	}

	return 0;
}

// Função de impressão do labirinto após o caminho ser encontrado
void printMaze(int rows, int columns, char **maze, int *visited){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			if(j == columns-1){
				printf("%c\n", maze[i][j]);
			}else{
				printf("%c", maze[i][j]);
			}
			
			if(maze[i][j] == '*'){
				(*visited) += 1; // Contagem dos pontos visitados
			}
		}
	}
}

// Função de impressão dos resultados finais
void printResults(int people, int paths, int visited, double explored){
	explored = (visited*100)/paths; // Porcentagem de caminhos visitados por disponíveis
	
	printf("\nVoce escapou de todos! Ninguem conseguiu te segurar!\n");
	printf("Veja abaixo os detalhes da sua fuga:\n");
	printf("----Pessoas te procurando: %d\n", people);
	printf("----Numero total de caminhos validos:   %d\n", paths);
	printf("----Numero total de caminhos visitados: %d\n", visited);
	printf("----Exploracao total do labirinto: %.1lf%%\n", explored);
}

int main(int argc, char *argv[]){
	char *file = readLine(stdin);
	FILE *fp;

	int rows, columns, x, y;

	int people = 0, paths = 0, visited = 0;
	double explored = 0;
	
	fp = fopen(file, "r");
	
	readFile(fp, &rows, &columns, &x, &y);
	
	char **maze = (char **) malloc(rows*sizeof(char *));
	for(int i = 0; i < rows; i++){
		maze[i] = (char *) malloc(columns*sizeof(char));
	}

	readMaze(fp, rows, columns, maze, &people, &paths);
	findPath(rows, columns, maze, x, y);
	printMaze(rows, columns, maze, &visited);
	printResults(people, paths, visited, explored);
	
	// Desalocação da memória utilizada
	for(int i = 0; i < rows; i++){
		free(maze[i]);
	}
	free(maze);
	free(file);
	fclose(fp);
	return 0;
}
