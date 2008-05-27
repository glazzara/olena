#include <math.h>
#include "binvox/geom/Face.h"
#include "WRL/WRL.h"
#include "WRL/Coin.h"
#include "WRL/OFF.h"
#include "WRL/Objet3D.h"
#include <mln/core/point3d.hh>
#include <mln/core/p_graph.hh>
#include <mln/util/graph.hh>
#include <mln/core/graph_image.hh>
#include <mln/core/line_graph_image.hh>
#include <mln/core/line_graph_elt_neighborhood.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/closing_area_on_vertices.hh>

#define NEXT(i) ((i)<2 ? (i)+1 : (i)-2)
#define PREV(i) ((i)>0 ? (i)-1 : (i)+2)
#define PI 3.14159265

int main()
{
  float x0,y0,z0,x1,x2,y1,y2,z1,z2;
  int n=0;
  SoDB::init();
  WRL* Vrml = new WRL();
  const string& my_filespec = "ml0.wrl";

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
  std::vector<int> curv;
  std::vector< std::vector<long double> > normal;
  std::vector< std::vector<long double> > alpha;
  std::vector<long double> vect1,vect2;
  
  using namespace mln;
  
  util::graph<point3d> g;

  binvox::Face *new_face_p;
  std::vector<binvox::Face *> faces;
  
  long double norm1,norm2,sum_alpha, sum_alpha_beta, sum_cot;
  long double tmp,beta;
  
  Lmailles* mailles = Vrml->obtMailles(Obj);
  new_face_p = new binvox::Face::Face();
  for(uint i=0; i< mailles->getNum();i++)
  {
    if ((*mailles)[i] != -1)
      new_face_p->add_vertex((*mailles)[i]);
    else
    {
      std::vector<long double> tmp_alpha(3);
      for(n=0; n<3;n++)
      {
        vect1.clear();
        vect2.clear();
        tmp = 0;
        
        (*ptsObj)[(*new_face_p)[n]].getValue(x0,y0,z0);
        (*ptsObj)[(*new_face_p)[NEXT(n)]].getValue(x1,y1,z1);
        (*ptsObj)[(*new_face_p)[PREV(n)]].getValue(x2,y2,z2);
        
        x0 = x0*10000;
        x1 = x1*10000;
        x2 = x2*10000;
        y0 = y0*10000;
        y1 = y1*10000;
        y2 = y2*10000;
        z0 = z0*10000;
        z1 = z1*10000;
        z2 = z2*10000;
        
        if(n==0)
        {
          long double tmpx = (y1-y0)*(z2-z0)-(z1-z0)*(y2-y0);
          long double tmpy = (z1-z0)*(x2-x0)-(x1-x0)*(z2-z0);
          long double tmpz = (x1-x0)*(z2-z0)-(x2-x0)*(y1-y0);
          long double normN = sqrt(tmpx*tmpx+tmpy*tmpy+tmpz*tmpz);
        
          std::vector<long double> tmpTab(3);
          tmpTab[0] = tmpx/normN;
          tmpTab[1] = tmpy/normN;
          tmpTab[2] = tmpz/normN;
          normal.push_back(tmpTab);
        }
        vect1.push_back(x1-x0);
        vect1.push_back(y1-y0);
        vect1.push_back(z1-z0);
          
        vect2.push_back(x2-x0);
        vect2.push_back(y2-y0);
        vect2.push_back(z2-z0);
          
        norm1 = sqrt(vect1[0]*vect1[0]+vect1[1]*vect1[1]+vect1[2]*vect1[2]);
        norm2 = sqrt(vect2[0]*vect2[0]+vect2[1]*vect2[1]+vect2[2]*vect2[2]);
          
        for(int k=0; k < 3; k++)
        {
          vect1[k]=vect1[k]/norm1;
          vect2[k]=vect2[k]/norm2;
          tmp += vect1[k] * vect2[k];
        }
          
        norm1 = sqrt(vect1[0]*vect1[0]+vect1[1]*vect1[1]+vect1[2]*vect1[2]);
        norm2 = sqrt(vect2[0]*vect2[0]+vect2[1]*vect2[1]+vect2[2]*vect2[2]);
        tmp_alpha[n] = acos(tmp/(norm1*norm2));
//         std::cout << "tmp_alpha: " << acos(tmp/(norm1*norm2)) << std::endl;

      }
      alpha.push_back(tmp_alpha);
      faces.push_back(new_face_p);
      new_face_p = new binvox::Face::Face();
    }
  }
  int nb = faces.size();
  
//   std::cout << "nb: " << nb << std::endl;
  
  for(int i=0; i< ptsObj->getNum();i++)
  {
    sum_alpha = 0;
    sum_alpha_beta = 0;
    sum_cot = 0;
    std::vector<bool> traite(faces.size(),false);


    for(int j=0; j < nb; j++)
    {
      n = (*faces[j]).has_vertex(i);
      tmp = 0;
      
      if(n != 0)
      {
        for(int k = 0; k < nb; k++)
        {
          if((!traite[k])&&(k!=j))
          {
            if(((*faces[k]).has_same_edge(*faces[j]))&&((*faces[k]).has_vertex(i)!=-1))
            {
              for(int p=0; p < 3; p++)
              {
                tmp += normal[j][p] * normal[k][p];
              }
              norm1 = sqrt(normal[k][0]*normal[k][0]+normal[k][1]*normal[k][1]+normal[k][2]*normal[k][2]);
              norm2 = sqrt(normal[j][0]*normal[j][0]+normal[j][1]*normal[j][1]+normal[j][2]*normal[j][2]);
              beta = acos(tmp/(norm1*norm2));
//               std::cout << "tmp: " << tmp << std::endl;
//               std::cout << "norm: " << (norm1*norm2) << std::endl;
              traite[k] = true;
              tmp = 0;
              break;
            }
          }
        }
//         std::cout << "alpha: " << alpha[j][n-1] << std::endl;

        sum_alpha_beta += (alpha[j][n-1])*beta;
        sum_alpha += (alpha[j][n-1]);
        sum_cot += (alpha[j][n-1]) * (alpha[j][n-1]) / tan((alpha[j][n-1])); 
      }
    }
//     tmp = (sum_alpha_beta)/( sum_alpha - 0.5 * sum_cot);
    tmp = (sum_alpha_beta)/( sum_alpha - 0.5 * sum_cot);
    curv.push_back(tmp*180/PI);
//     std::cout << "alpha_beta: " << sum_alpha_beta << std::endl;
//     std::cout << "tmp: " << tmp << std::endl;
//     std::cout << "curv: " << tmp*180/PI << std::endl;
  }


  double milieuX = 0;
  double milieuY = 0;
  double milieuZ = 0;
  double val = 0;
  std::vector<int> col;
  
  for(int i=0; i < nb; i++)
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
  
  SoMFInt32 * colorFace = Vrml -> coloriserFaces(curv);
  objet3D * object = Vrml->creerObjet( ptsObj,  mailles, colorFace,0);
//  objet3D * object = Vrml->creerObjet( ptsObj,  mailles, true);
  std::string nom("test.wrl");
  Vrml->ecrire(nom,  object);
//   std::cout << (Vrml->checkVRML(nom)) << std::endl;
  
  for(int i=0; i < nb; i++)
  {
    for(int j=0; j < nb; j++)
    {
      if(i!=j)
      {
        if((*faces[i]).has_same_edge(*faces[j]))
        {
  //         std::cout << i << " " << j << std::endl;
          g.add_edge(i,j);
//           t_pair p(i,j);
//           tmpEdge.push_back(p);
          edge_values.push_back(max(values[i], values[j]));
        }
      }
    }
  }

  


  typedef line_graph_image < point3d, int > gimage;

  p_line_graph<point3d> plg(g);
  line_graph_image<point3d, double> lgi(plg,values,edge_values);
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


  std::cout << "nb pt graph: " << lgi.domain().npoints() << std::endl;
  std::cout << "nb pt values: " << values.size() << std::endl;
  std::cout << "nb pt edge_values: " << edge_values.size() << std::endl;
  
  
  line_graph_elt_neighborhood<point3d> lgen;
  value::int_u16 nb_bassin;
  line_graph_image<point3d, value::int_u16> output;

  output = morpho::meyer_wst(lgi,lgen,nb_bassin);

  std::cout << "nombre de bassin: " << nb_bassin << std::endl;

//   Vrml->detruireObjet(Obj);
  delete ptsObj;
  delete mailles;

}
