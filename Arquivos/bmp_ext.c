// Arquivo C de bmp_extract
// Implementação de funções que possibilitam a leitura do arquivo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_ext.h"

int openBMP(char const *file_name, image *bmp){ // Salva o cabeçalho em uma struct
	// Se o retorno for -1, então houve um problema com o arquivo
	// Se o retorno for maior que zero, houve algum outro problema
	// No segundo caso, o número retornado indica qual foi o problema
	// Os problemas do tipo 0xA_ são problemas de leitura das informações do cabeçalho
	// Os problemas do tipo 0xB_ são problemas de alocação dinâmica
	// Os problemas do tipo 0xC_ são problemas com leitura dos valores RGB
	FILE *fp;
	int erro = 0;
	fp = fopen(file_name, "rb");

	if(fp==NULL){
		return -1;
	}else{
		// DADOS PARA A STRUCT ---------------------------------------------
		bmp->source = (char*) calloc(strlen(file_name), sizeof(char));
		strcpy(bmp->source, file_name); // Salva o nome do arquivo de origem

		if(fread(bmp->identifier, 		1, 		2, 	fp)!=2) erro = 0xA1;
		if(fread(&bmp->file_size, 		4, 		1, 	fp)!=1) erro = 0xA2;
		if(fread(&bmp->reserved, 		4,		1,	fp)!=1) erro = 0xA3;
		if(fread(&bmp->data_offset, 	4, 		1, 	fp)!=1) erro = 0xA4;
		if(fread(&bmp->header,			4, 		1, 	fp)!=1) erro = 0xA5;
		if(fread(&bmp->width, 			4, 		1, 	fp)!=1) erro = 0xA6;
		if(fread(&bmp->height, 			4, 		1, 	fp)!=1) erro = 0xA7;
		if(fread(&bmp->planes, 			2, 		1, 	fp)!=1) erro = 0xA8;
		if(fread(&bmp->bits_in_pixel, 	2, 		1, 	fp)!=1) erro = 0xA9;
		if(fread(&bmp->BI_RGB, 			4, 		1, 	fp)!=1) erro = 0xAA;
		if(fread(&bmp->raw, 			4,		1, 	fp)!=1) erro = 0xAB;
		if(fread(&bmp->h_res, 			4,		1, 	fp)!=1) erro = 0xAC;
		if(fread(&bmp->v_res, 			4,		1, 	fp)!=1) erro = 0xAD;
		if(fread(&bmp->pallete, 		4,		1, 	fp)!=1) erro = 0xAE;
		if(fread(&bmp->imp_colors,	 	4,		1, 	fp)!=1) erro = 0xAF;

		if(erro!=0) return erro;
	}

	if(erro==0){ // Só continua se foi possivel fazer as leituras do cabeçalho
		// Alocação dimâmica
		bmp->matR = (unsigned char**) calloc(bmp->height, sizeof(unsigned char*));
		bmp->matG = (unsigned char**) calloc(bmp->height, sizeof(unsigned char*));
		bmp->matB = (unsigned char**) calloc(bmp->height, sizeof(unsigned char*));

		int i;
		if(bmp->matR!=NULL && bmp->matB!=NULL && bmp->matG!=NULL){
			for(i=0; i<bmp->height; i++){
				*(bmp->matR+i) = (unsigned char*) calloc(bmp->width, sizeof(unsigned char));
				*(bmp->matG+i) = (unsigned char*) calloc(bmp->width, sizeof(unsigned char));
				*(bmp->matB+i) = (unsigned char*) calloc(bmp->width, sizeof(unsigned char));

				if(*(bmp->matR+i)==NULL || *(bmp->matG+i)==NULL || *(bmp->matB+i)==NULL){
					return erro=0xB2;
					break;
				} 
			}
		}else{
			return erro=0xB1;
		}
	}

	if(erro==0){ // Para se houve falha em algum ponto da execução;
		int i, j, padding;
		int lixo, count=0;
		padding = ((bmp->width*3)%4==0)? 0 : 4-((bmp->width*3)%4); 

		// salva os valores RGB vindos do arquivo
		for(i=0; i<bmp->height && erro==0; i++){
			for(j=0; j<bmp->width && erro==0; j++){
				if(fread(*(bmp->matB+i)+j, 1, 1, fp)!=1) erro=0xC1;
				if(fread(*(bmp->matG+i)+j, 1, 1, fp)!=1) erro=0xC2;
				if(fread(*(bmp->matR+i)+j, 1, 1, fp)!=1) erro=0xC3;
			}

			for(j=0; j<padding && erro==0; j++){
				if(fread(&lixo, 1, 1, fp)!=1){
					printf("Count: %d\n", count);
					erro = 0xC0;
					break;
				}else{
				// printf("ok\n");
				}
			}
			count++;
		}

		return erro;
	}

	fclose(fp);
}

