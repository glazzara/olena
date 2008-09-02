#include <math.h>
#include "binvox/geom/Face.h"
#include "WRL/WRL.h"
#include "WRL/Coin.h"
#include "WRL/OFF.h"
#include "WRL/Objet3D.h"
#include "Trimesh/TriMesh.h"
#include "Trimesh/TriMesh_algo.h"
#include "Trimesh/mesh_filter.h"
#include "Trimesh/mesh_shade.h"
#include <mln/core/point3d.hh>
#include <mln/core/p_graph.hh>
#include <mln/util/graph.hh>
#include <mln/core/graph_image.hh>
#include <mln/core/line_graph_image.hh>
#include <mln/core/line_graph_elt_neighborhood.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/math/abs.hh>

#define NEXT(i) ((i)<2 ? (i)+1 : (i)-2)
#define PREV(i) ((i)>0 ? (i)-1 : (i)+2)
#define PI 3.14159265

int main()
{
  float x0,y0,z0,x1,x2,y1,y2,z1,z2;
//  int n=0;
  SoDB::init();
  WRL* Vrml = new WRL();
  const string& my_filespec = "m9l0.wrl";

// permet la lecture d'un fichier vrml
  objet3D *Obj = Vrml->lire(my_filespec);


  if(!Obj) 
  {
    printf("Le fichier %s n'est pas lisible par COIN\n",my_filespec.c_str());
    return 0;
  }

//  std::string nom("test.wrl");
//  Vrml->ecrire(nom, Obj);
//  std::cout << (Vrml->checkVRML(nom)) << std::endl;
  
// recuperation des points
  Lpoints3D *ptsObj = Vrml->obtPoints(Obj);

  if(!ptsObj)
  {
    printf("Probleme de points de %s\n",my_filespec.c_str());
    delete ptsObj;
    Obj->unref();
    return 0;
  }

  std::vector<double> edge_values;
  std::vector<double> values;
  std::vector<float> fcurv,fcurv1,fcurv2;
  std::vector<long double> vect1,vect2;
  std::vector<int> curv;
  
  using namespace mln;
  
  util::graph<point3d> g;

  binvox::Face *new_face_p;
  std::vector<binvox::Face *> faces;
  Lmailles* mailles = Vrml->obtMailles(Obj);
 
  int nbP = ptsObj->getNum();
 
  TriMesh::TriMesh *trim = new TriMesh();

  trim->vertices.resize(nbP);
  
  for(int i = 0; i < nbP; i++)
  {
    (*ptsObj)[i].getValue(x0,y0,z0);
    trim->vertices[i][0] = x0;
    trim->vertices[i][1] = y0;
    trim->vertices[i][2] = z0;
  }
  


  new_face_p = new binvox::Face::Face();
  for(int i=0; i< mailles->getNum();i++)
  {
    if ((*mailles)[i] != -1)
      new_face_p->add_vertex((*mailles)[i]);
    else
    {
//       faire un reserve si hors de cette boucle
      trim->faces.push_back(TriMesh::Face((*new_face_p)[0],(*new_face_p)[1],(*new_face_p)[2]));
      faces.push_back(new_face_p);
      new_face_p = new binvox::Face::Face();
    }
  }
  
  int nbF = faces.size();

  trim->write("norm:curv:dcurv:ply_ascii:test.ply");

  //   trim->need_dcurv();
  shade("test.ply",1,1.5,"norm:curv:dcurv:ply_ascii:test1.ply");
  
  float k1,k2,X;
  fcurv.resize(nbF);
  fcurv1.resize(nbF);
  fcurv2.resize(nbF);

  for (int i = 0; i < nbF; i++) 
  {
    k1 = (trim->curv1[trim->faces[i].v[0]]+trim->curv1[trim->faces[i].v[1]]+trim->curv1[trim->faces[i].v[2]])/3;
    k2 = (trim->curv2[trim->faces[i].v[0]]+trim->curv2[trim->faces[i].v[1]]+trim->curv2[trim->faces[i].v[2]])/3;
    
    X= (k1 + k2)/2.;//courbure moyenne
    
    fcurv1[i]=k1;
    fcurv2[i]=k2;
    fcurv[i]=(atan(-X)+PI/2);
  }
  

  for (int i = 0; i < nbF; i++)
  {
    curv.push_back(int(fcurv[i]*100));
//    curv.push_back(int(min(max(double(256.0/PI*fcurv[i]),0.0), 255.99)));
//     std::cout << "curv: " << curv[i] << std::endl;
  }
  
  double milieuX = 0;
  double milieuY = 0;
  double milieuZ = 0;
   double val = 0;
  std::vector<double> col;
  
  for(int i=0; i < nbF; i++)
  {
    (*ptsObj)[(*faces[i])[0]].getValue(x0,y0,z0);
    (*ptsObj)[(*faces[i])[1]].getValue(x1,y1,z1);
    (*ptsObj)[(*faces[i])[2]].getValue(x2,y2,z2);
    
    
    x0 = x0*10000;
    x1 = x1*10000;
    x2 = x2*10000;
    y0 = y0*10000;
    y1 = y1*10000;
    y2 = y2*10000;
    z0 = z0*10000;
    z1 = z1*10000;
    z2 = z2*10000;

    milieuX = (x0+x1+x2)/3;
    milieuY = (y0+y1+y2)/3;
    milieuZ = (z0+z1+z2)/3;
    
    val = curv[(*faces[i])[0]]+curv[(*faces[i])[1]]+curv[(*faces[i])[2]];
//     std::vector<double> tmp;
//     tmp.push_back(milieuX*100);
//     tmp.push_back(milieuY*100);
//     tmp.push_back(milieuZ*100);
//     tmpPoint.push_back(tmp);

    g.add_node(point3d(milieuX, milieuY, milieuZ));
    col.push_back(val/3);
//     std::cout << (int)(val/3) << std::endl;
  
  }
  
//  objet3D * object = Vrml->creerObjet( ptsObj,  mailles, true);

//   std::cout << (Vrml->checkVRML(nom)) << std::endl;
  
  for(int i=0; i < nbF; i++)
  {
    for(int j=0; j < nbF; j++)
    {
      if(i!=j)
      {
        if((*faces[i]).has_same_edge(*faces[j]))
        {
  //         std::cout << i << " " << j << std::endl;
          g.add_edge(i,j);
//           t_pair p(i,j);
//           tmpEdge.push_back(p);
          edge_values.push_back(math::abs(col[i]-col[j]));
        }
      }
    }
  }

  


  typedef line_graph_image < point3d, int > gimage;

  p_line_graph<point3d> plg(g);
  line_graph_image<point3d, double> lgi(plg,col,edge_values);
// int i=5;
//  mln_piter_(gimage ) p(lgi.domain());
//   for_all (p)
//     std::cout << "ima (" << p << ") = " << lgi(p) << std::endl;
//     std::cout << "ima (" << p << ") = " << (lgi.node_values())[i++] << std::endl;
/*
for(int i=0; i < lgi.node_values().size(); i++)
{
  std::cout << "node (" << i << ") = " << (lgi.node_values())[i] << std::endl;
}
for(int i=0; i < lgi.edge_values().size(); i++)
{
  std::cout << "edge (" << i << ") = " << (lgi.edge_values())[i] << std::endl;
}*/


//  std::cout << "nb pt values: " << values.size() << std::endl;
  std::cout << "nb arrete: " << edge_values.size() << std::endl;
  
  
  line_graph_elt_neighborhood<point3d> lgen;
  value::int_u16 nb_bassin;
  line_graph_image<point3d, value::int_u16> output;

//////////////////////////////////
  typedef line_graph_image<point3d, value::int_u16> ima_t;
  ima_t closed_lg_ima(lgi.domain());
  morpho::closing_area(lgi,lgen,2,closed_lg_ima);
  output = morpho::meyer_wst(closed_lg_ima,lgen,nb_bassin);  
/////////////////////////////////
//  output = morpho::meyer_wst(lgi,lgen,nb_bassin);

  std::cout << "nombre de bassin: " << nb_bassin << std::endl;
  std::vector<int> valcol;
  
  typedef line_graph_image<point3d,value::int_u16> out_t;
  
   mln_piter_(out_t) pout(output.domain());
   for_all(pout)
   {
   valcol.push_back(output(pout));
   std::cout << output(pout) << std::endl;
   }

  std::cout << "taille valcol: " << valcol.size() << std::endl;
  std::cout << "nombre points: " << nbP << std::endl;
  std::cout << "nombre faces: " << nbF << std::endl;



  SoMFInt32 * colorFace = Vrml -> coloriserFaces(valcol);
  objet3D * object = Vrml->creerObjet( ptsObj,  mailles, colorFace,nb_bassin);
  std::string nom("test.wrl");
  Vrml->ecrire(nom,  object);

  Vrml->detruireObjet(Obj);
  delete ptsObj;
  delete mailles;

}
