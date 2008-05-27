#ifndef __Histogramme_h__
#define __Histogramme_h__

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "WRL/calcul.h"

#ifndef PI
#define PI 3.1415926535897932
#endif

using namespace std;

class Histogramme
{
	public :

	Histogramme(int monNbBin);
	Histogramme(int monNbBin, int nbValeurs, int *valeursNormees);
	Histogramme(int monNbBin, int nbValeurs, float *valeurs);
	Histogramme(vector<int>  valeurs);
	Histogramme(vector<float>  valeurs);
	Histogramme(int monNbBin, unsigned long *valeurs);
	Histogramme(vector<float>  valeurs, bool normalisation);
	Histogramme(int monNbBin, int nbValeurs, float *valeurs, float valeurBinMax);
	~Histogramme(void);
	Histogramme(vector < vector <float> > valeurs,vector <float> max, int nbBin, bool Aire,vector <float> Aires );
	Histogramme(vector<int>  valeurs, int histSize, bool Aire, vector <float> Aires);
	void sauve(const char *nom);
	void sauve(ofstream *fout , const char *balise);
	void plot(const char *nom);
	float* obValeurs(void){return hist;} 
	void Normaliser();
	
	void affiche(void);
	float testSommeHistogramme(void);

	protected :

	void maxTableau(float *tab, int nbElement, int *indiceMax, float *valeurMax);
	void maxTableau(int *tab, int nbElement, int *indiceMax, int *valeurMax);
	void minTableau(int *tab, int nbElement, int *indiceMin, int *valeurMin);
	void NormalisationQuantification(float *tab, int nbElement, float valeurMax, int *nvTab);

	private :

	float *hist;
	int nbBin;

};



#endif

