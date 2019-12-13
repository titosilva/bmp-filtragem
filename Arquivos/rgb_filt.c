// Implementação das funções de RGB filt
#include "rgb_filt.h"
#include <stdio.h>
#include <stdlib.h>

// Eu adaptei a parte de filtros para que fosse possivel um tamanho de janela par tambem
// Esse caso nao deve existir e no codigo principal eu adicionei um bloqueio a este caso
// Mas fiz so como algo complementar

int rgb_filtro_media(unsigned char **Ri, unsigned char **Gi, unsigned char **Bi, unsigned char **Rf, unsigned char **Gf, unsigned char **Bf, int janela, int largura, int altura){
	// Erros
	// 0xA_ - Erros de alocação dinâmica
	unsigned char **Rt, **Gt, **Bt; // Matrizes temporarias
	int i, j, x, y;
	int erro=0;
	int tam_h = (janela%2==0)? altura+janela : altura+janela-1; // Diferenciação para casos pares e ímpares
	int tam_v = (janela%2==0)? largura+janela : largura+janela-1;

	Rt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));
	Gt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));
	Bt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));

	if(Rt!=NULL && Gt!=NULL && Bt!=NULL){
		for(i=0; i<tam_h; i++){
			*(Rt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));
			*(Gt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));
			*(Bt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));

			if(*(Rt+i)==NULL || *(Gt+i)==NULL || *(Bt+i)==NULL){
				erro=0xA1;
				break;
			}
		}
	}else{
		erro=0xA0;
	}

	if(erro==0){ // Só continua se não houver nenhum erro na execução
		// Copia valores RGB para serem processados

		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				*(*(Rt+i+janela/2)+j+janela/2) = *(*(Ri+i)+j);
				*(*(Gt+i+janela/2)+j+janela/2) = *(*(Gi+i)+j);
				*(*(Bt+i+janela/2)+j+janela/2) = *(*(Bi+i)+j);
			}
		}

		int Rtotal=0, Gtotal=0, Btotal=0;
		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				// Faz a media de um quadrado janela X janela ao redor do elemento selecionado das matrizes t
				Rtotal=0;
				Btotal=0;
				Gtotal=0;
				for(x=-janela/2; (janela%2==0)? x<=janela/2-1 : x<=janela/2; x++){ // casos de janela par e janela ímpar
					for(y=-janela/2; (janela%2==0)? y<=janela/2-1 : y<=janela/2; y++){
						Rtotal += *(*(Rt+i+x+janela/2)+j+y+janela/2);
						Gtotal += *(*(Gt+i+x+janela/2)+j+y+janela/2);
						Btotal += *(*(Bt+i+x+janela/2)+j+y+janela/2);
					}
				}

				// Salva os valores obtidos
				*(*(Rf+i)+j) = Rtotal/(janela*janela);
				*(*(Gf+i)+j) = Gtotal/(janela*janela);
				*(*(Bf+i)+j) = Btotal/(janela*janela);
			}
		}
	}

	// Desaloca a memoria
	for(i=0; i<altura; i++){
		free(*(Rt+i));
		free(*(Gt+i));
		free(*(Bt+i));
	}

	free(Rt); free(Gt); free(Bt);

	return erro;
}

int rgb_filtro_mediana(unsigned char **Ri, unsigned char **Gi, unsigned char **Bi, unsigned char **Rf, unsigned char **Gf, unsigned char **Bf, int janela, int largura, int altura){
	// Erros
	// 0xA_ - Erros de alocação dinâmica
	unsigned char **Rt, **Gt, **Bt; // Matrizes temporarias
	int i, j, x, y;
	int erro=0;
	int tam_h = (janela%2==0)? altura+janela : altura+janela-1; // Diferenciação para casos pares e ímpares
	int tam_v = (janela%2==0)? largura+janela : largura+janela-1;

	Rt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));
	Gt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));
	Bt = (unsigned char**) calloc(tam_h, sizeof(unsigned char*));

	if(Rt!=NULL && Gt!=NULL && Bt!=NULL){
		for(i=0; i<tam_h; i++){
			*(Rt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));
			*(Gt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));
			*(Bt+i) = (unsigned char*) calloc(tam_v, sizeof(unsigned char));

			if(*(Rt+i)==NULL || *(Gt+i)==NULL || *(Bt+i)==NULL){
				erro=0xA1;
				break;
			}
		}
	}else{
		erro=0xA0;
	}

	if(erro==0){ // Só continua se não houver nenhum erro na execução
		// Copia valores RGB para serem processados

		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				*(*(Rt+i+janela/2)+j+janela/2) = *(*(Ri+i)+j);
				*(*(Gt+i+janela/2)+j+janela/2) = *(*(Gi+i)+j);
				*(*(Bt+i+janela/2)+j+janela/2) = *(*(Bi+i)+j);
			}
		}

		unsigned char *Rvet, *Gvet, *Bvet;
		Rvet = (unsigned char*) calloc(janela*janela, sizeof(unsigned char));
		Bvet = (unsigned char*) calloc(janela*janela, sizeof(unsigned char));
		Gvet = (unsigned char*) calloc(janela*janela, sizeof(unsigned char));
		int m=0;
		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				// Salva os elementos dos vetores para calcular a mediana
				m=0;	
				for(x=-janela/2; (janela%2==0)? x<=janela/2-1 : x<=janela/2; x++){ // casos de janela par e janela ímpar
					for(y=-janela/2; (janela%2==0)? y<=janela/2-1 : y<=janela/2; y++){
						*(Rvet+m) = *(*(Rt+i+x+janela/2)+j+y+janela/2);
						*(Gvet+m) = *(*(Gt+i+x+janela/2)+j+y+janela/2);
						*(Bvet+m) = *(*(Bt+i+x+janela/2)+j+y+janela/2);
						m++;
					}
				}

				quick_sort(Rvet, 0, janela*janela-1);
				quick_sort(Gvet, 0, janela*janela-1);
				quick_sort(Bvet, 0, janela*janela-1);

				// Salva os valores obtidos
				// Caso a quantidade de elementos seja par, deve ser feita a média dos elementos centrais
				*(*(Rf+i)+j) = (janela%2==0)? (*(Rvet+janela*janela/2) + *(Rvet-1+janela*janela/2))/2 : *(Rvet+janela*janela/2);
				*(*(Gf+i)+j) = (janela%2==0)? (*(Gvet+janela*janela/2) + *(Gvet-1+janela*janela/2))/2 : *(Gvet+janela*janela/2);
				*(*(Bf+i)+j) = (janela%2==0)? (*(Bvet+janela*janela/2) + *(Bvet-1+janela*janela/2))/2 : *(Bvet+janela*janela/2);
			}
		}

		free(Rvet); free(Bvet); free(Gvet);
	}

	// Desaloca a memoria
	for(i=0; i<altura; i++){
		free(*(Rt+i));
		free(*(Gt+i));
		free(*(Bt+i));
	}

	free(Rt); free(Gt); free(Bt);

	return erro;
}

void quick_sort(unsigned char *vet, int inicio, int fim){
	unsigned char pivo = vet[(inicio+fim)/2];
	int i=inicio, j=fim;
	unsigned char temp;

	while(i<j){
		while(vet[i]<pivo) i++;
		while(vet[j]>pivo) j--;

		if(i<=j){
			temp = vet[i];
			vet[i] = vet[j];
			vet[j] = temp;
			i++;
			j--;
		}
	}

	if(j>inicio) quick_sort(vet, inicio, j);
	if(i<fim) quick_sort(vet, i, fim);
}