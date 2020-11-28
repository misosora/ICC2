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
( Programa: Spotify                                                                              )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READLINE_BUFFER 4096
#define MAX 20000 // Número máximo de músicas
#define MAX_NAME 100

// Struct que contém todos os dados sobre as músicas contidas no CSV
typedef struct song_{
	char trackName[MAX_NAME];
	char trackId[MAX_NAME];
	char albumName[MAX_NAME];
	char albumId[MAX_NAME];
	char artistName[MAX_NAME];
	char artistId[MAX_NAME];
	char releaseDate[MAX_NAME];
	int length;
	float popularity;
	float acousticness;
	float danceability;
	float energy;
	float instrumentalness;
	float liveness;
	float loudness;
	float speechiness;
	float tempo;
	int timeSignature;
} SONG;

// Struct que contém os dados de cada artista presente no CSV
typedef struct artist_{
	char artistName[MAX_NAME];
	char artistId[MAX_NAME];
	int numTracks;
	float artistPopularity;
} ARTIST;

// Função de leitura de strings
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

// Função merge utilizada pelo Merge Sort para juntar os vetores ordenados
void merge(ARTIST *artists, SONG *songs, int start, int div1, int div2, int end, int check){
	// Vetor auxiliar que guardará os valores ordenados
	SONG *aux1;
	ARTIST *aux2;
	
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
	
	// O valor de 'check' define se estamos ordenando a struct de músicas (1) ou artistas (0)
	// em todas as comparações abaixo
	if(check){
		aux1 = (SONG *) malloc((end-start+1)*sizeof(SONG));
		
		while(start1 <= div1 && start2 <= div2 && start3 <= end){
			if(strcmp(songs[start1].artistName, songs[start2].artistName) <= 0){
				aux1[startAux] = (strcmp(songs[start1].artistName, songs[start3].artistName) <= 0) ? (songs[start1++]) : (songs[start3++]);
			}else{
				aux1[startAux] = (strcmp(songs[start2].artistName, songs[start3].artistName) <= 0) ? (songs[start2++]) : (songs[start3++]);
			}
			
			startAux++;
		}

		while(start1 <= div1 && start2 <= div2){
			aux1[startAux] = (strcmp(songs[start1].artistName, songs[start2].artistName) <= 0) ? (songs[start1++]) : (songs[start2++]);
			startAux++;
		}

		while(start2 <= div2 && start3 <= end){
			aux1[startAux] = (strcmp(songs[start2].artistName, songs[start3].artistName) <= 0) ? (songs[start2++]) : (songs[start3++]);
			startAux++;
		}

		while(start1 <= div1 && start3 <= end){
			aux1[startAux] = (strcmp(songs[start1].artistName, songs[start3].artistName) <= 0) ? (songs[start1++]) : (songs[start3++]);
			startAux++;
		}

		while(start1 <= div1){
			aux1[startAux] = songs[start1++];
			startAux++;
		}

		while(start2 <= div2){
			aux1[startAux] = songs[start2++];
			startAux++;
		}

		while(start3 <= end){
			aux1[startAux] = songs[start3++];	
			startAux++;
		}
		
		// Copiando o vetor auxiliar (ordenado) no vetor original
		for(int i = start, j = 0; i <= end; i++, j++){
			songs[i] = aux1[j];
		}

		free(aux1);	
	
	}else{
		aux2 = (ARTIST *) malloc((end-start+1)*sizeof(ARTIST));
		
		while(start1 <= div1 && start2 <= div2 && start3 <= end){
			if(artists[start1].artistPopularity <= artists[start2].artistPopularity){
				aux2[startAux] = (artists[start1].artistPopularity <= artists[start3].artistPopularity) ? (artists[start1++]) : (artists[start3++]);
			}else{
				aux2[startAux] = (artists[start2].artistPopularity <= artists[start3].artistPopularity) ? (artists[start2++]) : (artists[start3++]);
			}
			
			startAux++;
		}

		while(start1 <= div1 && start2 <= div2){
			aux2[startAux] = (artists[start1].artistPopularity <= artists[start2].artistPopularity) ? (artists[start1++]) : (artists[start2++]);
			startAux++;
		}

		while(start2 <= div2 && start3 <= end){
			aux2[startAux] = (artists[start2].artistPopularity <= artists[start3].artistPopularity) ? (artists[start2++]) : (artists[start3++]);
			startAux++;
		}

		while(start1 <= div1 && start3 <= end){
			aux2[startAux] = (artists[start1].artistPopularity <= artists[start3].artistPopularity) ? (artists[start1++]) : (artists[start3++]);
			startAux++;
		}

		while(start1 <= div1){
			aux2[startAux] = artists[start1++];
			startAux++;
		}

		while(start2 <= div2){
			aux2[startAux] = artists[start2++];
			startAux++;
		}

		while(start3 <= end){
			aux2[startAux] = artists[start3++];
			startAux++;
		}
		
		// Copiando o vetor auxiliar (ordenado) no vetor original
		for(int i = start, j = 0; i <= end; i++, j++){
			artists[i] = aux2[j];
		}

		free(aux2);	
	}	
}

