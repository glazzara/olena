#include "WRL/MaillageTriangulaire.h"

//---------------------------------------------------------------------------
//		CONSTRUCTOR DESTRUCTOR
//---------------------------------------------------------------------------

maillageTriangulaire::maillageTriangulaire(Lmailles *indexMaille, Lpoints3D *pts)
{
	PbMaillage = init(indexMaille);
	ptPts = pts;
}

//---------------------------------------------------------------------------

maillageTriangulaire::~maillageTriangulaire(void)
{
	viderIndexMaille();
}

//---------------------------------------------------------------------------

void maillageTriangulaire::viderIndexMaille(void)
{

	for(unsigned int i=0 ; i < Tindice.size() ; i++)
		for(unsigned int j=0 ; j < Tindice[i].size() ; j++)
			if (! Tindice[i].empty())
				Tindice[i].clear();
	if (! Tindice.empty())
		Tindice.clear();
	

	nbMailles = 0;
}

//---------------------------------------------------------------------------

bool maillageTriangulaire::init(Lmailles *indexMaille)
{	// Lmailles = SoMFInt32
	// indexMaille est un tableau d'entier faisant reference aux points Lpoint
	// par sa position dans la liste
	// une maille est defini par la succession des indices des points
	// chaque maille est séparée par la valeur -1

	//donc si le maillage est bien triangulaire
	// on a nbMailles = taille de LMailles / (3 points + 1 separateur(-1) )
	nbMailles = (int) ( indexMaille->getNum() )/4;

	// pointe sur le première index
	const int* val = indexMaille->getValues(0);

	int nbValeurs = indexMaille->getNum();

	// allocation nb de lignes
	Tindice.resize(nbMailles);

	// allocation pour la premiere maille
	Tindice[0].resize(3);

	int iface = 0;
	for (int idx=0; idx < nbValeurs; ++idx) 
	{
		if ( (val[idx] == -1) & (idx%4 != 3) )
		{
			goto NONTRIANGULAIRE;
		}
		else if (val[idx] == -1)
		{
			++iface;
			// allocation pour la maille suivante
			if (idx < nbValeurs - 1)
			{
				Tindice[iface].resize(3);
			}
		}
		else
		{
			if (idx%4 < 3)
				Tindice[iface][idx%4] = val[idx];
			else
				goto NONTRIANGULAIRE;
		}
	}
	//delete val;
	return false;	


NONTRIANGULAIRE:
	cout << "PB: (maillageTriangulaire::init) Maillage non triangulaire - initialisation MaillageTriangulaire IMPOSSIBLE" << endl;

	// on vide "Tindice"
	viderIndexMaille();
	
	return true;
}

//---------------------------------------------------------------------------
//			affichage
//---------------------------------------------------------------------------

void maillageTriangulaire::afficheIndicesMaille(int i)
{
	if ( i >= nbMailles )
		cout << " Pas de Maille[ " << i << "]" << endl;
	else
	{
		cout << "Maille [" << i << "] : ";
		cout << Tindice[i][0] << "\t" ;
		cout << Tindice[i][1] << "\t" ;
		cout << Tindice[i][2] << endl;
	}
}

//---------------------------------------------------------------------------

void maillageTriangulaire::afficheIndicesMaillage(void)
{
	cout << "AFFICHAGE index MAILLES" << endl;

	for (int i=0; i<nbMailles; i++)
	{
		afficheIndicesMaille(i);
	}
}

void maillageTriangulaire::affichePoints(int i)
{
	if ( i >= nbMailles )
		cout << " Pas de Maille[ " << i << "]" << endl;
	else
	{
		point3D A ;
		point3D B ;
		point3D C ;

		obtMaillePoints(i, &A, &B, &C);

		cout << "AFFICHAGE coordonnées des points de la MAILLES [" << i << "] :" << endl;
		cout << "points A : " << A[0] << "\t" <<  A[1] << "\t" <<  A[2] << endl;
		cout << "points B : " << B[0] << "\t" <<  B[1] << "\t" <<  B[2] << endl;
		cout << "points C : " << C[0] << "\t" <<  C[1] << "\t" <<  C[2] << endl;
	}

}


