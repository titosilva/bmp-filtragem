// Este arquivo contem funcoes que extraem informações do arquivo bitmap
#ifndef BMP_EXT_H
#define BMP_EXT_H

typedef struct bmp_stc{
	char identifier[2];
	unsigned  int file_size;
	unsigned  int reserved;
	unsigned  int data_offset;
	unsigned  int header;
	unsigned  int width;
	unsigned  int height;
	unsigned short int planes;
	unsigned short int bits_in_pixel;
	unsigned  int BI_RGB;
	unsigned  int raw;
	unsigned  int h_res;
	unsigned  int v_res;
	unsigned  int pallete;
	unsigned  int imp_colors;
	char *source; // Guarda o nome do arquivo de origem

	unsigned char **matR;
	unsigned char **matG;
	unsigned char **matB;
}image;

// Salva os dados do cabeçalho do arquivo em uma struct imagem, alem de salvar os valores rgb em tres matrizes
int openBMP(char const *file_name, image *bmp); 
// Constroi um arquivo bmp apartir de matriz rgb, uma variavel bmp e um nome para o novo arquivo
int makeBMP(unsigned char **R, unsigned char **G, unsigned char **B, image bmp, char const *name); 
// Libera o espaço alocado para salvar o nome do arquivo de origem e para as matrizes rgb
int endBMP(image *bmp); 

#endif