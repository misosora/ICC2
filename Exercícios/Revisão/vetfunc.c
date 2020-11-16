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
( Programa: Revisando Vetores e Funções                                                          )
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

// Função de leitura dos números digitados
void readNumbers(int *numArray, int *count){
	char aux;

	while(!feof(stdin)){
		scanf("%d", &numArray[*count]);
		(*count)++;
		
		// Checar se chegou ao final da linha
		aux = getchar();
		if(aux == '\n'){
			break;
		}
	}
}

// Função de contagem da frequência dos números
void countNumbers(int *numArray, int count){
	int *freq = (int *) malloc(count*sizeof(int));
	for(int i = 0; i < count; i++){
		freq[i] = -1;
	}
	
	int aux;	
	for(int i = 0; i < count; i++){
		aux = 1;
		
		for(int j = i+1; j < count; j++){
			if(numArray[i] == numArray[j]){
				aux++;
				freq[j] = 0; // Garantir que não imprima o mesmo número várias vezes
			}
		}
		
		// Se o número ainda não foi contado, sua frequência é salva
		if(freq[i] != 0){
			freq[i] = aux;
		}
	}

	for(int i = 0; i < count; i++){
		if(freq[i] != 0){
			printf("%d (%d)\n", numArray[i], freq[i]);
		}
	}

	free(freq);
}

int main(int argc, char *argv[]){
	int *numArray = (int *) malloc(100*sizeof(int));
	int count = 0;

	readNumbers(numArray, &count);
	countNumbers(numArray, count)	;

	free(numArray);	
	return 0;
}
