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

#ifndef MLN_REGISTRATION_REGISTRATION_HH
# define MLN_REGISTRATION_REGISTRATION_HH

/*! \file mln/registration/registration.hh
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
    registration(const Image<I>& cloud,
        const Image<J>& surface);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename P, typename M>
      inline
      void
      registration_(const p_array<P>& C,
           const M& map,
           quat7<P::dim>& qk,
           const unsigned c_length,
           const float  epsilon = 1e-3)
      {
	trace::entering("registration::impl::registration_");

        buffer<4,quat7<P::dim> > buf_qk;
        buffer<3,float>          buf_dk;

        float         d_k = 10000;
        p_array<P>    Ck(C);

        algebra::vec<P::dim,float> mu_C = center(C, c_length), mu_Xk;

        buf_qk.store(qk);

        qk.apply_on(C, Ck, c_length);

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

          k++;
        } while ((qk - buf_qk[1]).sqr_norm() / qk.sqr_norm() > epsilon);

        trace::exiting("registration::impl::registration_");
      }

    } // end of namespace mln::registration::impl


    // FIXME: Separate icp.hh registration.hh multiscale_registration.hh
    // FIXME: Make it works in 3d *AND* 2d
    template <typename P, typename M>
    inline
    quat7<P::dim>
    registration(p_array<P>& cloud,
                 const M& map,
                 const p_array<P>& x)
    {
      trace::entering("registration::registration");

      mln_precondition(P::dim == 3 || P::dim == 2);
      mln_precondition(cloud.nsites() != 0);

      //init rigid transform qk
      quat7<P::dim> qk;

      //run registration
      impl::registration_(cloud, map, qk, l, 1e-3);

      trace::exiting("registration::registration");

      return qk;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_REGISTRATION_HH
