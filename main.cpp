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
		Mat ErosaoMorfologiaMatematica(Mat ImagemBase);
		Mat ExpansaoMorfologiaMatematicaAdjacente(Mat ImagemBase);
		Mat ErosaoMorfologiaMatematicaAdjacente(Mat ImagemBase);
		Mat DiferencaImagens(Mat ImagemBase, Mat ImagemDiff);
		pair<Mat, int> SeguirFronteiras(Mat ImagemBase);
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
Mat AnalisarImagem::ErosaoMorfologiaMatematica(Mat ImagemBase)
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
			uchar Mini=255;
			// loop da região em volta do kernel
			for(int KLin=Max(0, Lin-Ko) ; KLin<Min(Hei, Lin+Ko+1) ; KLin++)
			{
				for(int KCol=Max(0, Col-Ko) ; KCol<Min(Wid, Col+Ko+1) ; KCol++)
				{
					if(ImagemBase.at<uchar>(KLin, KCol) < Mini)
					{
						Mini = ImagemBase.at<uchar>(KLin, KCol);
					}
				}
			}
			// definir intensidade do pixel da imagem de retorno
			Retorno.at<uchar>(Lin, Col) = Mini;
		}
	}
	return Retorno;
}
Mat AnalisarImagem::ExpansaoMorfologiaMatematicaAdjacente(Mat ImagemBase)
{
    // variaveis
    int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
	Mat Retorno = Mat::zeros(Size(Wid, Hei), CV_8UC1);

	// operação principal, loop por todos os pixels
	for(int Lin=1 ; Lin<Hei-1 ; Lin++)
	{
		for(int Col=1 ; Col<Wid-1 ; Col++)
		{
			// obter maior valor da região
			uchar Maxi=0;
			// checar região em volta do kernel
			if(Maxi < ImagemBase.at<uchar>(Lin, Col)){Maxi = ImagemBase.at<uchar>(Lin, Col);}
			if(Maxi < ImagemBase.at<uchar>(Lin-1, Col)){Maxi = ImagemBase.at<uchar>(Lin-1, Col);}
			if(Maxi < ImagemBase.at<uchar>(Lin, Col-1)){Maxi = ImagemBase.at<uchar>(Lin, Col-1);}
			if(Maxi < ImagemBase.at<uchar>(Lin+1, Col)){Maxi = ImagemBase.at<uchar>(Lin+1, Col);}
			if(Maxi < ImagemBase.at<uchar>(Lin, Col+1)){Maxi = ImagemBase.at<uchar>(Lin, Col+1);}
			// definir intensidade do pixel da imagem de retorno
			Retorno.at<uchar>(Lin, Col) = Maxi;
		}
	}
	return Retorno;
}
Mat AnalisarImagem::ErosaoMorfologiaMatematicaAdjacente(Mat ImagemBase)
{
    // variaveis
    int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
    int KernelSize = 3;
	int Ko = KernelSize/2;
	Mat Retorno = Mat::zeros(Size(Wid, Hei), CV_8UC1);

	// operação principal, loop por todos os pixels
	for(int Lin=1 ; Lin<Hei-1 ; Lin++)
	{
		for(int Col=1 ; Col<Wid-1 ; Col++)
		{
		// obter maior valor da região
		uchar Mini=255;
		// checar região em volta do kernel
		if(Mini > ImagemBase.at<uchar>(Lin, Col)){Mini = ImagemBase.at<uchar>(Lin, Col);}
		if(Mini > ImagemBase.at<uchar>(Lin-1, Col)){Mini = ImagemBase.at<uchar>(Lin-1, Col);}
		if(Mini > ImagemBase.at<uchar>(Lin, Col-1)){Mini = ImagemBase.at<uchar>(Lin, Col-1);}
		if(Mini > ImagemBase.at<uchar>(Lin+1, Col)){Mini = ImagemBase.at<uchar>(Lin+1, Col);}
		if(Mini > ImagemBase.at<uchar>(Lin, Col+1)){Mini = ImagemBase.at<uchar>(Lin, Col+1);}
		// definir intensidade do pixel da imagem de retorno
		Retorno.at<uchar>(Lin, Col) = Mini;
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
pair<Mat, int> AnalisarImagem::SeguirFronteiras(Mat ImagemBase)
{

	int Wid = ImagemBase.cols;
    int Hei = ImagemBase.rows;
	vector<int> b0 = {0,0};
	vector<int> c0 = {0,0};

	// Passo 1
	// b0 é o ponto mais superior e mais a esquerda dado como 1
	for(int Lin=0 ; Lin<Hei ; Lin++)
	{
		for(int Col=0 ; Col<Wid ; Col++)
		{
			if (ImagemBase.at<Vec3b>(Lin, Col) == Vec3b(0, 0, 255))
			{
				b0 = {Lin, Col};
				printf("b0: %d, %d\n", b0[0], b0[1]);
				// c0 é o vizinho esquerdo de b0
				c0 = {Lin, Col-1};
				printf("c0: %d, %d\n", c0[0], c0[1]);
				Lin = Hei;
				Col = Wid;
				break;
			}
		}
	}

	// Começa na esquerda do pixel, e vai no sentido horário
	vector<int> deslocaX = {-1, -1,  0,  1,  1,  1,  0, -1};
	vector<int> deslocaY = { 0, -1, -1, -1,  0,  1,  1,  1};

	vector<int> b = b0;
	vector<int> c = c0;

	int looppoint = 0;

	// stats
	int qtpixeisborda = 0;

	// enquanto não chegar na base
	do{

		// Examina todos os 8 vizinhos de b0, começando por c0 e indo no sentido horário
		for(int i=looppoint ; i<999999999 ; i++)
		{
    		// loop around
            //cout << "look mom, im looping! i=" << i << endl;
    		if(i >= 8){i = 0;}
		    // Obter ponto de comparação
			c = {b[0]+deslocaY[i], b[1]+deslocaX[i]};
			// Comparar :D
			if(ImagemBase.at<Vec3b>(c[0], c[1]) == Vec3b(0, 0, 255)  || c[0] == b0[0] && c[1] == b0[1])
			{
				// looppint = i - 1 com correção para negativo :D
			    looppoint = (i + 7) % 8;
			    break;
			}
		}
		// Atualizar pontos
		vector<int> copiab = {b[0], b[1]};
		b = {c[0], c[1]};
		c = {copiab[0] + deslocaY[looppoint], copiab[1] + deslocaX[looppoint]};

		// Vetor que indica o novo ponto inicial
		vector<int> vetorBC = {c[0] - b[0], c[1] - b[1]};

		// laço para descobrir o novo ponto incial
		for(int j = 0; j < 8; j++)
		{
			if(vetorBC[0] == deslocaY[j] && vetorBC[1] == deslocaX[j])
			{
				looppoint = (j + 7) % 8;
				break;
			}
		}

		// Salvar Stats
		qtpixeisborda += 1;
		// Desenhar na imagem :D
		ImagemBase.at<Vec3b>(copiab[0], copiab[1]) = Vec3b(255, 0, 0);

	} while((b[0] != b0[0]) || (b[1] != b0[1]));

	return {ImagemBase, qtpixeisborda};
}

int main()
{
	AnalisarImagem Analisador;
	Mat img = Analisador.AbrirPreProcessar(ImagensPaths[15]);
	// lidar com ruido a base do ódio de classe
	img = Analisador.ExpansaoMorfologiaMatematica(img);
	img = Analisador.ExpansaoMorfologiaMatematica(img);
	img = Analisador.ExpansaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ErosaoMorfologiaMatematica(img);
	img = Analisador.ExpansaoMorfologiaMatematica(img);
	img = Analisador.ExpansaoMorfologiaMatematica(img);
	img = Analisador.ExpansaoMorfologiaMatematica(img);

	Mat imgexp = Analisador.ErosaoMorfologiaMatematicaAdjacente(img);
	Mat diff = Analisador.DiferencaImagens(img, imgexp);
	imshow("Diff", diff);
	waitKey(0);
	pair<Mat, int> borda = Analisador.SeguirFronteiras(diff);
	imshow("Borda", borda.first);
	waitKey(0);

	pair<Mat, int> newl = Analisador.SeguirFronteiras(borda.first);
	imshow("Borda", newl.first);
	waitKey(0);
	printf("Quantidade de pixels na borda: %d\n", borda.second);
}
