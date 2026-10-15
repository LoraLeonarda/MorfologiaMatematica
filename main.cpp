// Libs
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
// Eu te odeio c++
using namespace std;
using namespace cv;
typedef unsigned char byte;

// -----------------------------
// CLASSE DE ANALISE
// -----------------------------
class AnalisarImagem
{
	private:
		Mat PreProcessar(Mat ImagemBase);
		Mat ExpansaoMorfologiaMatematica(Mat ImagemBase);
		Mat DiferencaImagens(Mat ImagemBase, Mat ImagemDiff);
		vector<int> SeguirFronteiras(Mat ImagemBase);
	public:
		//
};

// -----------------------------
// IMPLEMENTAÇÃO DOS MÉTODOS
// -----------------------------
Mat AnalisarImagem::PreProcessar(Mat ImagemBase)
{
	Mat Retorno;
	// TODO
	return Retorno;
}
Mat AnalisarImagem::ExpansaoMorfologiaMatematica(Mat ImagemBase)
{
	Mat Retorno;
	// TODO
	return Retorno;
}
Mat AnalisarImagem::DiferencaImagens(Mat ImagemBase, Mat ImagemDiff)
{
	Mat Retorno;
	// TODO
	return Retorno;
}
vector<int> AnalisarImagem::SeguirFronteiras(Mat ImagemBase)
{
	vector<int> Retorno;
	// TODO
	return Retorno;
}

int main()
{
	cout << "aaa" << endl;
}