#ifndef __listeHisto_h__
#define __listeHisto_h__

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <math.h>
//#include <string>

#define TYPE1 ".dat"
#define nonTYPE1 ".dat~"

#define TYPE2 ".dsc"
#define nonTYPE2 ".dsc~"


using namespace std;

class listeHisto
{
	public :
		listeHisto(string monRepertoire, int monNbBin);
		listeHisto(string monRepertoire);
		~listeHisto(void);

		bool suivant(void);
		bool suivant(bool affiche);

		float * valeurs(string balise);
		float * valeurs(string balise, int monNbBin);
		float * coeff(int nbHarmo, int nbCoeff);
		string nom(void);

		bool estFinRep(void) {return finListe;}
	
	protected :
		bool ouvrirHisto(void);
		void fermerHisto(void);

	private :
		DIR* pRep;
		dirent * pFichier;
		int nbBin;
		string repertoire;
		ifstream *lire;
		bool finListe;
};

#endif

