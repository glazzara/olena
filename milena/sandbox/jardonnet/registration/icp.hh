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
# include <mln/core/w_window.hh>

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
    I
    icp(const Image<I>& cloud,
        const Image<J>& surface,
        I& r);
    
# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename P, typename T1, typename T2>
      inline
      void
      icp_(const p_array<P>& C,
           const p_array<P>& X,
           std::pair<T1,T2>&,
           p_array<P>& Ck)
      {
	trace::entering("registration::impl::icp_");

        unsigned int k;
        quat7<P::dim> old_qk, qk;
        float err, err_bis;

        assert(Ck.npoints() == C.npoints());
        p_array<P> Xk(C); //FIXME:is it correct?
        ///        Ck.reserve(C.npoints());
        //Xk.reserve(C.npoints());
        //assert(C.npoints() != 0);
        
        algebra::vec<P::dim,float> mu_C = center(C), mu_Xk;

        const float epsilon = 1e-3;

        //step 1
        k = 0;
        Ck = C;

        do {
          std::cout << "step2" << std::endl;
          //step 2 FIXME : etienne
          projection::de_base(Ck, X, Xk, err_bis);

          std::cout << "step2.1 center" << std::endl;
          mu_Xk = center(Xk);

          std::cout << "step3" << std::endl;
          // step 3
          old_qk = qk;
          qk = match(C, mu_C, Xk, mu_Xk);

          std::cout << "step4" << std::endl;
          // step 4
          qk.apply_on(C, Ck); // Ck+1 = qk(C)

          // err = d(Ck+1,Xk)
          err = rms(Ck, Xk);

          ++k;
          std::cout << err << std::endl;
        } while (k < 3 || (qk - old_qk).sqr_norm() > epsilon);

	trace::exiting("registration::impl::icp_");
      }

    } // end of namespace mln::registration::impl


    //FIXME: move?
    namespace convert
    {
      template <typename T>
      const image3d<T>&
      to_image_3d(const image3d<T>& img)
      {
        return img;
      }
      
      template <typename T>
      image3d<T>&
      to_image_3d(image3d<T>& img)
      {
        return img;
      }
    
      template <typename T>
      image3d<T>
      to_image_3d(const image2d<T>& img)
      {
        point3d pmin(img.domain().pmin()[0], img.domain().pmin()[1], -10);
        point3d pmax(img.domain().pmax()[0], img.domain().pmax()[1], 10);
        image3d<T> img3d(box3d(pmin,pmax));
        
        mln_piter(image3d<T>) p(img3d.domain());
        for_all(p)
        {
          if (p[2] == 0)
            img3d(p) = exact(img)(point2d(p[0],p[1]));
        }
        return img3d;
      }
    }
    

    //Only for 2d and 3d image
    template <typename I, typename J>
    inline
    I
    icp(Image<I>& cloud_,
        const Image<J>& surface_,
        I& r)
    {
      trace::entering("registration::icp");
      mln_precondition(exact(cloud_).has_data());
      mln_precondition(exact(surface_).has_data());

      std::cout << "convert to image3d" << std::endl;
      //convert to image: time consuming
      typedef image3d<mln_value(I)> I3d;
      I3d cloud = convert::to_image_3d(exact(cloud_));
      const I3d surface = convert::to_image_3d(exact(surface_));
      
      
      std::cout << "chamfer" << std::endl;
      /*
      //FIXME: not a chamfer. etienne?
      //create a pair (distance map, closest point)
      w_window<mln_dpoint(I3d), float> chamfer;
      */
      std::pair<mln_ch_value(I3d,float), mln_ch_value(I3d,mln_point(I3d)) > maps;
      /*
        dt::chamfer(surface, chamfer);
      */
      
      std::cout << "Build p_array" << std::endl;
      //build p_arrays.
      p_array<mln_point(I3d)> c,x;

      mln_piter(I3d) p1(cloud.domain());
      for_all(p1)
        if (exact(cloud)(p1))
          c.append(p1);
      
      mln_piter(I3d) p2(surface.domain());
      for_all(p2)
        if (exact(surface)(p2))
          x.append(p2);

      std::cout << "Start ICP" << std::endl;
      
      p_array<mln_point(I3d)> res(c);
      impl::icp_(c, x, maps, res);

      //to 2d
      for (unsigned e; e < res.npoints(); e++)
        {
          point2d p(res[e][0], res[e][1]);
          r(p) = true;
        }

      trace::exiting("registration::icp");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
