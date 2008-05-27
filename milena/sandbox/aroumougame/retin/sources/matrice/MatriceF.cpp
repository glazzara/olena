#include "matrice/MatriceF.h"


MatriceF::MatriceF(int taille)
{
	nbLignes=taille;
	nbColonnes=taille;

	matrice.resize(taille);   			// allocation nb lignes
	for(int i=0 ; i < matrice.size() ; i++)		// pour chaque ligne
		matrice[i].resize(taille);		// creation des colonnes
}


/*         // idem sans la librairie STL
MatriceF::MatriceF(int taille)
{
	nbLignes = nbColonnes = taille;
	matrice = new float*[taille];
	for(int i=0;i<taille;i++)	
		matrice[i]= new float[taille+1];				
}	*/

MatriceF::~MatriceF ()
{	// on libère la mémoire
	for(int i=0 ; i < matrice.size() ; i++)
		for(int j=0 ; j < matrice[i].size() ; j++)
			if (matrice[i].empty())
				matrice[i].clear();
	if (! matrice.empty())
		matrice.clear();
}
/*	// idem sans la librairie STL
MatriceF::~MatriceF (int taille)
{
	if(matrice)
		for(int i=0; i < taille ; ++i)	
			delete [] matrice[i];	
	delete [] matrice;
}	*/

MatriceF::MatriceF(int tailleLignes, int tailleColonnes)
{
	nbLignes = tailleLignes;
	nbColonnes = tailleColonnes;

	matrice.resize(tailleLignes);   			// allocation nb lignes
	for(int i=0 ; i < matrice.size() ; i++)			// pour chaque ligne
		matrice[i].resize(tailleColonnes);		// creation des colonnes
}

MatriceF::MatriceF(int taille,  float * valeur)
{
	nbLignes=taille;
	nbColonnes=taille;

	matrice.resize(taille);   			// allocation nb lignes
	for(int i=0 ; i < matrice.size() ; i++)		// pour chaque ligne
		matrice[i].resize(taille);		// creation des colonnes

	for(int i=0 ; i < matrice.size() ; i++)
		for(int j=0 ; j < matrice[i].size() ; j++)
		{
			if (i == j)	
				matrice[i][j] = valeur[i];
			else if (i < j)
			{
				int offset = taille-1;
				for (int a = 1 ; a <= i; a++)
					offset += taille - a;
				matrice[i][j] = valeur[offset+j-i];
			}
			else
				matrice[i][j] = matrice[j][i];
		}
}

MatriceF::MatriceF(int tailleLignes, int tailleColonnes, float *valeur)
{
	nbLignes=tailleLignes;
	nbColonnes=tailleColonnes;

	matrice.resize(tailleLignes);   			// allocation nb lignes
	for(int i=0 ; i < matrice.size() ; i++)			// pour chaque ligne
		matrice[i].resize(tailleColonnes);		// creation des colonnes

	for(int i=0 ; i < matrice.size() ; i++)
		for(int j=0 ; j < matrice[i].size() ; j++)
			matrice[i][j]=valeur[i*nbColonnes+j];
	
}

void MatriceF::affiche()
{
	cout << "la matrice vaut :" << endl;
	for(int i=0 ; i < matrice.size() ; i++)
	{
		for(int j=0 ; j < matrice[i].size() ; j++)
			cout << matrice[i][j] << "\t";
	cout << endl;
	}
	cout << endl;
}

int MatriceF::obtNbLignes() { return nbLignes; }
int MatriceF::obtNbColonnes() {return nbColonnes; }

void MatriceF::obtValeurs(float *valeur)
{
	//float *valeur;
	//valeur = new float [nbLignes*nbColonnes];
	for (int i=0 ; i < nbLignes ; i++)
		for (int j=0; j < nbColonnes ; j++)
			valeur[i*nbColonnes+j]=matrice[i][j];
}

