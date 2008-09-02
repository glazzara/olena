#include "WRL/calcul.h"

//****************************************************************************
//		CALCUL SUR POINT
//****************************************************************************

point3D* produitVectoriel(const point3D *u, const point3D *v)
{
	SbVec3f *normal;
	normal = new SbVec3f();

	float Ux, Uy, Uz, Vx, Vy, Vz;
	float Nx, Ny, Nz;

	u->getValue(Ux, Uy, Uz);
	v->getValue(Vx, Vy, Vz);
	
	Nx= Uy*Vz - Uz*Vy;
	Ny= Uz*Vx - Ux*Vz;
	Nz= Ux*Vy - Uy*Vx;
	
	if (fabs(Nx) < EPS)
		Nx = 0;
	if (fabs(Ny) < EPS)
		Ny = 0;
	if (fabs(Nz) < EPS)
		Nz = 0;

	normal->setValue(Nx,Ny,Nz);
		
	return normal;
}

//****************************************************************************

point3D* produitVectoriel(const point3D *u, const point3D *v, const point3D *ptCommun)
{
	float Ux, Uy, Uz, Vx, Vy, Vz, Cx, Cy, Cz;

	u->getValue(Ux, Uy, Uz);
	v->getValue(Vx, Vy, Vz);
	ptCommun->getValue(Cx, Cy, Cz);

	point3D A;
	point3D B;
	A.setValue(Ux-Cx, Uy-Cy, Uz-Cz);
	B.setValue(Vx-Cx, Vy-Cy, Vz-Cz);

	return produitVectoriel(&A, &B);

}

//****************************************************************************
//****************************************************************************
float norme(const point3D *u) 
{
	float Ux, Uy, Uz;
	u->getValue(Ux, Uy, Uz);
	return sqrt(Ux*Ux+Uy*Uy+Uz*Uz);

}

//****************************************************************************

void normer(point3D *u)
{
	float Ux, Uy, Uz;
	u->getValue(Ux, Uy, Uz);
	float n = norme(u);
	
	Ux	=	Ux/n;
	Uy	=	Uy/n;
	Uz	=	Uz/n;

	u->setValue(Ux, Uy, Uz);

}

//****************************************************************************

float angle(const point3D *u, const point3D *v)
{
	float Ux, Uy, Uz, Vx, Vy, Vz;
	float lU, lV;
	
	u->getValue(Ux, Uy, Uz);
	v->getValue(Vx, Vy, Vz);
	
	lU=u->length();
	lV=v->length();

	float angle;
	angle = acos((Ux*Vx+Uy*Vy+Uz*Vz)/(lU*lV));

	return angle;
}



//****************************************************************************
//****************************************************************************

float distance(const point3D *u, const point3D *v)
{
	float Ux, Uy, Uz, Vx, Vy, Vz;

	u->getValue(Ux, Uy, Uz);
	v->getValue(Vx, Vy, Vz);

	return sqrt( pow((Ux-Vx),2) + pow((Uy-Vy),2) + pow((Uz-Vz),2) ) ;
}


//****************************************************************************
//		CALCUL SUR LISTE de POINTS
//****************************************************************************

Lpoints3D* produitVectoriel(const Lpoints3D *Lu, const Lpoints3D *Lv)
{
	if ( Lu->getNum() != Lv->getNum() )
	{
		cerr << "ERREUR : (calcul:produitVectoriel) arguments entrées de tailles differents" << endl;
		return NULL;
	}

	Lpoints3D *Lnormal;
	Lnormal = new Lpoints3D();

	point3D  *normal;

	for (int i=0 ; i<Lu->getNum(); ++i)
	{
		
		normal = produitVectoriel( &((*Lu)[i]) , &((*Lv)[i]) );
		Lnormal->set1Value(i, (*normal)[0], (*normal)[1], (*normal)[2]);
		
	}
	if (normal != NULL)
		delete normal;

	return Lnormal;
}

//****************************************************************************

Lpoints3D* produitVectoriel(const Lpoints3D *Lu, const Lpoints3D *Lv, const Lpoints3D *LptCommun)
{

	if ( ( Lu->getNum() != Lv->getNum() ) && ( Lu->getNum() != LptCommun->getNum() ) )
	{
		cerr << "ERREUR : (calcul:produitVectoriel) arguments entrées de tailles differents" << endl;
		return NULL;
	}

	Lpoints3D *Lnormal;
	Lnormal = new Lpoints3D();

	point3D  *normal;

	for (int i=0 ; i<Lu->getNum(); ++i)
	{
		
		normal = produitVectoriel( &((*Lu)[i]) , &((*Lv)[i]) , &((*LptCommun)[i]) );
		Lnormal->set1Value(i, (*normal)[0], (*normal)[1], (*normal)[2]);
		if (normal != NULL)
			delete normal;
	}

	return Lnormal;
}


//****************************************************************************

void normer(Lpoints3D **Lu)
{

	point3D *anormer = new point3D;
	Lpoints3D *newLu = new Lpoints3D;

	for (int i=0 ; i<(*Lu)->getNum(); ++i)
	{
		anormer->setValue( (**Lu)[i][0], (**Lu)[i][1], (**Lu)[i][2] );
		normer(anormer);
		newLu->set1Value(i, (*anormer)[0], (*anormer)[1], (*anormer)[2] );
	}

	delete anormer;
	delete *Lu;
	
	*Lu = newLu;
}


