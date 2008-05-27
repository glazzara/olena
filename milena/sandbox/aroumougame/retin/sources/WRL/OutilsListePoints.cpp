#include "WRL/OutilsListePoints.h"

//****************************************************************************
//			Constructeur - destructeur
//****************************************************************************

OutilsListePoints::OutilsListePoints(Lpoints3D *ptPtsVrml)
{
	ptPts = ptPtsVrml;
}

//****************************************************************************

OutilsListePoints::~OutilsListePoints(void)
{
}

//****************************************************************************
//			Affichage
//****************************************************************************

void OutilsListePoints::affiche(void) const
{
	cout << "########################################################" << endl;
	cout << "#		COORDONNEES DES POINTS			#" << endl;
	cout << "########################################################" << endl;
	cout << endl;
	cout << "Nombre de points : " << ptPts->getNum() << endl;
	cout << endl;

	float temps[3];
	for (int i=0; i < ptPts->getNum(); ++i)
	{
		const point3D *val = ptPts->getValues(i);
		val->getValue(temps[0],temps[1],temps[2]);
		cout << "point [" << i+1 << "] : " << temps[0] << "\t" << temps[1] << "\t" << temps[2] << endl;
	}
}

//****************************************************************************

void OutilsListePoints::affiche(int i) const
{
	cout << "########################################################" << endl;
	cout << "#		COORDONNEES DU POINT			#" << endl;
	cout << "########################################################" << endl;
	cout << endl;
	cout << "Nombre de points : " << ptPts->getNum() << endl;
	cout << endl;

	float temps[3];

	const point3D *val = ptPts->getValues(i);
	val->getValue(temps[0],temps[1],temps[2]);
	cout << "point [" << i+1 << "] : " << temps[0] << "\t" << temps[1] << "\t" << temps[2] << endl;
	
}


//****************************************************************************
//			rotation
//****************************************************************************

void OutilsListePoints::rotationX(float angle)
{

	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D rotPt(&pts);

		rotPt.rotationX(angle);
		ptPts->set1Value(i, pts);
	}
}

//****************************************************************************

void OutilsListePoints::rotationY(float angle)
{

	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D rotPt(&pts);

		rotPt.rotationY(angle);
		ptPts->set1Value(i, pts);
	}
}

//****************************************************************************

void OutilsListePoints::rotationZ(float angle)
{

	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D rotPt(&pts);

		rotPt.rotationZ(angle);
		ptPts->set1Value(i, pts);
	}
}

//****************************************************************************
//			angle
//****************************************************************************

float * OutilsListePoints::AngleInclinaison(void)
{

	float *angle = new float[nbPoints()];

	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D anglPt(&pts);

		angle[i] = anglPt.AngleInclinaison();

	}

	return angle;
}

//****************************************************************************

float * OutilsListePoints::AngleAzimut(void)
{
	float *angle = new float[nbPoints()];

	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D anglPt(&pts);

		angle[i] = anglPt.AngleAzimut();

	}
	return angle;
}

//****************************************************************************

vector <vector <float> > OutilsListePoints::AnglesSpheriques(void)
{
	vector <vector <float> > angles;
	angles.resize(nbPoints());
	for (int i=0; i < nbPoints() ; i++)
	{
		angles[i].resize(2);
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D anglPt(&pts);
		angles[i][0] = anglPt.AngleInclinaison();
		angles[i][1] = anglPt.AngleAzimut();
	}
	return angles;
}

//****************************************************************************

vector <float> OutilsListePoints::Angles(const point3D *axe)
{
	vector <float> angles;
	angles.resize(nbPoints());
	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D anglPt(&pts);
		angles[i] = anglPt.Angle(axe);
	}
	return angles;
}

//****************************************************************************

vector <float> OutilsListePoints::Longueurs()
{
	vector <float> longs;
	longs.resize(nbPoints());
	for (int i=0; i < nbPoints() ; i++)
	{
		point3D pts( *(ptPts->getValues(i)) );
		OutilsPoint3D longPt(&pts);
		longs[i] = longPt.LongueurCorde();
	}
	return longs;
}








