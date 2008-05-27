#ifndef __maillageTriangulaire_h__
#define __maillageTriangulaire_h__

#include <iostream>
#include <vector>

#include "WRL/Coin.h"
#include "WRL/MailleTriangulaire.h"

using namespace std;

class maillageTriangulaire
{
	public :
	maillageTriangulaire(Lmailles *indexMaille, Lpoints3D *pts);
	~maillageTriangulaire(void);

	void afficheIndicesMaille(int i);
	void afficheIndicesMaillage(void);
	void affichePoints(int i);
	
	int * obtMailleIndices(int i);
	void obtMaillePoints(int i, point3D *A, point3D *B, point3D *C);
	void obtMaillePoints(Lpoints3D *LA, Lpoints3D *LB, Lpoints3D *LC);

	int nb(void) {return nbMailles; }

	Lpoints3D * calculCentres(void);
	float * calculAires(void);
	vector <float> calculAiresV(void);

	void calculMilieuxArretes(Lpoints3D *LmBC, Lpoints3D *LmAC, Lpoints3D *LmAB );

	bool maillesTriangulaires(void) {return !(PbMaillage) ;}

	protected :

	void viderIndexMaille(void);

	// memorise les indices des mailles dans Tindice
	// retourne true en cas de PB
	// est appelé par le constructeur
	bool init(Lmailles *indexMaille);

	private :

	vector < vector<uint> > Tindice;
	int nbMailles;
	bool PbMaillage;

	Lpoints3D *ptPts;

	

};

#endif

