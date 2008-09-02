#include "WRL/WRL.h"
#include "WRL/MaillageTriangulaire.h"

WRL::WRL(void)
{
	affichage = WRL::rien;
}

//---------------------------------------------------------------------------

WRL::WRL(option affiche)
{
	affichage = affiche;
}

//---------------------------------------------------------------------------

WRL::~WRL(void)
{

}

bool WRL::checkVRML(string nomFichier)
{
	objet3D * Obj = lire(nomFichier);
	if(Obj)
	{
		Lpoints3D *ptsObj = obtPoints(Obj);
		if(!ptsObj)
		{
			printf("Probleme de points de %s\n",nomFichier.c_str());
			delete ptsObj;
			Obj->unref();
			return false;
		}
		else
		{
			Lmailles *maillesObj = obtMailles(Obj);
			if(!maillesObj)
			{
				printf("Probleme de mailles de %s\n",nomFichier.c_str());
				delete ptsObj;
				delete maillesObj;
				Obj->unref();
				return false;
			}
			else
			{
				maillageTriangulaire *triangles = new maillageTriangulaire(maillesObj, ptsObj);
				if(!triangles->maillesTriangulaires())
				{
					printf("Probleme de mailles non triangulaires %s\n",nomFichier.c_str());
					delete ptsObj;
					delete maillesObj;
					delete triangles;
					Obj->unref();
					return false;
				}
				else
				{
					delete ptsObj;
					delete maillesObj;
					delete triangles;
					Obj->unref();
					return true;
				}
			}
		}
	}
	else
	{
		printf("Le fichier %s n'est pas lisible par COIN\n",nomFichier.c_str());
		return false;
	}
}



//****************************************************************************
//			POUR LIRE
//****************************************************************************

objet3D * WRL::lire(const string nomFichier)
{
	objet3D *obj = new objet3D();

	//ouverture du fichier
	SoInput *in = new SoInput();
	if ( !(in->openFile(nomFichier.c_str())) )
	{
		if (affichage == WRL::verbose)
			cerr << "! Pb ouverture de " << nomFichier << endl;
		else if (affichage == WRL::debug)
			cout << "! Pb ouverture de " << nomFichier << endl;
		return NULL;
	}

	// Chargement du modèle
	obj = SoDB::readAll(in);
	if (obj == NULL)
	{
		if (affichage == WRL::verbose)
			cerr << "! Pb en lecture de " << nomFichier << endl;
		if (affichage == WRL::debug)
			cout << "! Pb en lecture de " << nomFichier << endl;
		return NULL;
	}

	// fermeture du fichier
	delete in;
	if (affichage == WRL::verbose || affichage == WRL::debug)
		cout << " Scene Chargée " << endl;

	obj->ref();
	return obj;
}

//****************************************************************************
//			POUR CREER UN NV OBJET3D
//****************************************************************************

objet3D * WRL::creerObjet(void)
{
	objet3D *obj = new objet3D();
	obj->ref();
	return obj;
}

//---------------------------------------------------------------------------

objet3D * WRL::creerObjet(Lpoints3D *mesPoints)
{
	if ( mesPoints != NULL )
	{
		objet3D *obj = creerObjet();

		SoVRMLCoordinate *listePoint = new SoVRMLCoordinate;
		listePoint->point=*mesPoints;

		obj->addChild(listePoint);

		return obj;
	}

	if (affichage == WRL::verbose)
		cerr << "! PB creation objet : point NULL " << endl;
	if (affichage == WRL::debug)
		cout << "! PB creation objet : point NULL " << endl;

	return NULL;
}

//---------------------------------------------------------------------------

