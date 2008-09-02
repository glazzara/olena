#ifndef __OutilsPoint3D_h__
#define __OutilsPoint3D_h__

#include <iostream>

#include "WRL/Coin.h"
//#include "math.h"

using namespace std;

class OutilsPoint3D
{
	public :
		OutilsPoint3D(point3D * pt);

		~OutilsPoint3D(void);

		// passage en coordonnée sphérique

		float AngleInclinaison(void);
		float AngleInclinaison(const point3D *centre);
		float AngleAzimut(void);
		float AngleAzimut(const point3D *centre);
		float LongueurCorde(void);
		float LongueurCorde(const point3D *centre);


		float Angle(const point3D *axe);  // axe passe par le 0 0 0

		float AngleXoY(void);
		float AngleYoZ(void);
		float AngleZoX(void);

		// affichage
		void affiche(void);

		// rotation
		void rotationX(float angle);
		void rotationY(float angle);
		void rotationZ(float angle);


	protected :
		int Heaviside(float X);
		int sign(float X);

	private :
		point3D * ptPoint3D;
};

#endif

