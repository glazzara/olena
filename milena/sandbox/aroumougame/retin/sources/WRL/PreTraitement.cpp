#include "WRL/PreTraitement.h"
#define PI 3.1415926535897932

PreTraitement::PreTraitement(Lpoints3D *ptPtsVrml)
{
	ptPts = ptPtsVrml;
	calculCentre();
	axe1.setValue(0,0,0);
	axe2.setValue(0,0,0);
}

PreTraitement::~PreTraitement(void)
{

}

//****************************************************************************
//			CENTRE DE GRAVITE
//****************************************************************************

void PreTraitement::calculCentre(void)
{
	float tpsX, tpsY, tpsZ;
	double cX = 0.0, cY = 0.0, cZ = 0.0;

	for (int i=0; i < ptPts->getNum(); ++i)
	{
		const point3D *val = ptPts->getValues(i);
		val->getValue( tpsX, tpsY, tpsZ);
		cX += tpsX;
		cY += tpsY;
		cZ += tpsZ;
	}
	cX /= ptPts->getNum();
	cY /= ptPts->getNum();
	cZ /= ptPts->getNum();


	centreObjet.setValue((float) cX, (float) cY, (float) cZ);
}

//****************************************************************************

void PreTraitement::afficheCentre(void) const
{
	cout << "Centre Gravite : " << centreObjet[0] << " , " << centreObjet[1] << " , " << centreObjet[2] << endl;
}

//****************************************************************************

bool PreTraitement::centrer(void)
{
	float temps[3];
	float x,y,z;
	
	centreObjet.getValue(x,y,z);
	for (int i=0; i < ptPts->getNum(); ++i)
	{
		const point3D *val = ptPts->getValues(i);
		val->getValue(temps[0],temps[1],temps[2]);
		temps[0]-=x;
		temps[1]-=y;
		temps[2]-=z;
		ptPts->set1Value(i,temps);
	}
	calculCentre();
	if(estCentre())
		return true;
	else
		return false;
}

//****************************************************************************

bool PreTraitement::estCentre(void)
{
	float centre[3]; 
	centreObjet.getValue(centre[0],centre[1],centre[2]);

	if ( fabs(centre[0]) <= PRECISION && fabs(centre[1]) <= PRECISION && fabs(centre[2]) <= PRECISION )
		return true;
	else
		return false;
}

//****************************************************************************
//			AXES PRINCIPAUX
//****************************************************************************

void PreTraitement::ACP(void)
{
	float tpsX, tpsY, tpsZ;	
	float autoCorr[6]={0,0,0,0,0,0};
	float direction1[3], direction2[3];
	

	//on centre et on reduit les valeurs + calcul de la matrice de correlation
	if (!estCentre())
		centrer();

	for (int i=0; i < ptPts->getNum(); ++i)
	{
		const SbVec3f *val = ptPts->getValues(i);
		val->getValue(tpsX,tpsY,tpsZ);
		autoCorr[0]+=(tpsX*tpsX);	autoCorr[1]+=(tpsY*tpsY);	autoCorr[2]+=(tpsZ*tpsZ);
		autoCorr[3]+=(tpsX*tpsY);	autoCorr[4]+=(tpsX*tpsZ);	autoCorr[5]+=(tpsY*tpsZ);
	}
	autoCorr[0]=autoCorr[0]/ptPts->getNum();
	autoCorr[1]=autoCorr[1]/ptPts->getNum();
	autoCorr[2]=autoCorr[2]/ptPts->getNum();
	autoCorr[3]=autoCorr[3]/ptPts->getNum();
	autoCorr[4]=autoCorr[4]/ptPts->getNum();
	autoCorr[5]=autoCorr[5]/ptPts->getNum();

	MatriceF autocorrelation(3,autoCorr);
	//autocorrelation.affiche();
	autocorrelation.obtDirectionsPrincipales(direction1,direction2);

// recherche si direction1 est suivant X, Y ou Z
	int max1=0;
	float dirMax=fabs(direction1[0]);
	for (int i=1 ; i < 3 ; i++)
		if (dirMax <= fabs(direction1[i]))
		{
			dirMax=fabs(direction1[i]);
			max1=i;
		}
		
// on suppose que l'objet a été numérisé suivant sa direction principale
// => direction1[max] > 0
	if (direction1[max1] < 0)
	{
		direction1[0] = - direction1[0];
		direction1[1] = - direction1[1];
		direction1[2] = - direction1[2];
	}

// recherche si direction2 est suivant X, Y ou Z
	int max2=0;
	dirMax=fabs(direction2[0]);
	for (int i=1 ; i < 3 ; i++)
		if (dirMax <= fabs(direction2[i]))
		{
			dirMax=fabs(direction2[i]);
			max2=i;
		}

	if (max1 == max2)
		cerr << "PB deux orientations principales ont meme importances" << endl;

	axe1.setValue(direction1[0],direction1[1],direction1[2]);

// on verrifie que le produit vectoriel des deux axes est positif suivant l'autre axe
	int max3;
	if (max1 + max2 == 1)
		max3 = 2;
	else if (max1 + max2 == 2)
		max3 = 1;
	else
		max3 = 0;

	SbVec3f *normal = produitVectoriel(&axe1, &axe2);
	normer(normal);
	normal->getValue(direction1[0],direction1[1],direction1[2]);
	if (direction1[max3] < 0)
	{
		// axe2 du mauvais sens
		direction2[0] = - direction2[0];
		direction2[1] = - direction2[1];
		direction2[2] = - direction2[2];
	}

	axe2.setValue(direction2[0],direction2[1],direction2[2]);

	delete normal;
}
//****************************************************************************

void PreTraitement::afficheAxes(void) const
{
	cout << " \t \t ACP" << endl;
	cout << "AXE1 : " << axe1[0] << " , " << axe1[1] << " , " << axe1[2] << endl;
	cout << "AXE2 : " << axe2[0] << " , " << axe2[1] << " , " << axe2[2] << endl;
}


void PreTraitement::aligner(void)
{
// alignement suivant axe principal
	// cherche AXE1
	ACP();
	OutilsPoint3D ptAxe1(&axe1);
	// angle entre l'axe Y est le projete de axe1 sur YOZ
	float angle = -ptAxe1.AngleYoZ();
	float angle2 = PI/2 - ptAxe1.AngleXoY();
	// rotation perpendiculaire a X de - angle
	OutilsListePoints modifPoints(ptPts);
	modifPoints.rotationX(angle);
	modifPoints.rotationZ(angle2);

// idem pour alignement suivant axe2
	ACP();
	OutilsPoint3D ptAxe2(&axe2);
	angle2 = ptAxe2.AngleZoX();
	angle2 = - angle2 - PI/2;
	modifPoints.rotationY(angle2);
	
}

//****************************************************************************