objet3D * WRL::creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, bool effetBrillant)
{
	if ( mesPoints != NULL && mesMailles != NULL )
	{

		objet3D *obj = creerObjet();
		if(effetBrillant)
		{
// TEXTURE
			SoVRMLMaterial *texture = new SoVRMLMaterial;
			texture->diffuseColor.setValue( 0.55, 0.55, 0.55 );
			texture->ambientIntensity.setValue( 0.54 );
			texture->specularColor.setValue ( 0.5, 0.5, 0.5 );
			texture->shininess.setValue(  0.15 );
			texture->transparency.setValue( 0 );
	
			obj->addChild(texture);
		}
// Objet

		SoVRMLCoordinate *listePoint = new SoVRMLCoordinate;
		SoVRMLIndexedFaceSet *listeFace = new SoVRMLIndexedFaceSet;

		// coord
		listePoint->point=*mesPoints;
		listeFace->coord=listePoint;

		// coordIndex
		listeFace->coordIndex=*mesMailles;

		// afficher le dos des mailles
		listeFace->solid = false;


		obj->addChild(listeFace);

		return obj;
	}

	if (affichage == WRL::verbose)
		cerr << "! PB creation objet : point ou maille NULL " << endl;
	if (affichage == WRL::debug)
		cout << "! PB creation objet : point ou maille NULL " << endl;

	return NULL;
}


//****************************************************************************

objet3D * WRL::creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, SoMFInt32 *couleursFaces)
{
	if ( mesPoints != NULL && mesMailles != NULL && couleursFaces != NULL)
	{
		objet3D *obj = creerObjet();

		SoVRMLCoordinate *listePoint = new SoVRMLCoordinate;
		SoVRMLIndexedFaceSet *listeFace = new SoVRMLIndexedFaceSet;
		SoVRMLColor *couleurs = new SoVRMLColor;

		// coord
		listePoint->point=*mesPoints;
		listeFace->coord=listePoint;

		// coordIndex
		listeFace->coordIndex=*mesMailles;

		// liste des couleurs
		couleurs->color.set1Value(0,1,0,0);	// rouge
		couleurs->color.set1Value(1,1,0.5,0);	// orange	
		couleurs->color.set1Value(2,1,1,0);	// jaune
		couleurs->color.set1Value(3,0.5,1,0);	// vert fluo
		couleurs->color.set1Value(4,0,1,0);	// vert
		couleurs->color.set1Value(5,0,1,0.5);	// vert bleu
		couleurs->color.set1Value(6,0,1,1);	// bleu ciel
		couleurs->color.set1Value(7,0,0.5,1);	// bleu 
		couleurs->color.set1Value(8,0,0,1);	// bleu
		couleurs->color.set1Value(9,0,0,0);	// noir
		couleurs->color.set1Value(10,1,1,1);	// blanc


		listeFace->color=couleurs;

		// colorIndex
		listeFace->colorIndex=*couleursFaces;

		// une couleur par face
		// et non une couleur par points
		listeFace->colorPerVertex=FALSE;

		// afficher le dos des mailles
		listeFace->solid = false;

		obj->addChild(listeFace);

		return obj;
	}

	if (affichage == WRL::verbose)
		cerr << "! PB creation objet : point ou maille ou couleur NULL " << endl;
	if (affichage == WRL::debug)
		cout << "! PB creation objet : point ou maille ou couleur NULL " << endl;

	return NULL;
}

//****************************************************************************

objet3D * WRL::creerObjet(Lpoints3D *mesPoints, Lmailles *mesMailles, SoMFInt32 *couleursFaces, int nbCouleurs)
{
	if ( mesPoints != NULL && mesMailles != NULL && couleursFaces != NULL)
	{
		objet3D *obj = creerObjet();

		SoVRMLCoordinate *listePoint = new SoVRMLCoordinate;
		SoVRMLIndexedFaceSet *listeFace = new SoVRMLIndexedFaceSet;

		// coord
		listePoint->point=*mesPoints;
		listeFace->coord=listePoint;

		// coordIndex
		listeFace->coordIndex=*mesMailles;

		// liste des couleurs
		SoVRMLColor *couleurs = Couleurs(nbCouleurs);


		listeFace->color=couleurs;

		// colorIndex
		listeFace->colorIndex=*couleursFaces;

		// une couleur par face
		// et non une couleur par points
		listeFace->colorPerVertex=FALSE;

		listeFace->solid = false;

		obj->addChild(listeFace);

		return obj;
	}

	if (affichage == WRL::verbose)
		cerr << "! PB creation objet : point ou maille ou couleur NULL " << endl;
	if (affichage == WRL::debug)
		cout << "! PB creation objet : point ou maille ou couleur NULL " << endl;

	return NULL;
}

