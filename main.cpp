// Libs
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
// Eu te odeio c++
using namespace std;
using namespace cv;

// Paths de todas as imagens
vector<string> ImagensPaths = {
	"Imagens/IMG_20230504_090659.bmp",
	"Imagens/IMG_20230504_090748.bmp",
	"Imagens/IMG_20230504_090851.bmp",
	"Imagens/IMG_20230504_090854.bmp",
	"Imagens/IMG_20230504_090900.bmp",
	"Imagens/IMG_20230504_090908.bmp",
	"Imagens/IMG_20230504_091023.bmp",
	"Imagens/IMG_20230504_091027.bmp",
	"Imagens/IMG_20230504_091033.bmp",
	"Imagens/IMG_20230504_091037.bmp",
	"Imagens/IMG_20230504_091116.bmp",
	"Imagens/IMG_20230504_091121.bmp",
	"Imagens/IMG_20230504_091129.bmp",
	"Imagens/IMG_20230504_091133.bmp",
	"Imagens/IMG_20230504_091140.bmp",
	"Imagens/IMG_20230504_091235.bmp",
	"Imagens/IMG_20230504_091238.bmp",
	"Imagens/IMG_20230504_091241.bmp",
	"Imagens/IMG_20230504_091243.bmp",
	"Imagens/IMG_20230504_091321.bmp",
	"Imagens/IMG_20230504_091322.bmp",
	"Imagens/IMG_20230504_091325.bmp",
	"Imagens/IMG_20230504_091328.bmp",
	"Imagens/IMG_20230504_091331.bmp",
	"Imagens/IMG_20230504_091412.bmp",
	"Imagens/IMG_20230504_091414.bmp",
	"Imagens/IMG_20230504_091416.bmp",
	"Imagens/IMG_20230504_091421.bmp",
	"Imagens/IMG_20230504_091423.bmp",
	"Imagens/IMG_20230504_091425.bmp",
	"Imagens/IMG_20230504_091459.bmp",
	"Imagens/IMG_20230504_091501.bmp",
	"Imagens/IMG_20230504_091503.bmp",
	"Imagens/IMG_20230504_091505.bmp",
	"Imagens/IMG_20230504_091510.bmp"
};

// Funções extras de auxilio
int Max(int a, int b)
{
	if(a>b)
	{
		return a;
	}
	return b;
}
int Min(int a, int b)
{
	if(a<b)
	{
		return a;
	}
	return b;
}
int lerp(int a, int b, float perc)
{
	return (int)(((float)b - (float)a)*perc + (float)a);
}

// -----------------------------
// CLASSE DE ANALISE
// -----------------------------
class AnalisarImagem
{
	private:
		//
	public:
		Mat AbrirPreProcessar(string Filename);
		Mat ExpansaoMorfologiaMatematica(Mat ImagemBase);
		Mat DiferencaImagens(Mat ImagemBase, Mat ImagemDiff);
		vector<int> SeguirFronteiras(Mat ImagemBase);
};

