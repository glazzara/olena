#include "thinning.hh"

#include <mln/value/rgb8.hh>

#include <mln/core/point3d.hh>
#include <mln/make/point3d.hh>

#include <mln/util/graph.hh>
#include <mln/core/image/line_graph_image.hh>
#include <mln/core/image/graph_image.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>


int main()
{
  std::vector<bool> vertex_bool_values(false);
  
  p_set< mln::point3d > X;
  p_set< mln::point3d > X_complement;
  
  int nb= 5;
  int centre = 2;
  for(int i=0; i<nb; i++)
  {
    for(int j=0; j<nb; j++)
    {
      X_complement.insert(mln::make::point3d(0, i, j));
      X_complement.insert(mln::make::point3d(i, j, 0));
      X_complement.insert(mln::make::point3d(i, 0, j));
      X_complement.insert(mln::make::point3d(i, nb-1, j));
      X_complement.insert(mln::make::point3d(i, j, nb-1));
      X_complement.insert(mln::make::point3d(nb-1, i, j));
    }
  }
  for(int i=1; i<nb-1; i++)
  {
    for(int j=1; j<nb-1; j++)
    {
      for(int k=1; k<nb-1; k++)
      {
        if((i!=centre) || (j!=centre) || (k!=centre))
          X.insert(mln::make::point3d(i, j, k));
      } 
    }
  }
  
  for(int i=0; i<nb; i++)
  {
    for(int j=0; j<nb; j++)
    {
      X_complement.insert(mln::make::point3d(0+5, i, j));
      X_complement.insert(mln::make::point3d(i+5, j, 0));
      X_complement.insert(mln::make::point3d(i+5, 0, j));
      X_complement.insert(mln::make::point3d(i+5, nb-1, j));
      X_complement.insert(mln::make::point3d(5+i, j, nb-1));
      X_complement.insert(mln::make::point3d(nb-1+5, i, j));
    }
  }
  for(int i=1; i<nb-1; i++)
  {
    for(int j=1; j<nb-1; j++)
    {
      for(int k=1; k<nb-1; k++)
      {
        if((i!=centre) || (j!=centre) || (k!=centre))
          X.insert(mln::make::point3d(i+5, j, k));
      } 
    }
  }
  
  X_complement.insert(mln::make::point3d(centre, centre, centre));
  X_complement.insert(mln::make::point3d(centre+5, centre, centre));
  
  std::cout << "X "<< X.npoints() << std::endl;
  std::cout << "X_complement "<< X_complement.npoints() << std::endl;
  
  X = thinning(X,c6(),X_complement,2000,false);
  
  for(uint i=0; i<X.npoints(); i++)
  {
    std::cout << X[i] << std::endl;
  }
}
