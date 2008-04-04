#include <mln/core/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <sandbox/jardonnet/registration/icp_lazy.hh>

template <typename I>
class fun
{
  typedef mln_point(I) result;

  const operator () (mln_point(I)& p)
  {
    algebra::vec<P::dim,float> Cki = Ck[i];
    algebra::vec<P::dim,float> best_x = X[0];
    best_d = norm::l2(Cki - best_x);
    for (size_t j = 1; j < X.npoints(); ++j)
      {
        algebra::vec<P::dim,float> Xj = X[j];
        float d = norm::l2(Cki - Xj);
        if (d < best_d)
          {
            best_d = d;
            best_x = Xj;
          }
      }
    Xk.hook_()[i] = algebra::to_point<P>(best_x);
    map.map(Ck[i]) = Xk[i];
    map.known(Ck[i]) = true;
  }
}


int main(int, char* argv[])
{
  using namespace mln;
  
  image2d<bool> img1;
  image2d<bool> img2;
  
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);
  
  io::pbm::save(registration::icp(img1,img2), "./+registred.pbm");
}

