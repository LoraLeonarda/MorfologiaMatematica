# Trabalho de PID sobre morfologia matemática

Objetivos:
- Marcar a fronteira das folhas com a cor vermelha através da morfologia matemática
- Informar quantas folhas há na imagem
- Informar o número de pixels da borda de cada folha e totais
- Informar o número de pixels existentes na fronteira de cada folha
- Responder: é possível identificar a espécie da planta usando apenas este descritor de fronteiras (número de pixels)?

Detalhes de implementação:
- OpenCV para abrir imagem
- Morfologia matemática para identificar borda, implementação manual
- Algoritmo seguidor de fronteiras para percorrer os pixels da mesma, definido nas páginas 814 a 816 do livro
- Fluxo da interface
  - Para cada folha na imagem
    - Mostrar imagem com a borda vermelha da folha, junto com:
	  - Contador de pixels na borda da folha
	  - Se possível, a espécie
  - Ao final, mostrar todas as bordas vermelhas, e estatísticas gerais:
	- Contador de folhas
    - Contador global de pixels nas bordas