// -----------------------------
// IMPLEMENTAÇÃO DOS MÉTODOS
// -----------------------------
Mat AnalisarImagem::AbrirPreProcessar(string Filename)
{
	// abrir imagem e converter para grayscale
	Mat ImagemOriginal = imread(Filename, IMREAD_GRAYSCALE);

	// check!
	if(ImagemOriginal.empty())
	{
        cerr << "Imagem \"" << Filename << "\" não foi aberta" << endl;
		return Mat::zeros(Size(1, 1), CV_8UC1);
    }
	
	// limiarizar!
	int Limiar = 150;
	int Wid = ImagemOriginal.cols;
    int Hei = ImagemOriginal.rows;
	for(int Lin=0 ; Lin<Hei ; Lin++)
	{
		for(int Col=0 ; Col<Wid ; Col++)
		{
			// aplica filtros fotométricos no pixel se necessário, fins de teste
			ImagemOriginal.at<uchar>(Lin, Col) = lerp(ImagemOriginal.at<uchar>(Lin, Col), 128, 0.0);
			
			// meio termo! (aplica inversão de cores, se menor q limiar -> preto)
			if(ImagemOriginal.at<uchar>(Lin, Col) <= Limiar)
			{
				ImagemOriginal.at<uchar>(Lin, Col) = 255;
			}
			else
			{
				ImagemOriginal.at<uchar>(Lin, Col) = 0;
			}
		}
	}
	return ImagemOriginal;
}
Mat AnalisarImagem::ExpansaoMorfologiaMatematica(Mat ImagemBase)
{
    // variaveis
    int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
    int KernelSize = 3;
	int Ko = KernelSize/2;
	Mat Retorno = Mat::zeros(Size(Wid, Hei), CV_8UC1);

	// operação principal, loop por todos os pixels
	for(int Lin=0 ; Lin<Hei ; Lin++)
	{
		for(int Col=0 ; Col<Wid ; Col++)
		{
			// obter maior valor da região
			uchar Maxi=0;
			// loop da região em volta do kernel
			for(int KLin=Max(0, Lin-Ko) ; KLin<Min(Hei, Lin+Ko+1) ; KLin++)
			{
				for(int KCol=Max(0, Col-Ko) ; KCol<Min(Wid, Col+Ko+1) ; KCol++)
				{
					if(ImagemBase.at<uchar>(KLin, KCol) > Maxi)
					{
						Maxi = ImagemBase.at<uchar>(KLin, KCol);
					}
				}
			}
			// definir intensidade do pixel da imagem de retorno
			Retorno.at<uchar>(Lin, Col) = Maxi;
		}
	}
	return Retorno;
}
Mat AnalisarImagem::DiferencaImagens(Mat ImagemBase, Mat ImagemDiff)
{
	int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
	Mat Retorno = Mat::zeros(Size(Wid, Hei), CV_8UC3);
	// operação principal, loop por todos os pixels
	for(int Lin=0 ; Lin<Hei ; Lin++)
	{
		for(int Col=0 ; Col<Wid ; Col++)
		{
			// não é exatamente um diff, mas como a imagem é binária, testa com if !=
			if(ImagemBase.at<uchar>(Lin, Col) != ImagemDiff.at<uchar>(Lin, Col))
			{
				Retorno.at<Vec3b>(Lin, Col) = Vec3b(0, 0, 255);
			}
		}
	}
	return Retorno;
}
vector<int> AnalisarImagem::SeguirFronteiras(Mat ImagemBase)
{

	int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
	vector<int> b0 = (0,0)
	vector<int> c0 = (0,0)

	// Passo 1
	// b0 é o ponto mais superior e mais a esquerda dado como 1
	for(int Lin=0 ; Lin<Hei ; Lin++)
	{
		for(int Col=0 ; Col<Wid ; Col++)
		{
			if (ImagemBase.at<uchar>(Lin, Col) == 1)
			{
				b0 = (Lin, Col)
				// c0 é o vizinho esquerdo de b0
				c0 = (Lin-1, Col)
				Lin - Hei
				break;
			}
		}
	}

	// Começa na esquerda do pixel, e vai no sentido anti-horário
	deslocaX = [0, -1, -1, -1, 0, 1, 1, 1]
	deslocaY = [-1, -1, 0, 1, 1, 1, 0, -1]

	comeco_deslocamentoX = c0 - b0
	comeco_deslocamentoY = c0 - b0
	
	// Examina todos os 8 vizinhos de b0, começando por c0 e indo no sentido horário
	for(int i = 0; i < 8; i++){

	}
	

	// b1 é o primeiro vizinho encontrado cujo valor é 1
	// c1 é o ponto (fundo) imediatamente precedente de b1 na sequência

	// Armazena as localizações de b0 para o passo 5

	// Passo 2
	// Faça b = b0 e c=c0

	// Passo 3
	// Faça os 8 vizinhos de b, começando por c e indo no sentido horário, ser denotados como n1,n2...n8.
	// Encontre o primeiro vizinho dado como 1 e denote ele pelo seu nk

	//Passo 4
	// Faça b = nk e c = nk-1

	// Passo 5
	// Repita os passos 3 e 4 até que b = b0
	// A sequência de b pontos encontrada quando o algoritmo para é o conjunto dos pontos de borda ordenados

	// Segue para o próximo conjunto de bordas

	return Retorno;
}

int main()
{
	AnalisarImagem Analisador;
	Mat img = Analisador.AbrirPreProcessar(ImagensPaths[0]);
	Mat imgexp = Analisador.ExpansaoMorfologiaMatematica(img);
	Mat diff = Analisador.DiferencaImagens(img, imgexp);
	imshow("Diff", diff);
	waitKey(0);
}
