#ifndef MLN_REGISTRATION_SAVE_HH
# define MLN_REGISTRATION_SAVE_HH

# include <mln/convert/to_image.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/save.hh>
# include <string>

# include "quat7.hh"
# include "tools.hh"

namespace mln
{

  namespace io
  {
    namespace pbm
    {
      template <typename P>
      void save(const p_array<P>& a, const std::string& filename)
      {
        mln_precondition(P::dim == 2);
        
        image2d out = convert::to_image(a);
        save(out, filename);
      }
    }
  }
  
  
  namespace registration
  {

    template<typename P>
    void save_(const quat7<3>& qk,
               const p_array<P>& c,
               const p_array<P>& x,
               int every)
    {
      static int id = 0;
      static int i = 0;

      if (++i % every != 0)
        return;
      
      //build ck: apply transform
      p_array<P> ck(c);
      qk.apply_on(c, ck, c.npoints());

      const box_<P> working_box = enlarge(bigger(ck.bbox(),x.bbox()),100);
      image2d<value::rgb8> out(convert::to_box2d(working_box), 1);
      level::fill(out, literal::white);
      
      //x in green
      for (unsigned i = 0; i < ck.npoints(); i++)
        {
          point2d p(ck[i][0], ck[i][1]);
          if (out.has(p))
            out(p) = literal::green;
        }

      //x in black
      for (unsigned i = 0; i < x.npoints(); i++)
        {
          point2d p(x[i][0], x[i][1]);
          if (out.has(p))
            out(p) = literal::black;
        }
      
      //save
      std::stringstream oss;
      oss << "step_" << id++ << ".ppm";
      io::ppm::save(out, oss.str());
    }
    
  }
  
}

#endif
