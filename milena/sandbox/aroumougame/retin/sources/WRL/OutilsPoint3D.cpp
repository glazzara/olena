#include "WRL/OutilsPoint3D.h"
#define PI 3.1415926535897932

//---------------------------------------------------------------------------
//		CONSTRUCTOR DESTRUCTOR
//---------------------------------------------------------------------------

OutilsPoint3D::OutilsPoint3D(point3D * pt)
{
	ptPoint3D = pt;
}

//---------------------------------------------------------------------------

OutilsPoint3D::~OutilsPoint3D(void)
{
}

//---------------------------------------------------------------------------
//		Passage en coordonnée Sphérique
//---------------------------------------------------------------------------

float OutilsPoint3D::AngleYoZ(void)
{
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);
	
	return (atan(Uz/Uy) + PI*Heaviside(-Uy)*sign(Uz));
}

//---------------------------------------------------------------------------

float OutilsPoint3D::AngleZoX(void)
{
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	return (atan(Ux/Uz) + PI*Heaviside(-Uz)*sign(Ux));
}

//---------------------------------------------------------------------------

float OutilsPoint3D::AngleXoY(void)
{
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	return (atan(Uy/Ux) + PI*Heaviside(-Ux)*sign(Uy));
}

//---------------------------------------------------------------------------
//		Passage en coordonnée Sphérique
//---------------------------------------------------------------------------

float OutilsPoint3D::AngleAzimut(void)
{	// E [0 Pi] angle zOu
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	float angle = atan(Uy/Ux) + PI*Heaviside(-Ux)*sign(Uy) ;

	if (isnan(angle))
		angle = 0.0;

	return (angle);

}

//---------------------------------------------------------------------------

float OutilsPoint3D::AngleAzimut(const point3D *centre)
{	// E [0 Pi] angle zCu
	float Ux, Uy, Uz, Cx, Cy, Cz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	centre->getValue(Cx, Cy, Cz);
	Ux-=Cx;
	Uy-=Cy;

	float angle = atan(Uy/Ux) + PI*Heaviside(-Ux)*sign(Uy);
	if (isnan(angle))
		angle = 0.0;	

	return (angle);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

float OutilsPoint3D::AngleInclinaison(void)
{	// E [-Pi Pi] angle xOu' avec u' projete de u sur xOy
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	return ( acos( Uz/(LongueurCorde()) ) );
}

//---------------------------------------------------------------------------

float OutilsPoint3D::AngleInclinaison(const point3D *centre)
{	// E [-Pi Pi] angle xCu' avec u' projete de u sur xCy
	float Ux, Uy, Uz, Cx, Cy, Cz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	centre->getValue(Cx, Cy, Cz);
	Uz-=Cz;

	return ( acos( Uz/(LongueurCorde(centre)) ) );
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

float OutilsPoint3D::LongueurCorde(void)
{
	return ptPoint3D->length();
}

//---------------------------------------------------------------------------

float OutilsPoint3D::LongueurCorde(const point3D *centre)
{
	float Ux, Uy, Uz, Cx, Cy, Cz;

	ptPoint3D->getValue(Ux, Uy, Uz);
	centre->getValue(Cx, Cy, Cz);
	Ux-=Cx;		Uy-=Cy;		Uz-=Cz;;

	return sqrt( pow(Ux,2) + pow(Uy,2) + pow(Uz,2) );
}

//---------------------------------------------------------------------------
//			AUTRE
//---------------------------------------------------------------------------

float OutilsPoint3D::Angle(const point3D *axe)
// renvoie l'angle en radians entre point, 0 0 0 et axe
{
	float Px, Py, Pz, Ax, Ay, Az;
	float dP, dA;
	ptPoint3D->getValue(Px, Py, Pz);
	axe->getValue(Ax, Ay, Az);

	
	dP=ptPoint3D->length();
	dA=axe->length();

	float angle;
	angle = acos((Px*Ax+Py*Ay+Pz*Az)/(dP*dA));
	//cout << angle << "\t" ;
	if (isnan(angle))
		angle = 0.0;
	return angle;
}

//---------------------------------------------------------------------------
//		affichage
//---------------------------------------------------------------------------


void OutilsPoint3D::affiche(void)
{
	float Ux, Uy, Uz;

	ptPoint3D->getValue(Ux, Uy, Uz);
	cout << "coordonnees :" << Ux << "\t" << Uy << "\t" << Uz << endl;
}

//---------------------------------------------------------------------------
//		Rotation
//---------------------------------------------------------------------------


void OutilsPoint3D::rotationZ(float angle)
{
	float Ux, Uy, Uz;
	float NewUx, NewUy, NewUz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	NewUx = Ux*cos(angle) - Uy*sin(angle);
	NewUy = Ux*sin(angle) + Uy*cos(angle);
	NewUz = Uz;

	ptPoint3D->setValue(NewUx, NewUy, NewUz);
}

void OutilsPoint3D::rotationX(float angle)
{
	float Ux, Uy, Uz;
	float NewUx, NewUy, NewUz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	NewUy = Uy*cos(angle) - Uz*sin(angle);
	NewUz = Uy*sin(angle) + Uz*cos(angle);
	NewUx = Ux;

	ptPoint3D->setValue(NewUx, NewUy, NewUz);
}

void OutilsPoint3D::rotationY(float angle)
{
	float Ux, Uy, Uz;
	float NewUx, NewUy, NewUz;

	ptPoint3D->getValue(Ux, Uy, Uz);

	NewUz = Uz*cos(angle) - Ux*sin(angle);
	NewUx = Uz*sin(angle) + Ux*cos(angle);
	NewUy = Uy;

	ptPoint3D->setValue(NewUx, NewUy, NewUz);
}

//---------------------------------------------------------------------------
//		OUTILS PROTECTED
//---------------------------------------------------------------------------

int OutilsPoint3D::Heaviside(float X)
{
	if ( X <= 0)
		return 0;
	else
		return 1;
}

//---------------------------------------------------------------------------

int OutilsPoint3D::sign(float X)
{
	if ( X < 0)
		return -1;
	else
		return 1;
}




