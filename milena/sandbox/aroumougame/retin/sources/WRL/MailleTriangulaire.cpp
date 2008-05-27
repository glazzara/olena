# include "WRL/MailleTriangulaire.h"

//---------------------------------------------------------------------------
//		CONSTRUCTOR DESTRUCTOR
//---------------------------------------------------------------------------

mailleTriangulaire::mailleTriangulaire(float A[3], float B[3], float C[3])
{
	ptA.setValue(A);
	ptB.setValue(B);
	ptC.setValue(C);
}

//---------------------------------------------------------------------------

mailleTriangulaire::mailleTriangulaire(point3D *A, point3D *B, point3D *C)
{
	ptA.setValue( (*A)[0], (*A)[1], (*A)[2]);
	ptB.setValue( (*B)[0], (*B)[1], (*B)[2]);
	ptC.setValue( (*C)[0], (*C)[1], (*C)[2]);
}

//---------------------------------------------------------------------------

mailleTriangulaire::mailleTriangulaire(Lpoints3D *pts, int* maille)
{
	const point3D *A = pts->getValues(maille[0]);
	const point3D *B = pts->getValues(maille[1]);
	const point3D *C = pts->getValues(maille[2]);

	ptA.setValue( (*A)[0], (*A)[1], (*A)[2]);
	ptB.setValue( (*B)[0], (*B)[1], (*B)[2]);
	ptC.setValue( (*C)[0], (*C)[1], (*C)[2]);
	
}

//---------------------------------------------------------------------------

mailleTriangulaire::mailleTriangulaire(Lpoints3D *pts, vector<uint> maille)
{

	const point3D *A = &((*pts)[(maille)[0]]);
	const point3D *B = &((*pts)[(maille)[1]]);
	const point3D *C = &((*pts)[(maille)[2]]);

	ptA.setValue( (*A)[0], (*A)[1], (*A)[2] );
	ptB.setValue( (*B)[0], (*B)[1], (*B)[2] );
	ptC.setValue( (*C)[0], (*C)[1], (*C)[2] );
	
}

//---------------------------------------------------------------------------

mailleTriangulaire::~mailleTriangulaire(void)
{

}

//---------------------------------------------------------------------------
//			Outils
//---------------------------------------------------------------------------

point3D* mailleTriangulaire::centre(void)
{
	point3D *Centre = new point3D;
	
	*Centre = (ptA+ptB+ptC)/3;

	return Centre;
}

//---------------------------------------------------------------------------

float mailleTriangulaire::aire(void)
{
	point3D *normal =  produitVectoriel( &ptA, &ptB, &ptC  );

	float S =  0.5 * norme( normal );
	delete normal;
	return S;

}

//---------------------------------------------------------------------------

void mailleTriangulaire::milieux (point3D *a, point3D *b, point3D *c)
{
	*c = ( ptA + ptB )/2;
	*b = ( ptA + ptC )/2;
	*a = ( ptB + ptC )/2;

}


