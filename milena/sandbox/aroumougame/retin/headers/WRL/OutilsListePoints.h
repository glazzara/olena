#ifndef __OutilsListePoints_h__
#define __OutilsListePoints_h__

#include <iostream>

#include "WRL/Coin.h"
#include "WRL/OutilsPoint3D.h"
#include <vector>

using namespace std;

class OutilsListePoints
{
	public :

	OutilsListePoints(Lpoints3D *ptPtsVrml);
	~OutilsListePoints(void);

// affiche les coordonnées de chaque point
	void affiche(void) const;
	void affiche(int i) const;

	void rotationX(float angle);
	void rotationY(float angle);
	void rotationZ(float angle);

	float * AngleInclinaison(void);
	float * AngleAzimut(void);

	vector <float> Angles(const point3D *axe);
	vector <float> Longueurs(void);

	vector <vector <float> > AnglesSpheriques(void);

	Lpoints3D * sphericToCart(float * AngleInclin, float * AngleAzim, float *rot);

	int nbPoints(void) { return ptPts->getNum() ; }

	const point3D* operator[] (int increment) {return ptPts->getValues(increment); }
	
	private :

	Lpoints3D *ptPts;
};

#endif

