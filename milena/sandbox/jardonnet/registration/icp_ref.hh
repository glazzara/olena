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
# include <mln/literal/all.hh>
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
           const p_array<P>& X,
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

        p_array<P>    Ck(C);
        float d_k = 1000, d_k_1 = 1000, e_k = 1000;

        algebra::vec<P::dim,float> mu_C = center(C, c_length), mu_Xk;

        buf_qk.store(qk);

        save_(qk,C,X,map,2);
      
        qk.apply_on(C, Ck, c_length);

        save_(qk,C,X,map,2);

        unsigned int k = 0;
       
        do {
          //compute qk
          qk = match(C, mu_C, Ck, map, c_length);

          buf_qk.store(qk);
         
          //Ck+1 = qk(C)
          qk.apply_on(C, Ck, c_length);

          d_k_1 = d_k;
          
          // d_k = d(Yk, Pk+1)
          //     = d(closest(qk-1(P)), qk(P))
          d_k = rms(C, map, c_length, buf_qk[1], qk);

          // e_k = d(closest(qk-1(P)), qk-1(P))
          e_k = rms(C, map, c_length, buf_qk[1], buf_qk[1]);
          
#ifndef NDEBUG
          save_(qk,C,X,map,2);
          //print info
          std::cout << k << '\t' << (e_k >= d_k ? ' ' : '-') << '\t' << e_k << '\t' << d_k << '\t'
                    << ((qk - buf_qk[1]).sqr_norm() / qk.sqr_norm()) << '\t'
                    << std::endl;
          //count the number of points processed
          pts += c_length;
#endif
          k++;
        } while (d_k_1 - d_k > epsilon);
        // FIXME : test (e_k >= d_k) but seems to be a bad idea.

        trace::exiting("registration::impl::icp_");
      }

    } // end of namespace mln::registration::impl


    // Only for 3d images
    template <typename P, typename M>
    inline
    quat7<P::dim>
    icp(p_array<P> cloud, // FIXME : is almost const (shuffled)
        const M& map,
        const float q,
        const unsigned nb_it,
        const p_array<P>& x)
    {
      trace::entering("registration::icp");
      
      mln_precondition(P::dim == 3);
      mln_precondition(cloud.nsites() != 0);

      shuffle(cloud);

      //init rigid transform qk 
      quat7<P::dim> qk;

#ifndef NDEBUG       // FIXME: theo
      image2d<value::rgb8> tmp(500,800);
      level::fill(tmp, literal::black);
      //write X
      mln_piter(p_array<P>) p(x);
      for_all(p)
      {
        point2d qp = point2d(p[0], p[1]);
        if (tmp.has(qp))
          tmp(qp) = literal::white;
      }
#endif
     
      //run icp
      for (int e = nb_it-1; e >= 0; e--)
        {
          
          size_t l = cloud.nsites() / std::pow(q, e);
          l = (l<1) ? 1 : l;
          impl::icp_(cloud, map, qk, l, x, 1e-3);

#ifndef NDEBUG
          {
            value::rgb8 c;
            switch (e) {
            case 2: c = literal::green; break;
            case 1: c = literal::blue; break;
            case 0: c = literal::red; break;
            }
            mln_piter(p_array<P>) p(cloud);
            for_all(p)
            {
              algebra::vec<3,float> p_ = P(p), qp_ = qk(p_);
              point2d qp = point2d(int(qp_[0]), int(qp_[1]));
              if (tmp.has(qp))
                tmp(qp) = c;
            }
            if (e == 0)
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
