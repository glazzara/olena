#ifndef __Coin_h__
#define __Coin_h__


// DOCUMENTATION COIN3D
// http://www-evasion.imag.fr/Membres/Francois.Faure/doc/inventorMentor/sgi_html/
// file:///usr/lsa/docs/Coin-2.4.4/index.html
//http://doc.coin3d.org/Coin/index.html

// pour chargement
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>

// sauvegarde
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

// pour parcours du fichier
#include <Inventor/nodes/SoSeparator.h>	

// pour trouver un element du fichier
#include <Inventor/actions/SoSearchAction.h>

// pour pointer sur les points
#include <Inventor/nodes/SoCoordinate3.h>		// VRML1
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>	// VRML2

// pour les faces
#include <Inventor/nodes/SoIndexedShape.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h>

// pour les lignes
#include <Inventor/VRMLnodes/SoVRMLIndexedLineSet.h>

// pour la couleur
#include <Inventor/VRMLnodes/SoVRMLColor.h>

// liste de points
#include <Inventor/fields/SoMFVec3f.h>

// texture
#include <Inventor/VRMLnodes/SoVRMLMaterial.h>

typedef SoSeparator objet3D;
typedef SoMFVec3f Lpoints3D;
typedef SoMFInt32 Lmailles;
typedef SbVec3f point3D;




#endif

