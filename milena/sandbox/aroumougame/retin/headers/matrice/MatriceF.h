#include <vector>
#include <iostream>
#include <stdlib.h>
#include "matrice/valeursVecteursPropres.h"

using namespace std;

class MatriceF
{
	public:
	MatriceF(int taille);				// matrice symetrique
	MatriceF(int tailleLignes, int tailleColonnes);
	MatriceF(int taille, float * valeur );		// initialisation matrice symetrique diagonale puis triangulaire sup lig par lig
	MatriceF(int tailleLignes, int tailleColonnes, float *valeur);	// initialisation matrice ligne par ligne
	~MatriceF();

	void affiche();
	void diagonalisation(MatriceF *diago, MatriceF *vecteurs );

	float obtValeur(int position1, int position2);
	int obtNbColonnes();
	int obtNbLignes();
	void obtValeurs(float *valeur); // attention: allouer de la memoire pour valeur = new float [nbLignes*nbColonnes];
	void obtDiagonale(float *diago);
	void obtDirectionsPrincipales(float *direction1, float *direction2);
	bool isDiagonalePositive();

	void chgValeurs(float *valeur);
	void chgDiagonale(float *valeur);
		

	private:
		int nbLignes, nbColonnes;
		vector < vector<float> > matrice;
};
