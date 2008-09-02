/***********************************************************************/
/*!
 * @file OFF.cpp
 * @date Mars. 2008
 * @author Julien Gony<BR>
 *         Université de Cergy-Pontoise<BR>
 *         ETIS - Equipe Image - UMR CNRS 8051
 *
 * @brief  Permet de lire, écrire obtenir le maillage et les points d'objet au format off
 *
 * Permet de lire, écrire obtenir le maillage et les points d'objet au format off
 *
 * @version 1.0
 */
/************************************************************************/

#include"WRL/OFF.h"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::endl;


Off::Off(const vector<float>& xSmt, const vector<float>& ySmt,const vector<float>& zSmt,const vector <int* >& mailles) : xSmt(xSmt),ySmt(ySmt),zSmt(zSmt),mailles(mailles)
{
	isValid=false;
	if((xSmt.size()!=0)&&(mailles.size()!=0)&&(xSmt.size()==ySmt.size())&&(xSmt.size()==zSmt.size()))
	{
		nbSommets = xSmt.size();
		nbMailles = mailles.size();
		isValid=true;
	}
}

Off::~Off()
{
	for(int i=0; i< mailles.size();i++)
		delete[] mailles.at(i);
}

/*! @brief Lecture d'un fichier OFF
 ***********************************************************************/
bool Off::charger(const string& nomFichier)
{
	/* Ouverture du fichier */
	ifstream fich(nomFichier.c_str());
	if (fich == NULL)
	{
		isValid = false;
		return isValid;
	}
	
	/* Verification du format */
	string tampon, bidon;
	fich >> tampon >> nbSommets >> nbMailles >> bidon;
	if ((tampon.compare("OFF") != 0) && (tampon.compare("off") != 0))
	{
		fich.close();
		isValid = false;
		return isValid;
	}
	
	/* Lecture du fichier : liste des sommets */
	xSmt.resize(nbSommets);
	ySmt.resize(nbSommets);
	zSmt.resize(nbSommets);
		
	for (int i=0; i < nbSommets; ++i)
		fich >> xSmt.at(i) >> ySmt.at(i) >> zSmt.at(i);
	
	/* Lecture du fichier : liste des mailles */
	mailles.resize(nbMailles);
	
	for (int i=0; i < nbMailles; ++i)
	{
		/* nb de sommets de la mailles */
		int nbSommetsMaille;
		fich >> nbSommetsMaille;
		mailles.at(i) = new int[nbSommetsMaille+1];
	
		mailles.at(i)[0] = nbSommetsMaille;
		for (int j=0; j < nbSommetsMaille; ++j)
			fich >> mailles.at(i)[j+1];
	}
	
	/* Fin */
	fich.close();
	
	isValid = true;
	return isValid;
}

/*! @brief Ecriture au format OFF
 *
 * Ecriture du modèle dans un fichier au format <b>OFF</b>
 * (Object File Format).<br/>
 * <i>Cf.</i> <a href="http://shape.cs.princeton.edu/benchmark/documentation/off_format.html" target="_f3D">Princeton Shape Benchmark</a>
 * pour la description du format.
 * @param nomOff  Nom du fichier du modèle.
 ***********************************************************************/
bool  Off::ecrire(const string& nomFichier)
{
	/* Initialisations */
	if (!isValid) return false;

	/* Ouverture du fichier */
	ofstream  fich(nomFichier.c_str());
	if (fich == NULL) return false;

	/* En-tête */
	fich << "OFF" << endl << nbSommets << "  " << nbMailles << "  " << 0 << endl;

	/* Liste des sommets */
	for (int i=0; i < nbSommets; ++i)
		fich << xSmt.at(i) << "  " << ySmt.at(i) << "  " << zSmt.at(i) << endl;

	/* Liste des mailles */
	for (int i=0; i < nbMailles; ++i) {
		for (int j=0; j < mailles.at(i)[0]+1; ++j)
			fich << mailles.at(i)[j] << "  ";
		fich << endl;
	}

	/* Fin */
	fich.close();
	return true;
}


bool  Off::check(const string& nomFichier)
{
	/* Ouverture du fichier */
	ifstream fich(nomFichier.c_str());
	fich.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
	if (fich == NULL) return false;
	
	try {
		/* Verification du format */
		string tampon, bidon;
		int nbSommets,nbMailles;
		fich >> tampon >> nbSommets >> nbMailles >> bidon;
		
		if ((tampon.compare("OFF") != 0) && (tampon.compare("off") != 0))
		{
			fich.close();
			return false;
		}
		
		for (int i=0; i < nbSommets; ++i)
		{
			float tmp1,tmp2,tmp3;
			fich >> tmp1 >> tmp2 >> tmp3;
		}
		
		for (int i=0; i < nbMailles; ++i)
		{
			int nbSommetsMaille,tmp1;
			fich >> nbSommetsMaille;
			if(nbSommetsMaille != 3)
			{
				std::cout << "Les maillages non triangulaire ne sont pas pris en charge" << std::endl;
				return false;
			}
			for (int j=0; j < nbSommetsMaille; ++j)
				fich >> tmp1;
		}
	}
	catch (ifstream::failure e) {
		std::cout << "Exception opening/reading file";
		return false;
	}
	return true;
}