void MatriceF ::chgValeurs(float *valeur)
{
	for (int i=0 ; i < matrice.size() ; i++)
		for (int j=0; j < matrice[i].size() ; j++)
			matrice[i][j]=valeur[i*nbColonnes+j];
}

void MatriceF ::chgDiagonale(float *valeur)
{
	for (int i=0 ; i < matrice.size() && i < matrice[i].size() ; i++)
			matrice[i][i]=valeur[i];
}

void MatriceF ::obtDiagonale(float *diago)
{
	for (int i=0 ; i < matrice.size() && i < matrice[i].size() ; i++)
			diago[i]=matrice[i][i];
}

float MatriceF ::obtValeur(int position1, int position2)
{
	return matrice[position1][position2];
}

bool MatriceF :: isDiagonalePositive()
{
	bool test = true; 
	float *diago;
	diago = new float [min(nbLignes,nbColonnes)];
	
	obtDiagonale(diago);
	
	for (int i=0 ; i < min(nbLignes,nbColonnes); i++)
		if (diago[i] < 0)
		{
			test = false;
			break;
		}
		
	delete [] diago;
	return test;
}



void MatriceF::diagonalisation(MatriceF *diago, MatriceF *vecteurs )
{
	//MatriceF matriceDiagonale (nbLignes, nbColonnes);

	float *valeursPropres;
	valeursPropres = new float [nbLignes];
	float *vecteursPropres;
	vecteursPropres = new float [nbLignes*nbColonnes];

	if (nbLignes != nbColonnes)
		cout << "WARNING : diagonalisation non testée pour matrice non carrée " << endl;

	float *valeur;
	valeur = new float [nbLignes*nbColonnes];
	obtValeurs(valeur);
	valeursVecteursPropres(nbLignes, nbColonnes, valeur, valeursPropres, vecteursPropres);
	// test
	/*	 cout << "les valeurs propres :" << endl;
		cout << valeursPropres[0] << "\t" << valeursPropres[1] << "\t" << valeursPropres[2] << endl;
		cout <<" vecteurs 1 : " << endl;
		cout << vecteursPropres[0] << "\t" << vecteursPropres[1] << "\t" << vecteursPropres[2] << endl;
		cout <<" vecteurs 2 : " << endl;
		cout << vecteursPropres[3] << "\t" << vecteursPropres[4] << "\t" << vecteursPropres[5] << endl;
		cout <<" vecteurs 3 : " << endl;
		cout << vecteursPropres[6] << "\t" << vecteursPropres[7] << "\t" << vecteursPropres[8] << endl; */

	vecteurs->chgValeurs(vecteursPropres);
	diago->chgDiagonale(valeursPropres);

	//diago->affiche();
	//vecteurs->affiche();

	delete [] valeur;
	delete [] valeursPropres;
	delete [] vecteursPropres;
}

void MatriceF::obtDirectionsPrincipales(float *direction1, float *direction2)
{
	int j1=0, j2=0;
	MatriceF matDiago(min(nbLignes, nbColonnes));
	MatriceF vectPropres(nbLignes, nbColonnes);
	float *valeursPropres;
	valeursPropres = new float [min(nbLignes, nbColonnes)];
	
	diagonalisation(&matDiago, &vectPropres);
	//matDiago.affiche();
	//vectPropres.affiche();	
	matDiago.obtDiagonale(valeursPropres);
	
	{
	float max = fabs(valeursPropres[j1]);
	for (int i=0 ; i < min(nbLignes, nbColonnes); i++)
		if (max <= fabs(valeursPropres[i]))
		{
			max = fabs(valeursPropres[i]);
			j2=j1;
			j1=i;
		}
	}
	
	for (int i=0 ; i < nbLignes; i++)
	{
		direction1[i]=vectPropres.obtValeur(i,j1);
		direction2[i]=vectPropres.obtValeur(i,j2);
	}	
	
	delete [] valeursPropres;
}



