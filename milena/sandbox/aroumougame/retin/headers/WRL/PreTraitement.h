#ifndef __PreTraitement_h__
#define __PreTraitement_h__

#include <iostream>

#include "WRL/Coin.h"
#include "WRL/calcul.h"
#include "WRL/OutilsListePoints.h"

// Calcul ACP
#include "matrice/MatriceF.h"

using namespace std;

class PreTraitement
{
	public :

	PreTraitement(Lpoints3D *ptPtsVrml);
	~PreTraitement(void);

// CENTRAGE

	void afficheCentre(void) const;
	bool estCentre(void);
	bool centrer(void);

// ACP

	// calcul les directions principales
	// si objet non centré => centrage
	// calcul valeur propre avec MatriceF.h + librairie GSL
	void ACP(void);
	void afficheAxes(void) const;
	point3D obtAxe1(void) { return axe1; }
	point3D obtAxe2(void) { return axe2; }

// alignement sur ACP
	void aligner(void);

	protected :
	
	void calculCentre(void);

	private :

	Lpoints3D *ptPts;
	point3D centreObjet;
	point3D axe1;
	point3D axe2;
};

#endif
