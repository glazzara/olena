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

/*! \file mln/binarization/threshold.hh
 *
 * \brief Produce a subsampled image
 */

# include <mln/value/quat.hh>
# include <mln/algebra/vec.hh>

namespace mln
{

  namespace registration
  {    

    /*! Registration FIXME : doxy
     *
     *
     */
    template <typename I, template J>
    inline
    void
    icp(const Image<I>& cloud,
        const Image<J>& surface);
    
# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      inline
      void
      icp_(const I& P,
           const J& X)
      {
	trace::entering("registration::impl::icp_");

        mln_concrete(I) Pk(cloud.domain());

        const float epsilon = 1e-3;
        float err, err_bis;
        quat old_qk, qk;
        unsigned int k;

        k = 0;
        Pk = P;
        do {
          //projection
          
          old_qk = qk;
          //qk = match(P, mu_P, Xk, mu_Xk);

          // error =
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


      
      output = impl::icp_(exact(cloud), exact(surface));

      trace::exiting("registration::icp");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
