//
// WrlMeshFile.cpp
// Classe permettant de lire un vrml et de creer le Mesh.
//
//

#include "binvox/geom/WrlMeshFile.h"


using namespace binvox;

WrlMeshFile::WrlMeshFile(Mesh& mesh_ref, string filespec) :
		MeshFile(mesh_ref, filespec)
{
	if (SoDB::isInitialized() != true) SoDB::init();
	Vrml = new WRL;
}  // constructor



WrlMeshFile::~WrlMeshFile()
{
	if (Vrml != NULL)
		delete Vrml;
}  // destructor


int
WrlMeshFile::load()
{
	mesh.clear();
	
	//chargement du vrml	
	
	objet3D *Obj = Vrml->lire(my_filespec);
	if(!Obj) 
	{
		printf("Le fichier %s n'est pas lisible par COIN\n",my_filespec.c_str());
		return 0;
	}
	Lpoints3D *ptsObj = Vrml->obtPoints(Obj);
	if(!ptsObj)
	{
		printf("Probleme de points de %s\n",my_filespec.c_str());
		delete ptsObj;
		Obj->unref();
		return 0;
	}
	Lmailles *maillesObj = Vrml->obtMailles(Obj);
	if(!maillesObj)
	{
		printf("Probleme de mailles de %s\n",my_filespec.c_str());
		delete ptsObj;
		delete maillesObj;
		Obj->unref();
		return 0;
	}
	
	//creation du Mesh
	Vertex *new_vertex_p;
	Face *new_face_p;
	
	Lpoints3D* points = Vrml->obtPoints(Obj);
	for(uint i=0; i< points->getNum();i++)
	{
		new_vertex_p = new Vertex((*points)[i][0],(*points)[i][1],(*points)[i][2]);
		mesh.vertices.push_back(new_vertex_p);
	}
	
	Lmailles* mailles = Vrml->obtMailles(Obj);
	new_face_p = new Face();
	for(uint i=0; i< mailles->getNum();i++)
	{
		if ((*mailles)[i] != -1)
			new_face_p->add_vertex((*mailles)[i]);
		else
		{
			mesh.faces.push_back(new_face_p);
			new_face_p = new Face();
		}
	}
	
	//nettoyage et fin
	delete points;
	delete mailles;
	Vrml->detruireObjet(Obj);
	delete ptsObj;
	delete maillesObj;
	
	cout << "Read " << mesh.faces.size() << " faces, " << mesh.vertices.size() << " vertices." << endl;
	return 1;
	
}  // WrlMeshFile::load



