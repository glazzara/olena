#include "histogramme/hist.h"



Histogramme::Histogramme(int monNbBin)
{
	nbBin = monNbBin;
	hist = new float[nbBin];

	// initialisation de l histogramme a zero
	for(int i=0; i< nbBin; i++)
		hist[i]=0;
}

Histogramme::Histogramme(int monNbBin, int nbValeurs, int *valeursNormees)
{
	nbBin = monNbBin;
	hist = new float[nbBin];

	// initialisation de l histogramme a zero
	for(int i=0; i< nbBin; i++)
		hist[i]=0;

	// affectation
	for(int i=0; i < nbValeurs; i++)
	{
		if ( valeursNormees[i] >= nbBin || valeursNormees[i] < 0 )
		{
			cerr << "Erreur constructeur Histogramme : paramètre d'entrée incorrect" << endl;
			exit(1);
		}
		else
			hist[valeursNormees[i]]++;
	}

	//normalisation
	for(int i=0; i < nbBin; i++)
		hist[i]=hist[i]/nbValeurs;	
}

Histogramme::Histogramme(int monNbBin, int nbValeurs, float *valeurs)
{
	nbBin = monNbBin;
	hist = new float[nbBin];

	// initialisation de l histogramme a zero
	for(int i=0; i< nbBin; i++)
		hist[i]=0;

	// recherche de la valeure maximale pour normalisation des données
	int indiceMax;
	float valeurMax;
	maxTableau(valeurs, nbValeurs, &indiceMax, &valeurMax);

	int *valeursNormees = new int[nbValeurs];
	NormalisationQuantification(valeurs, nbValeurs, valeurMax, valeursNormees);

	// affectation
	for(int i=0; i < nbValeurs; i++)
	{
		if ( valeursNormees[i] >= nbBin || valeursNormees[i] < 0 )
		{
			cerr << "Erreur lors de la normalisation des valeurs pour calcul histogramme" << endl;
			exit(1);
		}
		else
			hist[valeursNormees[i]]++;
	}
	//normalisation
	for(int i=0; i < nbBin; i++)
		hist[i]=hist[i]/nbValeurs;

	delete [] valeursNormees;
}

Histogramme::Histogramme(vector<int>  valeurs)
{
	nbBin = valeurs.size();
	hist = new float[nbBin];
	
	int somme = 0;
	
	for (int i=0 ; i<nbBin; i++)
	{
		somme += valeurs[i];
		hist[i] = valeurs[i];
	}

	// normalisation
	for (int i=0 ; i<nbBin; i++)
	{
		hist[i] = hist[i]/somme;
	}
}

Histogramme::Histogramme(vector<int>  valeurs, int histSize, bool Aire, vector <float> Aires)
{
	nbBin = histSize;
	hist = new float[nbBin];
	for(int i=0; i< nbBin; i++)
		hist[i]=0.;
	
	for (int i=0 ; i<valeurs.size(); i++)
	{
		if(Aire) hist[valeurs[i]] += Aires[i];
		else hist[valeurs[i]] ++;
	}
}

Histogramme::Histogramme(vector<float>  valeurs)
{
	nbBin = valeurs.size();
	hist = new float[nbBin];
	
	float somme = 0;
	
	for (int i=0 ; i<nbBin; i++)
	{
		somme += valeurs[i];
		hist[i] = valeurs[i];
	}

	// normalisation
	for (int i=0 ; i<nbBin; i++)
	{
		hist[i] = hist[i]/somme;
	}
}
Histogramme::Histogramme(vector < vector <float> > valeurs,vector <float> max, int monNbBin, bool Aire,vector <float> Aires )
{
	int size = valeurs.size();
	vector < vector <int> > discret;
	discret.resize(size);
	
	for(int i=0; i<size; i++)
	{
		discret[i].resize(valeurs[i].size());
		for(int j =0;j < valeurs[i].size(); j++)
		{
			discret[i][j] = (int) truncf (valeurs[i][j] * (float) monNbBin / (max[i] + 2*EPS) );
			if(discret[i][j]>=monNbBin) discret[i][j] = monNbBin-1;
		}
	}
	
	nbBin = (int)pow(monNbBin,size);
	hist = new float[nbBin];
	for(int i=0; i< nbBin; i++)
		hist[i]=0.;
	
	for(int j =0;j < valeurs[0].size(); j++)
	{
		int bin = 0;
		for(int i=0; i<size; i++)
		{
			bin += discret[i][j]*(int)pow(monNbBin,i);
		}
		if(Aire) hist[bin] += Aires[j];
		else hist[bin] ++;
	}
}


Histogramme::Histogramme(int monNbBin, unsigned long *valeurs)
{
	nbBin = monNbBin;
	hist = new float[nbBin];
	
	float somme = 0;
	for (int i=0 ; i<nbBin; i++)
	{
		somme += valeurs[i];
		hist[i] = valeurs[i];
	}

	// normalisation
	for (int i=0 ; i<nbBin; i++)
	{
		hist[i] = hist[i]/somme;
	}
}

Histogramme::Histogramme(vector<float>  valeurs, bool normalisation)
{
	nbBin = valeurs.size();
	hist = new float[nbBin];
	
	float somme = 0;
	
	for (int i=0 ; i<nbBin; i++)
	{
		somme += valeurs[i];
		hist[i] = valeurs[i];
	}

	// normalisation
	if (normalisation)
	{
		for (int i=0 ; i<nbBin; i++)
		{
			hist[i] = hist[i]/somme;
		}
	}
}

