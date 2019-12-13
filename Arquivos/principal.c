// Trabalho de APC - Questao 1

/*
	O programa é baseado nos arquivos bmp_ext, responsavel por extrair informações de arquivos bmp,
	e rgb_filt, responsavel por aplicar filtros de mediana ou de media

	No arquivo bmp_ext, ha tanto a parte de tirar as informações do cabeçalho do arquivo quanto de 
	criar as matriz rgb correspondentes à imagem. Essa matrizes possuem endereço salvo na struct correspondente.
*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp_ext.h"
#include "rgb_filt.h"
#include <string.h>

int main(int argc, char const *argv[]){
	image img;
	int erro=0;
	int i;

	if(argc!=5){
		printf("Sintaxe incorreta\nOs argumentos devem ser:\n");
		printf("Argumento 1: nome do arquivo bmp de origem\n");
		printf("Argumento 2: nome do arquivo bmp a ser gerado\n");
		printf("Argumento 3: tamanho da janela de filtragem a ser utilizada\n");
		printf("Argumento 4: modo de filto(media=1, mediana=2)\n");
	}else{
		image img;
		if(erro = openBMP(argv[1], &img)){
			printf("Ocorreu um erro: 0x%x", erro);
		}else{
			// Exibe informações na tela
			printf("Magic numbers:\t%s\n", 					img.identifier 	);
			printf("Tamanho do arquivo:\t%d\n", 			img.file_size  	);
			printf("Data offset:\t%d\n", 					img.data_offset	);
			printf("Tamanho do cabeçalho:\t%d\n", 			img.header     	);
			printf("Largura:\t%d\n", 						img.width      	);
			printf("Altura:\t%d\n", 						img.height     	);
			printf("Planos:\t%hd\n", 						img.planes     	);
			printf("Bits por pixel:\t%hd\n", 				img.bits_in_pixel);
			printf("Compressao:\t%d\n", 					img.BI_RGB     	);
			printf("Tamanho(raw):\t%d\n", 					img.raw        	);
			printf("Resolução horizontal:\t%d\n", 			img.h_res      	);
			printf("Resolução vertical:\t%d\n", 			img.v_res      	);
			printf("Pallete:\t%d\n", 						img.pallete    	);
			printf("Means all colors are imporant:\t%d\n", 	img.imp_colors 	);
			printf("Arquivo de origem: %s\n", 				img.source		);
			printf("\n");


			if(atoi(argv[3])%2!=0){
				// Matrizes que serão utilizadas para guardar os resultados das filtragens
				printf("Processando filtro...\n");
				unsigned char **R, **G, **B;

				R = (unsigned char**) calloc(img.height, sizeof(unsigned char*));
				G = (unsigned char**) calloc(img.height, sizeof(unsigned char*));
				B = (unsigned char**) calloc(img.height, sizeof(unsigned char*));

				if(R!=NULL && G!=NULL && B!=NULL){
					for(i=0; i<img.height; i++){
						*(R+i) = (unsigned char*) calloc(img.width, sizeof(unsigned char));
						*(G+i) = (unsigned char*) calloc(img.width, sizeof(unsigned char));
						*(B+i) = (unsigned char*) calloc(img.width, sizeof(unsigned char));

						if(*(R+i)==NULL || *(G+i)==NULL || *(B+i)==NULL){
							erro=0xC1;
							break;
						}
					}
				}else{
					erro=0xC0;
				}

				if(atoi(argv[3])==0){
					printf("Argumento invalido\n");
					erro=0xD1;
				}

			 	if(erro==0){
			 		switch(atoi(argv[4])){
			 			case 1:
			 				erro = rgb_filtro_media(img.matR, img.matG, img.matB, R, G, B, atoi(argv[3]), img.width, img.height);
			 				break;
			 			case 2:
			 				erro = rgb_filtro_mediana(img.matR, img.matG, img.matB, R, G, B, atoi(argv[3]), img.width, img.height);
			 				break;
			 			default:
			 				printf("Argumento invalido para o modo de filtragem\nUtilize 1 para media e 2 para mediana\n");
			 				erro = 0x01;
			 		}
			 	}

			 	if(erro==0){
			 		erro=makeBMP(R, G, B, img, argv[2]);
			 	}

			 	if(erro==0){
			 		printf("Finalizado!\n");
			 	}else{
			 		printf("Ocorreu um erro: 0x%x\n", erro);
			 	}

			 	for(i=0; i<img.height;i++){
			 		free(*(R+i));
			 		free(*(G+i));
			 		free(*(B+i));
			 	}
			 	free(R); free(G); free(B);

			}else{ // se o tamanho de janela passado pelo usuario for par
				printf("O tamanho da janela deve ser impar\n");
			}
		 	endBMP(&img);
		}
	}

	return 0;
}