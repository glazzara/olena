// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_REGISTRATION_ICP_HH
# define MLN_REGISTRATION_ICP_HH

/*! \file mln/registration/icp.hh
 *
 * \brief image registration
 */

# include <iostream>
# include <string>
# include <cmath>

# include <mln/algebra/quat.hh>
# include <mln/algebra/vec.hh>
# include <mln/make/w_window.hh>
# include <mln/make/w_window3d.hh>

# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/literal/black.hh>
# include <mln/level/fill.hh>
# include <mln/io/ppm/save.hh>


# include "tools.hh"

# include "cloud.hh"
# include "quat7.hh"
# include "update_qk.hh"
# include "chamfer.hh"

# include "save.hh"

namespace mln
{
  
  namespace registration
  {

#ifndef NDEBUG
    static unsigned pts = 0;
#endif
    
    /*! Registration FIXME : doxy
     *
     *
     */
    template <typename I, typename J>
    inline
    mln_concrete(I)
    icp(const Image<I>& cloud,
        const Image<J>& surface);
    
# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename P, typename M>
      inline
      void
      icp_(const p_array<P>& C,
           const M& map,
           quat7<P::dim>& qk,
           const size_t c_length,
           const float  epsilon = 1e-3)
      {
	trace::entering("registration::impl::icp_");

#ifndef NDEBUG
        //display registred points
        std::cout << "Register "
                  << c_length << " points" << std::endl;
        std::cout << "k\t\te_k >=\td_k\tdqk" << std::endl;
#endif

        buffer<4,quat7<P::dim> > buf_qk;
        buffer<3,float>          buf_dk;

        float         d_k = 10000;
        p_array<P>    Ck(C);

        algebra::vec<P::dim,float> mu_C = center(C, c_length), mu_Xk;

        buf_qk.store(qk);

        qk.apply_on(C, Ck, c_length);

        //buf_dk[0] = rms(C, map, c_length, buf_qk[1], qk);

        unsigned int k = 0;

        do {
          buf_dk.store(d_k);

          //compute qk
          qk = match(C, mu_C, Ck, map, c_length);

          buf_qk.store(qk);

          //update qk
          if (k > 2)
            qk = update_qk(buf_qk.get_array(), buf_dk.get_array());
          qk._qR.set_unit();
          buf_qk[0] = qk;
          
          //Ck+1 = qk(C)
          qk.apply_on(C, Ck, c_length);

          // d_k = d(Yk, Pk+1)
          //     = d(closest(qk-1(P)), qk(P))
          d_k = rms(C, map, c_length, buf_qk[1], qk);

#ifndef NDEBUG         
          // e_k = d(closest(qk-1(P)), qk-1(P))
          float e_k = rms(C, map, c_length, buf_qk[1], buf_qk[1]);

          //plot file
          std::cout << k << '\t' << (e_k >= d_k ? ' ' : '-')
                    << '\t' << e_k << '\t' << d_k << '\t'
                    << ((qk - buf_qk[1]).sqr_norm() / qk.sqr_norm()) << '\t'
                    << std::endl;
          //count the number of points processed
          pts += c_length;
#endif
          k++;
        } while ((qk - buf_qk[1]).sqr_norm() / qk.sqr_norm() > epsilon);

        trace::exiting("registration::impl::icp_");
      }

    } // end of namespace mln::registration::impl


    // Only for 3d images
    template <typename P, typename M>
    inline
    quat7<P::dim>
    icp(p_array<P> cloud, // More efficient without reference
        const M& map,
        const float q,
        const unsigned nb_it,
        const p_array<P>& x)
    {
      trace::entering("registration::icp");
      
      mln_precondition(P::dim == 3);
      mln_precondition(cloud.nsites() != 0);

      // Shuffle cloud
      shuffle(cloud);
            
      //init rigid transform qk
      quat7<P::dim> qk;


#ifndef NDEBUG       // FIXME: theo
      const box_<P> working_box = enlarge(bigger(cloud.bbox(),x.bbox()),5);
      image2d<value::rgb8> tmp(convert::to_box2d(working_box), 1);
      level::fill(tmp, literal::black);
      //write X
      mln_piter(p_array<P>) p(x);
      for_all(p)
      {
        point2d qp = point2d(p[0], p[1]);
        if (tmp.has(qp))
          tmp(qp) = literal::green;
      }
#endif

      
      //run icp
      for (int e = nb_it-1; e >= 0; e--)
        {
          
          size_t l = cloud.nsites() / std::pow(q, e);
          l = (l<1) ? 1 : l;
          impl::icp_(cloud, map, qk, l, 1e-3);

          /*
          //remove points
          p_array<P> tmp;
          tmp.reserve(cloud.nsites());
          qk.apply_on(cloud,tmp, cloud.nsites());
          float stddev, mean;
          mean_stddev(tmp, map, mean, stddev);
          for (unsigned i = 0; i < cloud.nsites(); i++)
          {
            algebra::vec<3,float> qci = tmp[i];
            algebra::vec<3,float> xi  = x[i];
            if (norm::l2(qci - xi) < 2 * stddev)
              cloud.hook_()[i] = cloud[(i+1) % cloud.nsites()];
          }
          */
#ifndef NDEBUG
          {
            value::rgb8 c;
            switch (e) {
            case 2: c = literal::white; break;
            case 1: c = literal::blue; break;
            case 0: c = literal::red; break;
            }
            mln_piter(p_array<P>) p(cloud);
            for_all(p)
            {
              algebra::vec<3,float> p_ = point3d(p), qp_ = qk(p_);
              point2d qp = point2d(int(qp_[0]), int(qp_[1]));
              if (tmp.has(qp))
                tmp(qp) = c;
            }
            io::ppm::save(tmp, "tmp.ppm");
          }
#endif 
        }

      trace::exiting("registration::icp");
      
      return qk;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