//---------------------------------------------------------------------------
//			INFO
//---------------------------------------------------------------------------

int * maillageTriangulaire::obtMailleIndices(int i)
{
	if ( i >= nbMailles )
	{
		cerr << " Pas de Maille[ " << i << "]" << endl;
		return NULL;
	}

	int *maille = new int[3];
	maille[0] = Tindice[i][0] ;
	maille[1] = Tindice[i][1] ;
	maille[2] = Tindice[i][2] ;

	return maille;
}

//---------------------------------------------------------------------------

void maillageTriangulaire::obtMaillePoints(int i, point3D *A, point3D *B, point3D *C)
{
	if ( i < nbMailles )
	{
		int *maille;
		maille = obtMailleIndices(i);

		*A = (*ptPts)[maille[0]];
		*B = (*ptPts)[maille[1]];
		*C = (*ptPts)[maille[2]];

		delete [] maille;
	}
	else
		cerr << " PB : (maillageTriangulaire::obtMaillePoints) Pas de Maille[ " << i << "]" << endl;

}

//---------------------------------------------------------------------------

void maillageTriangulaire::obtMaillePoints(Lpoints3D *LA, Lpoints3D *LB, Lpoints3D *LC)
{
	for ( int i=0; i < nbMailles ; i++ )
	{
		int *maille;
		maille = obtMailleIndices(i);

		point3D A = (*ptPts)[maille[0]];
		point3D B = (*ptPts)[maille[1]];
		point3D C = (*ptPts)[maille[2]];

		LA->set1Value(i, A[0], A[1], A[2] );
		LB->set1Value(i, B[0], B[1], B[2] );
		LC->set1Value(i, C[0], C[1], C[2] );

		delete [] maille;
	}
}


//---------------------------------------------------------------------------
//			CALCUL
//---------------------------------------------------------------------------

Lpoints3D * maillageTriangulaire::calculCentres(void)
{
	Lpoints3D *centres = new Lpoints3D;
	
	for (int i=0 ; i < Tindice.size() ; i++)
	{
		mailleTriangulaire maille(ptPts, (Tindice[i]) );
		point3D *centre =  maille.centre();
		centres->set1Value(i, (*centre)[0], (*centre)[1], (*centre)[2]);
		delete centre;
	}

	return centres;
}

//---------------------------------------------------------------------------

float * maillageTriangulaire::calculAires(void)
{
	float *aires = new float[nbMailles];
	
	for (int i=0 ; i < Tindice.size() ; i++)
	{
		mailleTriangulaire maille(ptPts, (Tindice[i]));
		aires[i] = maille.aire();
	}

	return aires;
}

//---------------------------------------------------------------------------

vector<float> maillageTriangulaire::calculAiresV(void)
{
	vector <float> aires;
	aires.resize(Tindice.size());

	for (int i=0 ; i < aires.size() ; i++)
	{
		mailleTriangulaire maille(ptPts, (Tindice[i]));
		aires[i] = maille.aire();
	}

	return aires;
}
//---------------------------------------------------------------------------

void maillageTriangulaire::calculMilieuxArretes(Lpoints3D *LmBC, Lpoints3D *LmAC, Lpoints3D *LmAB )
{
	point3D mBC;
	point3D mAC;
	point3D mAB;


	for (int i=0 ; i < Tindice.size() ; i++)
	{
		mailleTriangulaire maille(ptPts, (Tindice[i]) );
		maille.milieux (&mBC, &mAC, &mAB);

		LmBC->set1Value(i, mBC[0], mBC[1], mBC[2]);
		LmAC->set1Value(i, mAC[0], mAC[1], mAC[2]);
		LmAB->set1Value(i, mAB[0], mAB[1], mAB[2]);
	}
}





