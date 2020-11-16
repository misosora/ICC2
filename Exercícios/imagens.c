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
( Programa: Criando Imagens Digitais                                                             )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

// Função que imprime um valor inteiro de acordo com a formatação desejada
void printValue(int N, int i, int j, int value){
	if(j == N-1){
		printf("%d\n", value);
	}else{
		printf("%d\t", value);
	}
}

// Função que cria uma matriz com valores aleatórios a partir da seed T
// e imprime seus valores em sequência
void setMatrix(int N, int T, int **image){
	for(int i = 0; i < N; i++){
		image[i] = (int *) malloc(N*sizeof(int));
	}
	
	srand(T);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			image[i][j] = rand()%256;
			printValue(N, i, j, image[i][j]);
		}
	}
}

// Função recursiva que muda os valores da matriz de acordo com as especificações
void changeMatrix(int N, int T, int **image, int x, int y, int p){
	if(x < 0 || y < 0 || x > N-1 || y > N-1) return; // Borda da imagem atingida

	int v = image[x][y];
	int m = abs(p-v);

	if(m > T || v == p) return;
	
	if(m <= T) image[x][y] = p;
	
	// Chamada recursiva das direções em relação à posição atual
	changeMatrix(N, T, image, x-1, y, image[x][y]); // Cima
	changeMatrix(N, T, image, x, y+1, image[x][y]); // Direita
	changeMatrix(N, T, image, x+1, y, image[x][y]); // Baixo
	changeMatrix(N, T, image, x, y-1, image[x][y]); // Esquerda
}

// Função que cria um arquivo de imagem no formato PGM
void createPGM(int N, int **image, int initial){
	FILE *fp;
	if(initial) fp = fopen("initial.pgm", "w+");
	else fp = fopen("final.pgm", "w+");
	
	fprintf(fp, "P2\n%d %d\n%d\n", N, N, 255);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){	
			if(j == N-1) fprintf(fp, "%d\n", image[i][j]);
			else fprintf(fp, "%d ", image[i][j]);
		}
	}

	fclose(fp);
}

// Função de impressão da matriz após suas alterações
void printNewMatrix(int N, int **image){
	printf("\n");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			printValue(N, i, j, image[i][j]);
		}
	}
}

// Função que desaloca a memória utilizada numa matriz de inteiros
void freeMatrix(int N, int **image){
	for(int i = 0; i < N; i++){
		free(image[i]);
	}
	free(image);
}

int main(int argc, char *argv[]){
	// Variáveis principais e matriz da imagem
	int N, x, y, T;
	scanf("%d %d %d %d", &N, &x, &y, &T);
	int **image = (int **) malloc(N*sizeof(int *));
	
	setMatrix(N, T, image); // Setar e imprimir os valores da matriz inicial
	createPGM(N, image, 1); // Criar o arquivo de imagem PGM da matriz inicial
	
	// Mudar os valores da matriz
	changeMatrix(N, T, image, x-1, y, image[x][y]); // Cima
	changeMatrix(N, T, image, x, y+1, image[x][y]); // Direita
	changeMatrix(N, T, image, x+1, y, image[x][y]); // Baixo
	changeMatrix(N, T, image, x, y-1, image[x][y]); // Esquerda
	
	createPGM(N, image, 0); // Criar o arquivo de imagem PGM da matriz final
	printNewMatrix(N, image); // Imprimir os valores da matriz final
	freeMatrix(N, image); // Liberar a memória

	return 0;
}
