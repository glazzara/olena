#include "thinning.hh"
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/data/paste.hh>
#include <mln/data/fill.hh>

int main()
{
  std::cout << "start" << std::endl;
  image2d<bool> pic = io::pbm::load("vaseline.pbm");
  mln_fwd_piter_(image2d<bool>) p(pic.domain());
  std::cout << "image read -> create X" << std::endl;
  
  p_set<point2d> X;
  p_set<point2d> X_complement;
  
  
  for_all(p)
  {
    if(pic(p)==1)
    {
      X.insert(p);
    }
    else
    {
      X_complement.insert(p);
    }
  }
  
  std::cout << "X created -> thinning" << std::endl;
  
  X = thinning(X,c8(),X_complement,2000,true);
  
  std::cout << "thinning done -> save image" << std::endl;
  typedef mln::image2d<bool> I;
  typedef mln::p_set<point2d> S;
  
  sub_image<I, S> skeleton = pic | X;
  I output(pic.domain());
  data::fill(output, false);

  data::paste(skeleton, output);

  mln::io::pbm::save(output, "skeleton2.pbm");
  std::cout << "image saved" << std::endl;

}