int makeBMP(unsigned char **R, unsigned char **G, unsigned char **B, image bmp, char const *name){
	// Retornos:
	// -1	- Houve algum problema ao tentar criar o arquivo bmp
	// 0xA_	- Problemas ao tentar escrever partes do cabeçalho
	// 0xB_	- Problemas ao tentar escrever os valores RGB
	FILE *arq;
	arq = fopen(name, "wb");
	int erro=0;

	if(arq==NULL){
		return erro = -1;
	}else{
		if(fwrite(bmp.identifier, 		1, 		2, 	arq)!=2) erro = 0xA1;
		if(fwrite(&bmp.file_size, 		4, 		1, 	arq)!=1) erro = 0xA2;
		if(fwrite(&bmp.reserved, 		4,		1,	arq)!=1) erro = 0xA3;
		if(fwrite(&bmp.data_offset, 	4, 		1, 	arq)!=1) erro = 0xA4;
		if(fwrite(&bmp.header,			4, 		1, 	arq)!=1) erro = 0xA5;
		if(fwrite(&bmp.width, 			4, 		1, 	arq)!=1) erro = 0xA6;
		if(fwrite(&bmp.height, 			4, 		1, 	arq)!=1) erro = 0xA7;
		if(fwrite(&bmp.planes, 			2, 		1, 	arq)!=1) erro = 0xA8;
		if(fwrite(&bmp.bits_in_pixel, 	2, 		1, 	arq)!=1) erro = 0xA9;
		if(fwrite(&bmp.BI_RGB, 			4, 		1, 	arq)!=1) erro = 0xAA;
		if(fwrite(&bmp.raw, 			4,		1, 	arq)!=1) erro = 0xAB;
		if(fwrite(&bmp.h_res, 			4,		1, 	arq)!=1) erro = 0xAC;
		if(fwrite(&bmp.v_res, 			4,		1, 	arq)!=1) erro = 0xAD;
		if(fwrite(&bmp.pallete,			4,		1, 	arq)!=1) erro = 0xAE;
		if(fwrite(&bmp.imp_colors,	 	4,		1, 	arq)!=1) erro = 0xAF;

		if(erro==0){ // Verifica se não houve nenhum erro até aqui
			int i, j, padding;
			unsigned char zero=0;
			padding = ((bmp.width*3)%4==0)? 0 : 4-((bmp.width*3)%4); 

			for(i=0; i<bmp.height && erro==0; i++){
				for(j=0; j<bmp.width && erro==0; j++){
					if(fwrite(*(B+i)+j,	1,	1,	arq)!=1) erro = 0xB1;
					if(fwrite(*(G+i)+j,	1,	1,	arq)!=1) erro = 0xB2;
					if(fwrite(*(R+i)+j,	1,	1,	arq)!=1) erro = 0xB3;
				}

				for(j=0; j<padding; j++){
					if(fwrite(&zero, 1, 1, arq)!=1) erro=0xB4;
				}
			}

			return erro;
		}else{
			return erro;
		}
	}

	fclose(arq);
}

int endBMP(image *bmp){ // Libera memória alocada
	free(bmp->source);

	int i;
	for(i=0; i<bmp->height; i++){
		free(*(bmp->matR+i));
	}
	free(bmp->matR);

	for(i=0; i<bmp->height; i++){
		free(*(bmp->matG+i));
	}
	free(bmp->matG);

	for(i=0; i<bmp->height; i++){
		free(*(bmp->matB+i));
	}
	free(bmp->matB);
}
