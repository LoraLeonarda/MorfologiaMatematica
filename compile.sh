#!/bin/bash
echo "Compilando..."

# Se já foi compilado, deleta arquivo
if [ -e main ]
then
    rm main
	echo ""
else
    echo ""
fi

# Compilando!
g++ main.cpp -O2 -o main $(pkg-config --cflags --libs opencv4)

# Check de compilagem!
if [ -e main ]
then
	echo "Compilado com sucesso!"
	echo "Execute com: ./main <imagem.bmp>"
else
    echo "Erro na compilação, verifique os erros acima."
fi
