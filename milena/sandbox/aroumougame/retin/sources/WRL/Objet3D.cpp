/***********************************************************************/
/*!
 * @file Objet3D.cpp
 * @date Mars. 2008
 * @author Julien Gony<BR>
 *         Université de Cergy-Pontoise<BR>
 *         ETIS - Equipe Image - UMR CNRS 8051
 *
 * @brief  Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 *
 * Permet de lire, écrire, obtenir le maillage et les points d'un objet 3D
 *
 * @version 1.0
 */
/************************************************************************/

#include "WRL/Objet3D.h"
#include "WRL/WRL.h"
#include "WRL/OFF.h"


Objet3D::Objet3D(const string& nomFichier):_nomFichier(nomFichier),_points(NULL),_mailles(NULL)
{
	_type=type(nomFichier);
	if(_type==INVALID)
	{
		printf("type invalide d'objet3D\n");
	}
	else if(_type==VRML)
	{
		WRL* vrml = new WRL;
		objet3D *Obj = vrml->lire(_nomFichier);
		if(!Obj) 
		{
			printf("Le fichier %s n'est pas lisible par COIN\n",_nomFichier.c_str());
			return;
		}
		_points = vrml->obtPoints(Obj);
		if(!_points)
		{
			printf("Probleme de points de %s\n",_nomFichier.c_str());
			delete _points;
			Obj->unref();
			return;
		}
		_mailles = vrml->obtMailles(Obj);
		if(!_mailles)
		{
			printf("Probleme de mailles de %s\n",_nomFichier.c_str());
			delete _points;
			delete _mailles;
			Obj->unref();
			return;
		}
	
		vrml->detruireObjet(Obj);
		delete vrml;
	}
	else
	{
		Off off;
		bool marche = off.charger(_nomFichier);
		if(marche)
		{
			const vector<float>& xSmt = off.x();
			const vector<float>& ySmt = off.y(); 
			const vector<float>& zSmt = off.z();
		
			const vector <int* >& mailles =  off.m();
			
			_points = new Lpoints3D;
			for(int i=0; i< xSmt.size();i++)
				_points->set1Value(i,xSmt.at(i),ySmt.at(i),zSmt.at(i));
			
			_mailles = new Lmailles;
			for(int i=0; i< mailles.size();i++)
			{
				if(mailles.at(i)[0]!=3)
				{
					printf("Le maillage doit etre triangulaire\n");
					continue;
				}
				for(int j=0;j<mailles.at(i)[0];j++)
					_mailles->set1Value(i*4+j,mailles.at(i)[j+1]);
				
				_mailles->set1Value(i*4+mailles.at(i)[0],-1);
			}
		}
		else
		{
			printf("probleme avec lecture %s\n",_nomFichier.c_str());
			return;
		}
	}
}


Objet3D::Objet3D(Lpoints3D *points, Lmailles *mailles,int type) :_type(type),_nomFichier(""),_points(points),_mailles(mailles)
{
}

Objet3D::~Objet3D()
{
	if(_points)
		delete _points;
	if(_mailles)
		delete _mailles;
}


/****************************** Fonctions statiques ***************************************************/
bool Objet3D::check(const string& nomFichier)
{
	int typeObjet = type(nomFichier);
	if(typeObjet==INVALID)
		return false;
	if(typeObjet==VRML)
	{
		WRL *unVrml = new WRL;
		bool verif = unVrml->checkVRML(nomFichier);
		delete unVrml;
		return verif;
	}
	if(typeObjet==OFF)
	{
		bool verif = Off::check(nomFichier);
		return verif;
	}
}

int Objet3D::type(const string& fichier)
{
	int i = fichier.find_last_of('.',-1);
	if(i>=0)
	{
		string ext = fichier.substr(i);
		if((ext==".vrml")||(ext==".VRML")||(ext==".wrl")||(ext==".WRL")||(ext==".iv")||(ext==".IV"))
			return VRML;
		if((ext==".off")||(ext==".OFF"))
			return OFF;
		else
			return INVALID;
	}
	else
		return INVALID;
}
