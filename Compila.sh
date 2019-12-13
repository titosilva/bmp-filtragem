#!/bin/bash

echo "Criando arquivo executavel"
gcc -o BMP ./Arquivos/principal.c ./Arquivos/bmp_ext.c ./Arquivos/rgb_filt.c

if [ -x BMP ]
	then
		echo "Arquivo gerado: BMP" && echo "Para executa-lo, use" && echo "./BMP [argumentos]"
		echo "Finalizado!"
	else
		echo "Ocorreu uma falha ao tentar criar o executavel..."
fi