// Histogramme::Histogramme(vector<float>  valeurs, bool normalisationSum, bool normalisationVar)
// {
// 	nbBin = valeurs.size();
// 	hist = new float[nbBin];
// 	
// 	float somme = 0;
// 	
// 	for (int i=0 ; i<nbBin; i++)
// 	{
// 		somme += valeurs[i];
// 		hist[i] = valeurs[i];
// 	}
// 
// 	// normalisation somme
// 	if (normalisationSum)
// 	{
// 		for (int i=0 ; i<nbBin; i++)
// 		{
// 			hist[i] = hist[i]/somme;
// 		}
// 	}
// 
// 	// normalisation variance
// 
// 	if (normalisationVar)
// 	{
// 		float moy = somme/nbBin;
// 		
// 		float var=0;
// 		for (int i=0 ; i<nbBin; i++)
// 			var += pow( ( hist[i] - moy ), 2 );
// 
// 		float etype = sqrt(var);
// 
// 		for (int i=0 ; i<nbBin; i++)
// 		{
// 			hist[i] = hist[i]/etype;
// 		}
// 	}
// }

Histogramme::Histogramme(int monNbBin, int nbValeurs, float *valeurs, float valeurBinMax)
{
	nbBin = monNbBin;
	hist = new float[nbBin];

	// initialisation de l histogramme a zero
	for(int i=0; i< nbBin; i++)
		hist[i]=0;

	// affectation
	for(int i=0; i < nbValeurs; i++)
	{
	int valeurNormee = (int) nearbyint((valeurs[i]*(nbBin-1))/valeurBinMax);
		if ( valeurNormee >= nbBin || valeurNormee < 0 )
		{
			cerr << "Erreur lors de la normalisation des valeurs pour calcul histogramme" << endl;
			exit(1);
		}
		else
			hist[valeurNormee]++;
	}
	//normalisation
	for(int i=0; i < nbBin; i++)
		hist[i]=hist[i]/nbValeurs;
}


Histogramme::~Histogramme(void)
{
	delete [] hist;
}


void Histogramme::affiche(void)
{
	cout << endl;
	cout << "################################################################" << endl;
	cout << "#			HISTOGRAMME 				#" << endl;
	cout << "################################################################" << endl;
	cout << endl;
	
	for (int i=0 ; i < nbBin ; i++)
		cout << " indice : [" << i+1 << "] \t" <<  hist[i] << endl;
}

float Histogramme::testSommeHistogramme(void)
{	// fonction de test
	// calcul le nombre d elements de l histogramme et la compare au nombre de cordes
	float total = 0;
	for (int i=0 ; i < nbBin ; i++)
		total += hist[i];
	
	return total;
}



void Histogramme::maxTableau(float *tab, int nbElement, int *indiceMax, float *valeurMax)
{
	float tempsValeur=tab[0];
	int tempsIndice=0;
	for(int i=0;i<nbElement;i++)
	{
		if(tempsValeur<tab[i])
		{
			tempsValeur=tab[i];
			tempsIndice=i;
		}
	}	
	*indiceMax=tempsIndice;
	*valeurMax=tempsValeur;
}

void Histogramme::maxTableau(int *tab, int nbElement, int *indiceMax, int *valeurMax)
{
	int tempsValeur=tab[0];
	int tempsIndice=0;
	for(int i=0;i<nbElement;i++)
	{
		if(tempsValeur<tab[i])
		{
			tempsValeur=tab[i];
			tempsIndice=i;
		}
	}	
	*indiceMax=tempsIndice;
	*valeurMax=tempsValeur;
}

void Histogramme::minTableau(int *tab, int nbElement, int *indiceMin, int *valeurMin)
{
	int tempsValeur=tab[0];
	int tempsIndice=0;
	for(int i=0;i<nbElement;i++)
	{
		if(tempsValeur>tab[i])
		{
			tempsValeur=tab[i];
			tempsIndice=i;
		}
	}	
	*indiceMin=tempsIndice;
	*valeurMin=tempsValeur;
}

void Histogramme::NormalisationQuantification(float *tab, int nbElement, float valeurMax, int *nvTab)
{	
	for(int i=0; i<nbElement; i++)
		nvTab[i]= (int) nearbyint(tab[i]*(nbBin-1)/valeurMax);
}

void Histogramme::sauve(const char *nom)
{
	ofstream fout(nom);
	for (int i=0 ; i < nbBin ; i++)
		fout << i << "\t" << hist[i] << endl;
	fout.close();
}

void Histogramme::sauve(ofstream *fout , const char *balise)
{
	(*fout) << endl << balise << endl;
	for (int i=0 ; i < nbBin ; i++)
		(*fout) << i << "\t" << hist[i] << "\t";
	(*fout) << endl;
}

void Histogramme::plot(const char *nom)
{
	FILE *ml;
	if((ml=popen("gnuplot", "w")) == NULL)
	{
		perror("Mais ou est GNUplot");
		exit(-1);
	}

 	// initialization //
 	fflush(ml);
 	fprintf(ml, "plot '%s' using 1:2 with lines \n",nom);
 	fflush(ml);
}

void Histogramme::Normaliser()
{
	float somme = 0;
	
	for (int i=0 ; i<nbBin; i++)
		somme += hist[i];

	// normalisation
	for (int i=0 ; i<nbBin; i++)
		hist[i] = hist[i]/somme;
}

