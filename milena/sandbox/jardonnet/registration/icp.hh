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

# include <mln/algebra/quat.hh>
# include <mln/algebra/vec.hh>
# include <mln/make/w_window.hh>
# include <mln/make/window3d.hh>

# include "tools.hh"

# include "cloud.hh"
# include "quat7.hh"
# include "projection.hh"
# include "chamfer.hh"

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
    mln_concrete(I)
    icp(const Image<I>& cloud,
        const Image<J>& surface);
    
# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename P, typename T1, typename T2>
      inline
      p_array<P>
      icp_(p_array<P>& C,
           const p_array<P>& X,
           std::pair<T1,T2>&)
      {
	trace::entering("registration::impl::icp_");

        unsigned int  k;
        quat7<P::dim> old_qk, qk;
        float         err;
        float         err_bis;
        p_array<P>    Ck(C), Xk(C); //FIXME: Xk copy C

        algebra::vec<P::dim,float> mu_C = center(C), mu_Xk;

        const float epsilon = 1e-2;

        //// step 1
        k = 0;
        do {   
          //// step 2
          //err_bis = projection::fill_Xk(Ck, map, Xk);
          //projection::de_base(Ck, X, Xk, err_bis);
          projection::memo(Ck, X, Xk, err_bis);

          mu_Xk = center(Xk);

          //// step 3
          old_qk = qk;
          qk = match(C, mu_C, Xk, mu_Xk);

          //// step 4
          qk.apply_on(C, Ck); // Ck+1 = qk(C)

          //// err = d(Ck+1,Xk)
          err = rms(Ck, Xk);
          std::cout << "error :" << err << std::endl;

          ++k;
        } while (k < 3 || (qk - old_qk).sqr_norm() > epsilon);

        std::cout << "nb of iterations : " << k << std::endl;
        trace::exiting("registration::impl::icp_");
        return Ck;
      }

    } // end of namespace mln::registration::impl


    //Only for 2d and 3d image
    template <typename I, typename J>
    inline
    mln_concrete(I)
    icp(Image<I>& cloud_,
        const Image<J>& surface_)
    {
      trace::entering("registration::icp");
      mln_precondition(exact(cloud_).has_data());
      mln_precondition(exact(surface_).has_data());

      //convert to image: time consuming
      typedef image3d<mln_value(I)> I3d;
      I3d cloud = convert::to_image_3d(exact(cloud_));
      const I3d surface = convert::to_image_3d(exact(surface_));

      /*
      //create a pair (distance map, closest point)
      bool w[27] = {true, true, true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true, true, true};
      window<mln_dpoint(I3d)> win3d = make::window3d(w);
      fun::cham<point3d> fun;
      w_window<mln_dpoint(I3d), float> chamfer = make::w_window(win3d, fun);
      */
      std::pair<mln_ch_value(I3d,float), mln_ch_value(I3d,mln_point(I3d)) > maps;// =
        //dt::chamfer(surface, chamfer);
      
      
      //build p_arrays.
      p_array<mln_point(I3d)> c = convert::to_p_array(cloud);
      p_array<mln_point(I3d)> x = convert::to_p_array(surface);

      p_array<mln_point(I3d)> res = impl::icp_(c, x, maps);

      //to 2d : projection (FIXME:if 3d)
      //mln_concrete(I) output = convert::to_image2d(res)?
      mln_concrete(I) output(exact(cloud_).domain());
      for (size_t i = 0; i < res.npoints(); i++)
        {
          point2d p(res[i][0], res[i][1]);
          //FIXME: not necessary if output(res.bbox())
          //if (output.has(p))
            output(p) = true;
        }

      trace::exiting("registration::icp");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
