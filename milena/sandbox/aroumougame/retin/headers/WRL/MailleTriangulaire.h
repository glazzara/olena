#ifndef __mailleTriangulaire_h__
#define __mailleTriangulaire_h__

#include <iostream>
#include <vector>

#include "WRL/Coin.h"
#include "WRL/calcul.h"

using namespace std;

class mailleTriangulaire
{
	public :
	mailleTriangulaire(float A[3], float B[3], float C[3]);
	mailleTriangulaire(point3D *A, point3D *B, point3D *C);
	mailleTriangulaire(Lpoints3D *pts, vector<uint>  maille);
	mailleTriangulaire(Lpoints3D *pts, int *maille);
	~mailleTriangulaire(void);

	
	void milieux (point3D *a, point3D *b, point3D *c);
	point3D* centre(void);
	float aire(void);

	private :
	point3D ptA;
	point3D ptB;
	point3D ptC;

};

#endif


