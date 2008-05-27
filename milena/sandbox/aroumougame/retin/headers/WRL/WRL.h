// DOCUMENTATION COIN3D
// http://www-evasion.imag.fr/Membres/Francois.Faure/doc/inventorMentor/sgi_html/
// file:///usr/lsa/docs/Coin-2.4.4/index.html
//http://doc.coin3d.org/Coin/index.html


#ifndef __WRL_h__
#define __WRL_h__

#include <iostream>
#include <vector>
#include "WRL/Coin.h"

using namespace std;

bool initWRL();
bool finishWRL();

class WRL
{
	public :

		enum option {rien, verbose, debug};

		WRL(void);
		WRL(option affiche);
		~WRL(void);

		objet3D * lire(const string nomFichier);				// retourne NULL si probleme
		bool ecrire(const string nomFichier, objet3D *obj);

		objet3D * creerObjet(Lpoints3D *mesPoints);				// retourne NULL si probleme
		objet3D * creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, bool effetBrillant);		// retourne NULL si probleme
		// on dispose d'une palette de 9 couleurs
		objet3D * creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, SoMFInt32 *couleursFaces);		// retourne NULL si probleme
		objet3D * creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, SoMFInt32 *couleursFaces, int nbCouleurs);

		void detruireObjet(objet3D *obj);

		bool Vrml1ToVrml2(const string nomVRML1, const string nomVRML2,bool materiau);

		// TcouleursMailles est un tableau int[nbMailles] dont les valeurs sont comprises entre 0 et NB couleurs
		SoMFInt32 * coloriserFaces(int *TcouleursMailles, int nbMailles);
		SoMFInt32 * coloriserFaces(vector<int> TcouleursMailles);

		bool checkVRML(string nomFichier);
		
// retourne les indices des sommets de chaque maille triangulaire
// si aucune maille trouvée renvoie NULL
	// LMailles est un tableau d'entier faisant reference aux points Lpoint
	// par sa position dans la liste
	// une maille est defini par la succession des indices des points
	// chaque maille est séparée par la valeur -1
		Lmailles* obtMailles(objet3D *obj);
// retrourne les coordonnees des points
// si aucun point trouvé renvoie NULL
		Lpoints3D* obtPoints(objet3D *obj);


	protected :

		objet3D* creerObjet(void);
		int calculNbMaille(const Lmailles *indexPts);
		SoVRMLColor * Couleurs(int nbCouleurs);
// retourne indice couleur de chaque maille
// en fonction de tab
//		SoMFInt32* coloriser(float *tab, int tailleTab);
//		SoMFInt32* coloriser(float *tab, int tailleTab, float valeurMax);

	private :
		option affichage;
};

#endif
