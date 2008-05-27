#include "histogramme/listeHisto.h"


listeHisto::listeHisto(string monRepertoire, int monNbBin)
{
	repertoire = monRepertoire;
	nbBin = monNbBin;
	finListe = false;

	string nomFichier;

	lire = new ifstream;

	pRep = opendir(repertoire.c_str());
	pFichier = readdir (pRep) ;
	
	// on pointe sur le premier fichier
	suivant(true);
}

listeHisto::listeHisto(string monRepertoire)
{
	repertoire = monRepertoire;
	nbBin = 0;
	finListe = false;

	string nomFichier;

	lire = new ifstream;

	pRep = opendir(repertoire.c_str());
	pFichier = readdir (pRep) ;
	
	// on pointe sur le premier fichier
	suivant(true);
}

listeHisto::~listeHisto(void)
{
	delete lire;
	delete pFichier;
	closedir(pRep);
}


bool listeHisto::suivant(void)
{
	string nomFichier;


	fermerHisto();

	pFichier = readdir (pRep);
	if (pFichier != NULL)
	{
		nomFichier = pFichier->d_name;
		if ( (nomFichier.find(TYPE1, 0) != (string::npos) && nomFichier.find(nonTYPE1, 0) == (string::npos)) || (nomFichier.find(TYPE2, 0) != (string::npos) && nomFichier.find(nonTYPE2, 0) == (string::npos)) )
			return ouvrirHisto();
		else
			suivant();
	}
	else
	{
		finListe = true;
		return false;
	}
	return false;

}

bool listeHisto::suivant(bool affiche)
{
	string nomFichier;

	fermerHisto();

	pFichier = readdir (pRep);
	if (pFichier != NULL)
	{
		nomFichier = pFichier->d_name;
		if (affiche == true)
			cout << "nom fichier : " << nomFichier << endl;
		if ( (nomFichier.find(TYPE1, 0) != (string::npos) && nomFichier.find(nonTYPE1, 0) == (string::npos)) || (nomFichier.find(TYPE2, 0) != (string::npos) && nomFichier.find(nonTYPE2, 0) == (string::npos)) )
		{
			bool ok = ouvrirHisto();
			return ok;
		}
		else
		{
			cout << "PB fichier suivant n'a pas la bonne extension" << endl;
			suivant(true);
		}
	}
	else
	{
		cout << "pas de fichier suivant" << endl;
		finListe = true;
		return false;
	}
	return false;
}

bool listeHisto::ouvrirHisto(void)
{

	if (pFichier != NULL)
	{
		string nomFichier = pFichier->d_name;
		nomFichier = repertoire + "/" + nomFichier;
		cout << "nom fichier : " << nomFichier << endl;
		lire->open(nomFichier.c_str());
			if ( lire->good())
			{	
				return true;
			}
			else
			{
				cout << "Probleme ouverture du fichier : " << nomFichier << endl;
				return false;
			}
	}
	else
	{
		cout << "pointeur de fichier est NULL" << endl;
		return false;
	}
}

void listeHisto::fermerHisto(void)
{
	if (lire->is_open() && lire != NULL)
	{
		delete lire;
		lire = new ifstream;
	}
}


float * listeHisto::valeurs(string balise)
{
	string ligne;
	float valeur;
	int indice;

	lire->seekg(0,ios::beg);// se positionne au debut du fichier

	float *donnees = new float[nbBin];
	do
		getline(*lire, ligne);
	while (ligne != balise && !lire->eof() );
	if (ligne == balise)
		for (int i=0; i < nbBin; i++)
		{
			(*lire) >> indice;
			(*lire) >> valeur;
			if( isnan(valeur) != 0 || isinf(valeur) != 0)
			{
				cout << "PB NaN ou InF detecté" << endl;
				delete donnees;
				donnees = NULL;
				//fermerHisto();
				return donnees;
			}
			if (indice != i)
			{
				cout << "PB en lecture, fichier entre incorecte" << endl;
				delete donnees;
				donnees = NULL;
				//fermerHisto();
				return donnees;
			}
			donnees[indice] = valeur;
		}
	else
	{
		cout << "PB balise "<< balise << " non trouvé dans le fichier : "  << endl;
		donnees = NULL;
	}
	return donnees;
}

float * listeHisto::valeurs(string balise, int monNbBin)
{
	string ligne;
	float valeur;
	int indice;

	lire->seekg(0,ios::beg);// se positionne au debut du fichier

	float *donnees = new float[monNbBin];
	do
		getline(*lire, ligne);
	while (ligne != balise && !lire->eof() );
	if (ligne == balise)
		for (int i=0; i < monNbBin; i++)
		{
			(*lire) >> indice;
			//cout << indice << " : " ;
			(*lire) >> valeur;
			if( isnan(valeur) != 0 || isinf(valeur) != 0)
			{
				cout << "PB NaN ou InF detecté" << endl;
				delete donnees;
				donnees = NULL;
				//fermerHisto();
				return donnees;
			}
			if (indice != i)
			{
				cout << "PB en lecture, fichier entre incorrecte" << endl;
				delete donnees;
				donnees = NULL;
				//fermerHisto();
				return donnees;
			}
			donnees[indice] = valeur;
		}
	else
	{
		cout << "PB balise "<< balise << " non trouvé dans le fichier : "  << endl;
		donnees = NULL;
	}
	return donnees;
}

float * listeHisto::coeff(int nbHarmo, int nbCoeff)
{
	string ligne;
	float valeur;
	int numLigne = 0;

	lire->seekg(0,ios::beg);// se positionne au debut du fichier

	float *donnees = new float[nbCoeff*nbHarmo];
	do
	{
		getline(*lire, ligne);
		if (numLigne < nbHarmo)
		{
			for (int i=0; i < nbCoeff; i++)
			{
				(*lire) >> valeur;
				if( isnan(valeur) != 0 || isinf(valeur) != 0)
				{
					cout << "PB NaN ou InF detecté" << endl;
					delete donnees;
					donnees = NULL;
					return donnees;
				}
				donnees[nbCoeff*numLigne+i] = valeur;
			}
		}
		else
		{
			cerr << "Fichier nom Conforme" << endl;
			delete donnees;
			donnees = NULL;
			return donnees;
		}
		numLigne++;
	}
	while (!lire->eof() );

	return donnees;
}

string listeHisto::nom(void)
{
	string nomSansExt = pFichier->d_name;
	int pos = nomSansExt.find(".",0);
	nomSansExt = nomSansExt.erase(pos, nomSansExt.size()-pos);
	return nomSansExt;
}