//---------------------------------------------------------------------------

SoVRMLColor * WRL::Couleurs(int nbCouleurs)
{
	SoVRMLColor *couleurs = new SoVRMLColor;

        if ( nbCouleurs == 0 )
        {
          for (int i=0; i<255 ; i++)
          {
                  float col = 1.00 - ((float) i) / 255.0;
                  couleurs->color.set1HSVValue(i, 0,0,col);
          }
        }
	else if ( nbCouleurs <= 9 )
	{
		couleurs->color.set1Value(0,1,0,0);	// rouge
		couleurs->color.set1Value(1,1,0.5,0);	// orange	
		couleurs->color.set1Value(2,1,1,0);	// jaune
		couleurs->color.set1Value(3,0.5,1,0);	// vert fluo
		couleurs->color.set1Value(4,0,1,0);	// vert
		couleurs->color.set1Value(5,0,1,0.5);	// vert bleu
		couleurs->color.set1Value(6,0,1,1);	// bleu ciel
		couleurs->color.set1Value(7,0,0.5,1);	// bleu 
		couleurs->color.set1Value(8,0,0,1);	// bleu
	}

	else if ( nbCouleurs <= 16 )
	{
		couleurs->color.set1Value(0,1,1,1);			// blanc
		couleurs->color.set1Value(1,0,0,0);			// noir	
		couleurs->color.set1Value(2,0.5,0.5,0.5);		// gris
		couleurs->color.set1Value(3,0.75,0.75,0.75 );		// argent
		couleurs->color.set1Value(4,0,0,1);			// bleu
		couleurs->color.set1Value(5,0,0,0.5);			// bleu marine
		couleurs->color.set1Value(6,0,1,1);			// cyan
		couleurs->color.set1Value(7,0,0.5,0.5);			// cyan foncée 
		couleurs->color.set1Value(8,0,0.5,0);			// vert
		couleurs->color.set1Value(9,0,1,0);			// vert claire
		couleurs->color.set1Value(10,1,1,0);			// jaune
		couleurs->color.set1Value(11,0.5,0.5,0);		// vert olive
		couleurs->color.set1Value(12,1,0,0);			// rouge
		couleurs->color.set1Value(13,0.5,0,0);			// marron
		couleurs->color.set1Value(14,1,0,1);			// lilas = magenta
		couleurs->color.set1Value(15,0.5,0,0.5);		// pourpre
	}	

	else if ( nbCouleurs <= 28 )
	{
		couleurs->color.set1Value(0,0,0,0);			// noir	
		couleurs->color.set1Value(1,0.25,0.25,0.25);		// gris fonce
		couleurs->color.set1Value(2,0.5,0.5,0.5);		// gris
		couleurs->color.set1Value(3,0.75,0.75,0.75 );		// argent
		couleurs->color.set1Value(4,1,1,1);			// blanc

		couleurs->color.set1Value(5,0,0,0.25);			// bleu claire
		couleurs->color.set1Value(6,0,0,0.5);			// bleu marine
		couleurs->color.set1Value(7,0,0,0.75);			// bleu marine
		couleurs->color.set1Value(8,0,0,1);			// bleu

		couleurs->color.set1Value(9,0,0.25,0.25);		//  
		couleurs->color.set1Value(10,0,0.5,0.5);		// cyan foncée
		couleurs->color.set1Value(11,0,0.75,0.75);		// 
		couleurs->color.set1Value(12,0,1,1);			// cyan

		couleurs->color.set1Value(12,0,0.25,0);			// 
		couleurs->color.set1Value(13,0,0.5,0);			// vert
		couleurs->color.set1Value(14,0,0.75,0);			// 
		couleurs->color.set1Value(15,0,1,0);			// vert claire

		couleurs->color.set1Value(16,0.25,0.25,0);			// jaune
		couleurs->color.set1Value(17,0.5,0.5,0);		// vert olive
		couleurs->color.set1Value(18,0.75,0.75,0);			// jaune
		couleurs->color.set1Value(19,1,1,0);		// vert olive

		couleurs->color.set1Value(20,0.25,0,0);			// rouge
		couleurs->color.set1Value(21,0.5,0,0);			// marron
		couleurs->color.set1Value(22,0.75,0,0);			// rouge
		couleurs->color.set1Value(23,1,0,0);			// marron


		couleurs->color.set1Value(24,0.25,0,0.25);			// lilas = magenta
		couleurs->color.set1Value(25,0.5,0,0.5);		// pourpre
		couleurs->color.set1Value(26,0.75,0,0.75);			// lilas = magenta
		couleurs->color.set1Value(27,1,0,1);		// pourpre

	}
	else
	{
// 		float tps = ((float) nbCouleurs)/2;
// 		int nbTeinte = (int) ceil(tps);
// 		for (int i=0; i<nbTeinte ; i++)
// 		{
// 			float teinte = ((float) i )/(nbTeinte);
// 			couleurs->color.set1HSVValue(i*2, teinte, 0.75, 0.75 );
// 			couleurs->color.set1HSVValue(i*2 +1, teinte, 1, 1 );
// 		}

		for (int i=0; i<nbCouleurs ; i++)
		{
			float teinte = ((float) i )/(nbCouleurs);
			couleurs->color.set1HSVValue(i, teinte, 1, 1 );
		}

	}


	return couleurs;
}

