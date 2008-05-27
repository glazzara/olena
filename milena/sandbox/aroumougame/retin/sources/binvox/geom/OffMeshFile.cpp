//
// OffMeshFile.cpp
// Classe permettant de lire un off et de creer le Mesh.
//
//

#include "binvox/geom/OffMeshFile.h"


using namespace binvox;

OffMeshFile::OffMeshFile(Mesh& mesh_ref, string filespec) :
		MeshFile(mesh_ref, filespec)
{
}  // constructor



OffMeshFile::~OffMeshFile()
{
}  // destructor


int
OffMeshFile::load()
{
	mesh.clear();
	
	//chargement du off	
	bool marche = off.charger(my_filespec);
	if(!marche)
		return 0;
	
	const vector<float>& xSmt = off.x();
	const vector<float>& ySmt = off.y(); 
	const vector<float>& zSmt = off.z();
	const vector <int* >& mailles =  off.m();
	
	//creation du Mesh
	Vertex *new_vertex_p;
	Face *new_face_p;
	
	for(uint i=0; i< xSmt.size();i++)
	{
		new_vertex_p = new Vertex(xSmt.at(i),ySmt.at(i),zSmt.at(i));
		mesh.vertices.push_back(new_vertex_p);
	}

	for(uint i=0; i< mailles.size();i++)
	{
		new_face_p = new Face();
		for(uint j=0;j<mailles.at(i)[0];j++)
		{
			new_face_p->add_vertex(mailles.at(i)[j+1]);
		}
		mesh.faces.push_back(new_face_p);
	}
	
	cout << "Read " << mesh.faces.size() << " faces, " << mesh.vertices.size() << " vertices." << endl;
	return 1;
	
}  // WrlMeshFile::load



