//
// WrlMeshFile.h
// Classe permettant de lire un vrml et de creer le Mesh.
//
//

#ifndef __WrlMeshFile_h
#define __WrlMeshFile_h

#include "binvox/geom/MeshFile.h"
#include "WRL/WRL.h"
namespace binvox {
	
class WrlMeshFile : public MeshFile
{

public:

  WrlMeshFile(Mesh& mesh_ref, string filespec);
  ~WrlMeshFile();

  int load();
  void save() {}
  

  
private:

	WRL* Vrml;
};  // WrlMeshFile class

}
#endif