//****************************************************************************
//			POUR DETRUIRE UN OBJET3D
//****************************************************************************

void WRL::detruireObjet(objet3D *obj)
{
	if (obj != NULL)
		obj->unref();
}

//****************************************************************************
//			POUR ECRITURE
//****************************************************************************

bool WRL::ecrire(const string nomFichier, objet3D *obj)
{
	if ( obj != NULL )
	{
		SoWriteAction wa;
		SoOutput *out=wa.getOutput();
		if ( out->openFile(nomFichier.c_str()) != false)
		{
			wa.apply(obj);
			if (affichage == WRL::verbose || affichage == WRL::debug)
				cout << " WRL : " << nomFichier << " ECRIT" << endl;
			return true;
		}
		else
		{
			if (affichage == WRL::debug )
				cerr << "PB ecriture du fichier : " << nomFichier << endl;
			if (affichage == WRL::verbose )
				cout << "PB ecriture du fichier : " << nomFichier << endl;
			return false;
		}
		delete out;
	}

	if (affichage == WRL::debug )
		cerr << "L'objet est vide" << endl;
	if (affichage == WRL::verbose )
		cout << "L'objet est vide"  << endl;
	return false;
}

//****************************************************************************
//			OUTILS LECTURE
//****************************************************************************

Lpoints3D * WRL::obtPoints(objet3D *obj)
{
	SoSearchAction act;
	SoPathList liste;
	SoFullPath *path;

	Lpoints3D *points = new Lpoints3D;

	// recherche point VRML1
 	act.setType(SoCoordinate3::getClassTypeId());
 	act.setInterest(SoSearchAction::ALL);
 	act.apply(obj);
 	liste = act.getPaths();
	if (liste.getLength() != 0)
	{
		path = (SoFullPath *) liste[0];
		SoCoordinate3 *co = (SoCoordinate3 *) path->getTail();
		*points = co->point;
		if (affichage == WRL::debug || affichage == WRL::verbose)
			cout << "VRML1 : Nb de points trouvés : " << points->getNum() << endl;
			
		return points;
	}

	// recherche point VRML2
 	act.setType(SoVRMLCoordinate::getClassTypeId());
 	act.setInterest(SoSearchAction::ALL);
 	act.apply(obj);
 	liste = act.getPaths();
	if (liste.getLength() != 0)
	{
		path = (SoFullPath *) liste[0];
		SoVRMLCoordinate *co = (SoVRMLCoordinate *) path->getTail();
		*points = co->point;
		if (affichage == WRL::debug || affichage == WRL::verbose)
			cout << "VRML2 : Nb de points trouvés : " << points->getNum() << endl;
		return points;
	}

	if ( affichage == WRL::debug )
		cout << "PB : (  WRL::obtPoints ) Aucun point trouvé" << endl;
	else if ( affichage == WRL::verbose )
		cerr << "PB : (  WRL::obtPoints ) Aucun point trouvé" << endl;

	return NULL;
}