// Função Merge Sort, que utiliza a estratégia de divisão e conquista para ordenar um vetor
void mergeSort(ARTIST *artists, SONG *songs, int start, int end, int check){
	if(end <= start) return;
    
	// Divisões do vetor
	int div1 = start + (end-start)/3;
	int div2 = ((end-div1)%2 != 0) ? (div1 + (end-div1)/2 + 1) : (div1 + (end-div1)/2); // Somando 1 se a divisão não for exata

	// Chamada recursiva do Merge Sort em 3 partes + junção e ordenação das partes do vetor
	// O valor de 'check' define se estamos ordenando a struct de músicas (1) ou artistas (0)
	if(check){
		mergeSort(artists, songs, start, div1, 1);
		mergeSort(artists, songs, div1+1, div2, 1);
		mergeSort(artists, songs, div2+1, end, 1);

		merge(artists, songs, start, div1, div2, end, 1);
	}else{
		mergeSort(artists, songs, start, div1, 0);
		mergeSort(artists, songs, div1+1, div2, 0);
		mergeSort(artists, songs, div2+1, end, 0);

		merge(artists, songs, start, div1, div2, end, 0);
	}
}

// Função que checa se o token é uma música específica sem nome no CSV
void checkToken(int i, SONG *songs, char *token){
	if(!strcmp(" 0rQtoQXQfwpDW0c7Fw1NeM", token)){
		strcpy(songs[i].trackId, token+1);
	}else{
		strcpy(songs[i].trackName, token);
		token = strtok(NULL, ";");
		strcpy(songs[i].trackId, token+1);
	}
}

// Função que divide cada linha do CSV em todas as características
// das músicas (especificadas na primeira linha do arquivo) e as coloca na struct
void setSongs(int *i, SONG *songs, char *token){	
	checkToken(*i, songs, token);
	
	token = strtok(NULL, ";");
	strcpy(songs[*i].albumName, token+1);
	
	token = strtok(NULL, ";");
	strcpy(songs[*i].albumId, token+1);
	
	token = strtok(NULL, ";");
	strcpy(songs[*i].artistName, token+1);
	
	token = strtok(NULL, ";");
	strcpy(songs[*i].artistId, token+1);
	
	token = strtok(NULL, ";");
	strcpy(songs[*i].releaseDate, token+1);
	
	token = strtok(NULL, ";");
	songs[*i].length = atoi(token);
	
	token = strtok(NULL, ";");
	songs[*i].popularity = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].acousticness = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].danceability = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].energy = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].instrumentalness = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].liveness = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].loudness = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].speechiness = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].tempo = atof(token);
	
	token = strtok(NULL, ";");
	songs[*i].timeSignature = atoi(token);
	
	(*i) += 1;
}

// Função que conta e coloca todos os artistas na struct,
// calculando também sua popularidade
void setArtists(int *i, int j, SONG *songs, ARTIST *artists){
	strcpy(artists[0].artistName, songs[0].artistName);
	strcpy(artists[0].artistId, songs[0].artistId);
	artists[0].numTracks = 1;
	artists[0].artistPopularity = songs[0].popularity;
	(*i) = 1;

	// Comparar todos os elementos em artists com os elementos de songs:
	// - Se o artista já estiver em artists, soma 1 em numTracks e soma a popularidade da música
	// - Se o artista não estiver em artists, adiciona o artista e suas infos na posição seguinte
	// 	 e soma 1 à quantidade de artistas

	int k = 1;
	while(k < j){
		if(!strcmp(artists[*i-1].artistName, songs[k].artistName)){
			artists[*i-1].numTracks += 1;
			artists[*i-1].artistPopularity += songs[k].popularity;
		}else{
			strcpy(artists[*i].artistName, songs[k].artistName);
			strcpy(artists[*i].artistId, songs[k].artistId);
			artists[*i].numTracks = 1;
			artists[*i].artistPopularity += songs[k].popularity;
			(*i) += 1;
		}
		k++;
	}

	// Cálculo final da popularidade do artista
	for(int k = 0; k < (*i); k++){
		artists[k].artistPopularity /= artists[k].numTracks;
	}
}

// Função de leitura do CSV de músicas
void readCSV(FILE *fp, SONG *songs, ARTIST *artists, int *numArtists, int *numTracks){
	while(!feof(fp)){
		char *temp = readLine(fp);
		if((*numTracks) == 0){ // Primeira linha do CSV é descartada
			free(temp);
			temp = readLine(fp);
		}

		char *token = strtok(temp, ";");
		if(token == NULL){ // Parada da leitura quando acabarem as informações
			free(temp);
			break;
		}

		setSongs(numTracks, songs, token);
		
		free(temp);
	}
	
	mergeSort(artists, songs, 0, (*numTracks)-1, 1);
	setArtists(numArtists, *numTracks, songs, artists);
}

// Função que ordena e imprime as informações desejadas
void printArtists(ARTIST *artists, SONG *songs, int numArtists, int numTracks){
	mergeSort(artists, songs, 0, numArtists-1, 0);

	printf("O Dataset contem %d Artistas e %d Musicas\n", numArtists, numTracks);
	printf("Os artistas mais populares sao:\n");
	for(int i = numArtists-1; i >= 0; i--){
		printf("(%d) Artista: %s, Musicas: %d musicas, Popularidade: %.2lf, Link: https://open.spotify.com/artist/%s\n", numArtists-i, artists[i].artistName, artists[i].numTracks, artists[i].artistPopularity, artists[i].artistId);
	}
}

int main(int argc, char *argv[]){
	char *fileName = readLine(stdin);
	FILE *fp = fopen(fileName, "r+");

	int numArtists = 0, numTracks = 0;

	SONG *songs = (SONG *) malloc(MAX*sizeof(SONG));
	ARTIST *artists = (ARTIST *) malloc((MAX/200)*sizeof(SONG));

	readCSV(fp, songs, artists, &numArtists, &numTracks);
	printArtists(artists, songs, numArtists, numTracks);

	fclose(fp);
	free(songs);
	free(artists);
	free(fileName);
	
	return 0;
}
