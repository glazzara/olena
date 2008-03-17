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

# include <mln/value/quat.hh>
# include <mln/algebra/vec.hh>


typedef mln::algebra::vec<3, float> vec3f;
typedef std::vector< vec3f > vecs_t;
  
#include "cloud.hh"
#include "quat7.hh"
#include "projection.hh"

namespace mln
{
  
  namespace registration
  {

    /*! Registration FIXME : doxy
     *
     *
     */
    template <typename I, typename J>
    inline
    void
    icp(const Image<I>& cloud,
        const Image<J>& surface);
    
# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      inline
      void
      icp_(const vecs_t& P,
           const vecs_t& X)
      {
	trace::entering("registration::impl::icp_");

        unsigned int k;
        quat7 old_qk, qk;
        float err, err_bis;

        vecs_t Pk(P.size()), Xk(Pk.size());
        vec3f mu_P = center(P), mu_Xk;

        const float epsilon = 1e-3;

        //step 1
        k = 0;
        Pk = P;

        do {
          //step 2 FIXME : etienne
          projection::de_base(Pk, X, Xk, mu_Xk, err_bis);
          
          // step 3
          old_qk = qk;
          qk = match(P, mu_P, Xk, mu_Xk);

          // step 4
          qk.apply_on(P, Pk); // Pk+1 = qk(P)

          // err = d(Pk+1,Xk)
          err = rms(Pk, Xk);

          ++k;
        } while (k < 3 || (qk - old_qk).sqr_norm() > epsilon);

	trace::exiting("registration::impl::icp_");
      }

    } // end of namespace mln::registration::impl


    // this version could convert image cloud in a vector of point?
    template <typename I, typename J>
    inline
    void
    icp(const Image<I>& cloud,
        const Image<J>& surface)
    {
      trace::entering("registration::icp");
      mln_precondition(exact(cloud).has_data());
      mln_precondition(exact(surface).has_data());

      vecs_t a,b; // FIXME : to built.

      impl::icp_(a, b);

      trace::exiting("registration::icp");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
