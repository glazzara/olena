//
// OffMeshFile.h
// Classe permettant de lire un off et de creer le Mesh.
//
//

#ifndef __OffMeshFile_h
#define __OffMeshFile_h

#include "binvox/geom/MeshFile.h"
#include "WRL/OFF.h"
namespace binvox {
	
class OffMeshFile : public MeshFile
{

public:

  OffMeshFile(Mesh& mesh_ref, string filespec);
  ~OffMeshFile();

  int load();
  void save() {}
  

  
private:

	Off off;
};

}
#endif