//---------------------------------------------------------------------------

Lmailles* WRL::obtMailles(objet3D *obj)
{
	SoSearchAction act;
	SoPathList liste;
	SoFullPath *path;
	Lmailles *indexPts = new Lmailles;


// recherche facettes VRML1
 	act.setType(SoIndexedShape::getClassTypeId());
 	act.setInterest(SoSearchAction::ALL);
 	act.apply(obj);
 	liste = act.getPaths();
	if (liste.getLength() != 0)
	{
		path = (SoFullPath *) liste[0];
		SoIndexedShape *co = (SoIndexedShape *) path->getTail();
		*indexPts = co->coordIndex;
		if (affichage == WRL::debug || affichage == WRL::verbose)
			cout << "VRML1 : Nb de mailles trouvées : " << calculNbMaille(indexPts) << endl;
		return indexPts;
	}
	
// recherche facettes VRML97
 	act.setType(SoVRMLIndexedShape::getClassTypeId());
 	act.setInterest(SoSearchAction::ALL);
 	act.apply(obj);
 	liste = act.getPaths();
	if (liste.getLength() != 0)
	{
		path = (SoFullPath *) liste[0];
		SoVRMLIndexedShape *co = (SoVRMLIndexedShape *) path->getTail();
		*indexPts = co->coordIndex;
		if (affichage == WRL::debug || affichage == WRL::verbose)
			cout << "VRML2 : Nb de mailles trouvées : " << calculNbMaille(indexPts) << endl;
		return indexPts;
	}

	if ( affichage == WRL::debug )
		cout << "PB : Aucune maille trouvée" << endl;
	else if ( affichage == WRL::verbose )
		cerr << "PB : Aucune maille trouvée" << endl;

	return NULL;
}

//---------------------------------------------------------------------------

int  WRL::calculNbMaille(const Lmailles *indexPts)
{
	if (indexPts != NULL)
	{
		int   iface = 0;
		const int* val = indexPts->getValues(0);
		for (int idx = 0 ; idx < indexPts->getNum() ; idx++)
		{
			if (  val[idx]== -1 ) ++iface;
 		}
		return iface;
	}
	else
	{
		if (affichage == WRL::debug || affichage == WRL::verbose)
			cerr << "ERREUR : ( WRL::calculNbMaille ) Lmailles == NULL" << endl;
		return -1;
	}
}

//****************************************************************************
//			CONVERSION
//****************************************************************************

bool WRL::Vrml1ToVrml2(const string nomVRML1, const string nomVRML2,bool materiau)
{
	objet3D *objVrml1 = lire(nomVRML1);
	if ( objVrml1 != NULL )
	{
		objet3D * objVrml2 = creerObjet(obtPoints(objVrml1), obtMailles(objVrml1),materiau );
		bool ok = ecrire(nomVRML2, objVrml2);
		detruireObjet(objVrml1);
		detruireObjet(objVrml2);
		return ok;
	}
	if (affichage == WRL::debug || affichage == WRL::verbose)
		cerr << "ERREUR : ( WRL::Vrml1ToVrml2 ) ouverture de " << nomVRML1 << endl;
	return false;
}

//****************************************************************************
//			COULEUR
//****************************************************************************

SoMFInt32 * WRL::coloriserFaces(int *TcouleursMailles, int nbMailles)
{
	if (TcouleursMailles == NULL)
		return NULL;

	SoMFInt32 *couleurFaces = new SoMFInt32;

	for(int i=0; i<nbMailles; i++)
	{
		couleurFaces->set1Value(i, TcouleursMailles[i]);
	}
	return couleurFaces;
}

//---------------------------------------------------------------------------

SoMFInt32 * WRL::coloriserFaces(vector<int> TcouleursMailles)
{
	if (TcouleursMailles.empty())
	{
		return NULL;
	}

	SoMFInt32 *couleurFaces = new SoMFInt32;

	for(unsigned int i=0; i<TcouleursMailles.size(); i++)
	{
		couleurFaces->set1Value(i, TcouleursMailles[i]);
	}
	return couleurFaces;
}


