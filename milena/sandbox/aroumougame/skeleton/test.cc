#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/core/p_image2d.hh>
#include <mln/core/p_set.hh>
#include <mln/core/neighb2d.hh>

int main()
{
  using namespace mln;
  using value::int_u8;
  int T;

  typedef int_u8 input_val_t;
  image2d<input_val_t> input;
  image2d<bool> pic = io::pbm::load("test.pbm");

  mln_fwd_piter_(image2d<bool>) p(pic.domain());
  p_set<point2d> X;
  p_set<point2d> traite;
  p_set<point2d> voisin;
  p_set<point2d> composant;
  for_all(p)
  {
    if(pic(p)==0)
    {
      X.insert(p.to_point());
    }
  }
  
  for(uint i=0; i<X.npoints(); i++)
  {
    std::cout << "echo 1 point " << X[i] << std::endl;
    traite.insert(X[i]);
    mln_niter_(neighb2d) q(c4(), X[i]);
    for_all(q)
    {
      if ((X.has(q.to_point()))&&(!traite.has(q.to_point())))
      {
        voisin.insert(q.to_point());
        std::cout << "point voisin " << q.to_point() << std::endl;
      }
    }
    
    if(voisin.npoints()<=1)
    {
      std::cout << X[i] <<": " << voisin.npoints() << std::endl;
      traite.clear();
      voisin.clear();
      continue;
    }
    else
      T=0;
    
    for(uint j=0; j < voisin.npoints(); j++)
    {
      std::cout << "echo 2 " << voisin.npoints() << std::endl;
      T++;
      traite.insert(voisin[j]);
      mln_niter_(neighb2d) t(c4(), voisin[j]);
      for_all(t)
      {
        if ((X.has(t.to_point()))&&(!traite.has(t.to_point())))
        {
          composant.insert(t.to_point());
          std::cout << "echo 3  insert composant" << t.to_point() << std::endl;
        }
      }
     
    for(uint k=0; k < composant.npoints(); k++)
    {
      traite.insert(composant[k]);
      if(voisin.has(composant[k]))
      {
        voisin.remove(composant[k]);
        std::cout << "echo 4  remove voisin" << composant[k] << std::endl;
      }
      
      mln_niter_(neighb2d) r(c4(), composant[k]);
      for_all(r)
      {
//         std::cout << r.to_point() << std::endl;
        if ((X.has(r.to_point()))&&(!traite.has(r.to_point()))&&(!composant.has(r.to_point())))
        {
          composant.insert(r.to_point());
          std::cout << "echo 5  insert composant'" << r.to_point() << std::endl;
        }
      }
    }
    composant.clear();
    
    }
    voisin.clear();
    traite.clear();
    std::cout << X[i] <<": " << T << std::endl;
  }












// io::pgm::load(input, "test1.pgm");

//   mln_fwd_piter_(image2d<input_val_t>) p(input.domain());
// 
//   for_all(p)
//       std::cout << input(p) << std::endl;
// 



 /* p_image2d<point2d> ps(20,20);


  mln_fwd_piter_(box2d) p(inplace(make::box2d(2,2,13,13)));
  for_all(p)
  {
    ps.insert(p);
  }*/

//   for(int i = 2; i< 14; i++)
//   {
//     for(int j = 2; j< 14; j++)
//     {
//       ps.insert(make::point2d(i, j));
//     }
//   }
/*  input = ps.image();
  io::pbm::save(input, "out.pbm");*/

}

