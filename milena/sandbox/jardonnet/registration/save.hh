#ifndef MLN_REGISTRATION_SAVE_HH
# define MLN_REGISTRATION_SAVE_HH

# include <mln/convert/to_image.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/save.hh>
# include <mln/draw/all.hh>
# include <string>

# include "quat7.hh"
# include "tools.hh"
# include "power_it.hh"

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
        
        image2d<bool> out = convert::to_image(a);
        save(out, filename);
      }
    }
  }
  
  
  namespace registration
  {

    template<typename P, typename M>
    void save_(const quat7<3>& qk,
               const p_array<P>& c,
               const p_array<P>& x,
               const M& map,
               int every)
    {
      static int id = 0;
      static int i = 0;

      if (++i % every != 0)
        return;

      //build ck: apply transform
      p_array<P> ck(c);
      qk.apply_on(c, ck, c.npoints());

      const box_<P> working_box = enlarge(bigger(ck.bbox(),x.bbox()),5);
      image2d<value::rgb8> out(convert::to_box2d(working_box), 1);
      level::fill(out, literal::white);

      //plot mu_Ck
      algebra::vec<P::dim,float> mu_Ck = center(ck, ck.npoints());
      draw::plot(out, point2d(mu_Ck[0], mu_Ck[1]), literal::green);
      
      //Ck orientation
      algebra::mat<P::dim,P::dim,float> Mk(literal::zero);
      for (unsigned i = 0; i < ck.npoints(); ++i)
        {
          algebra::vec<P::dim,float> Cki  = ck[i];
          Mk += make::mat(Cki - mu_Ck) * trans(make::mat(Cki - mu_Ck));
        }
      Mk /= c.npoints();
      algebra::vec<3,float> vck = power_it(Mk);
      draw::line(out, point2d(mu_Ck[0], mu_Ck[1]),
                 point2d(mu_Ck[0]+vck[0]*10, mu_Ck[1]+vck[1]*10),
                 literal::red);

      //build xk : project ck
      p_array<P> xk;
      for (unsigned i = 0; i < ck.npoints(); ++i)
        xk.append(map(ck[i]));
      
      //plot mu_Xk
      algebra::vec<P::dim,float> mu_Xk = center(xk, xk.npoints());
      draw::plot(out, point2d(mu_Xk[0], mu_Xk[1]), literal::blue);

      //Xk orientation
      algebra::mat<P::dim,P::dim,float> Mxk(literal::zero);
      for (unsigned i = 0; i < xk.npoints(); ++i)
        {
          algebra::vec<P::dim,float> Xki  = xk[i];
          Mxk += make::mat(Xki - mu_Xk) * trans(make::mat(Xki - mu_Xk));
        }
      Mxk /= c.npoints();
      algebra::vec<3,float> vxk = power_it(Mxk);
      draw::line(out, point2d(mu_Xk[0], mu_Xk[1]),
                 point2d(mu_Xk[0]+vxk[0]*10, mu_Xk[1]+vxk[1]*10),
                 literal::red);
      
      
      //ck in green
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
      
      //xk in blue
      for (unsigned i = 0; i < xk.npoints(); i++)
        {
          point2d p(xk[i][0], xk[i][1]);
          if (out.has(p))
            out(p) = literal::red;
        }  

      //save
      std::stringstream oss;
      oss << "step_" << id++ << ".ppm";
      io::ppm::save(out, oss.str());
    }
    
  }
  
}

#endif
