// Esse arquivo contem funções que ajudam a manusear matrizes RGB e fazer filtragens
#ifndef RGB_HANDLE_H
#define RGB_HANDLE_H


//Atenção: todas matrizes f devem ter o mesmo tamanho que as matrizes i
// Filtragem usando media
// Pega os valores da matriz Ri e aplica filtragem com janela, salvando o resultado em Rf
int rgb_filtro_media(unsigned char **Ri,  unsigned char **Gi, unsigned char **Bi, unsigned char **Rf, unsigned char **Gf, unsigned char **Bf, int janela, int largura, int altura);
// Filtragem usando mediana
int rgb_filtro_mediana(unsigned char **Ri, unsigned char **Gi, unsigned char **Bi, unsigned char **Rf, unsigned char **Gf, unsigned char **Bf, int janela, int largura, int altura);
// Quick sort para mediana
void quick_sort(unsigned char *vet, int inicio, int fim);

#endif