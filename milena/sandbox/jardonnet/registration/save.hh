#ifndef MLN_REGISTRATION_SAVE_HH
# define MLN_REGISTRATION_SAVE_HH

#include <string>

namespace mln
{

  namespace registration
  {

    template <typename P>
    void save_(const p_array<P>& Ck, int l)
    {
      static int id = 0;
      
      using namespace std;
      
      //FIXME: Should use Ck box but Ck.bbox() is not correct for c_length
      image2d<bool> img(box2d(500,800), 0);
      for (size_t i = 0; i < l; i++)
        {
          point2d p = convert::to_point2d(Ck[i]);
          if (img.has(p))
            img(p) = true;
        }
      
      //FIXME: Good but put point after c_length
      //image2d<bool> img = convert::to_image2d(Ck);
      
      std::stringstream oss;
      oss << "i_" << id++ << ".pbm";
      io::pbm::save(img, oss.str());
      
    }
    
  }
  
}

#endif
