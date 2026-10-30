# Trabalho de Processamento de Imagens Digitais - Parte 01

Alunos:
- Ellen Christini Schreiber Brzozoski
- Lóra Leonarda Lívero Lavaqui
- João Vitor da Silva

## Objetivos:
- Marcar a fronteira das folhas com a cor vermelha através de morfologia matemática
- Informar quantas folhas há na imagem
- Informar o número de pixels da borda de cada folha e totais
- Responder: é possível identificar a espécie da planta usando apenas este descritor de fronteiras (número de pixels)?

## Detalhes de implementação:
- OpenCV para abrir imagem
- Morfologia matemática para identificar borda, implementação manual
- Algoritmo seguidor de fronteiras para percorrer os pixels da mesma, definido nas páginas 814 a 816 do [Livro](https://www.cl72.org/090imagePLib/books/Gonzales,Woods-Digital.Image.Processing.4th.Edition.pdf)

## Como executar:

### Instalação da Biblioteca (Linux)

- Execute o comando abaixo para instalar a biblioteca OpenCV

```bash
  sudo apt install libopencv-dev g++ cmake
```

### Compilação e Execução do algoritmo

- Para compilar, rode o comando abaixo:

```bash
  bash compile.sh
```

- Execute utilizando o comando:

```bash
  ./main
```

- Escolha a imagem a ser analisada, pelo índice de 0-34, e depois aperte uma tecla para observar cada folha tendo sua borda pintada





