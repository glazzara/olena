#include <mln/core/alias/point3d.hh>
#include <mln/core/alias/point2d.hh>
#include "thinning.hh"
#include <mln/data/paste.hh>
#include <mln/data/fill.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pbm/load.hh>


int main(int argc, char* argv[])
{
  if(argc!=5)
  {
    std::cout << "arguments: filename voisinage R alpha" << std::endl;
    exit(1);
  }
  image2d<bool> output;
  std::string filename = argv[1];
  int r = atoi(argv[3]);
  int alpha = atoi(argv[4]);
  
  image2d<bool> pic = io::pbm::load(filename);
  if(atoi(argv[2])==4)
    output = filteredSkeleton( pic, c4(), r, alpha, true);
  else
    output = filteredSkeleton( pic, c8(), r, alpha, true);
  mln::io::pbm::save(output, "FS-"+std::string(argv[2])+"_"+std::string(argv[3])+"_"+std::string(argv[4])+"_"+filename);
  
//   mln_fwd_piter_(image2d<bool>) p(pic.domain());
//   
//   p_set<point2d> X,Y;
//   p_set<point2d> X_complement;
//   
//   
//   for_all(p)
//   {
//     if(pic(p)==1)
//     {
//       X.insert(p);
//     }
//     else
//     {
//       X_complement.insert(p);
//     }
//   }
//   
//   int w = geom::ncols(pic);
//   int h = geom::nrows(pic);
//   
//   int l= math::min(w, h);
//   image2d<value::int_u8> pic1,dt,bis;
//   pic1 = intImage(pic);
//   dt = sedt(pic1);
//   uint rmax = getRMax(dt);
//   
//   image2d<value::int_u8> DTg(l,l,0);
//   std::vector< std::vector<int> > Mgl;
//   std::vector< std::vector<int> > Lut;
//   
//   Mgl = CompLutMask (DTg,Mgl,Lut,l,0,rmax);
//   
//   image2d<bool> pict;
//   pict = MA(pic, Mgl, dt, Lut);
//   
//   for_all(p)
//   {
//     if(pict(p)==1)
//     {
//       Y.insert(p);
//     }
//   }
//   
//   typedef mln::image2d<bool> I;
//   typedef mln::p_set<point2d> S;
//   
//   X = euclideanSkeleton(X, X_complement, dt, Y, c4(), true);
//   
/*  sub_image<I, S> skeleton = pic | X;
  I output(pic.domain());
  data::fill(output, false);

  data::paste(skeleton, output);*/
  
//   bis = DiscreteBisector(dt, Y, c4(), rmax);
//   
//   for_all(p)
//   {
//       std::cout << p << " val " << bis(p) << std::endl;
//   }
//   
//   std::string pgm = filename;
//   pgm[pgm.length()-2]='g';
//   io::pgm::save(bis,"DB-"+pgm);

/*  mln::io::pbm::save(pict, "MA-"+filename);
  mln::io::pbm::save(output, "ES-"+filename);*/
  
}
