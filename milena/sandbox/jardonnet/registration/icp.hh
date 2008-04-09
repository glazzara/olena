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

# include "tools.hh"

# include "cloud.hh"
# include "quat7.hh"
# include "projection.hh"
# include "chamfer.hh"

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
      p_array<P>
      icp_(const p_array<P>& C,
           M& map,
           quat7<P::dim>& qk,
           const size_t c_length,
           const float  epsilon = 1e-3)
      {
	trace::entering("registration::impl::icp_");

#ifndef NDEBUG
        //display registred points
        std::cout << "Register "
                  << c_length << " points" << std::endl;
        std::cout << "k\terror\tdqk" << std::endl;
#endif
        quat7<P::dim> old_qk;
        float         err;
        //float       err_bis;
        p_array<P>    Ck(C), Xk(C); //FIXME: Xk copy C

        algebra::vec<P::dim,float> mu_C = center(C, c_length), mu_Xk;

        qk.apply_on(C, Ck, c_length);

        unsigned int k = 0;

        do {
          //compute qk
          old_qk = qk;
          qk = match(C, mu_C, Ck, map, c_length);
          
          //Ck+1 = qk(C)
          qk.apply_on(C, Ck, c_length);
          
          //err = d(Ck+1,Xk)
          err = rms(Ck, Xk, c_length);

#ifndef NDEBUG
          {
            using namespace std;
            
            image2d<bool> img(box2d(500,800), 0);
            for (size_t i = 0; i < c_length; i++)
              {
                point2d p = convert::to_point2d(Ck[i]);
                if (img.has(p))
                  img(p) = true;
              }
            
            //image2d<bool> img = convert::to_image2d(Ck);
            stringstream oss;
            static int pimp = 0;
            oss << "i_" << pimp++ << ".pbm";
            io::pbm::save(img, oss.str());
          }
          
          //plot file
          std::cout << k << '\t' << err << '\t'
                    << (qk - old_qk).sqr_norm() << '\t'
                    << std::endl;
          //count the number of points processed
          pts += c_length;
#endif

          k++;
        } while (k < 3 || (qk - old_qk).sqr_norm() > epsilon);

        trace::exiting("registration::impl::icp_");
        return Ck;
      }

    } // end of namespace mln::registration::impl


    // Only for 3d images
    template <typename P, typename M>
    inline
    quat7<P::dim>
    icp(p_array<P> cloud,
        M& map,
        const unsigned nb_it)
    {
      trace::entering("registration::icp");
      
      mln_precondition(P::dim == 3);
      mln_precondition(cloud.npoints() != 0);

      // Shuffle cloud
      for (size_t i = 0; i < cloud.npoints(); i++)
        {
          size_t r = rand() % cloud.npoints();
          P tmp;
          tmp = cloud[i];
          cloud.hook_()[i] = cloud[r];
          cloud.hook_()[r] = tmp;
        }
      
      //init rigid transform qk
      quat7<P::dim> qk;
      
      //run icp
      for (int i = nb_it; i >= 0; i--)
        {
          size_t l = cloud.npoints() / std::pow(2., i); // 3 is arbitrary fixed
          impl::icp_(cloud, map, qk, l, i + 1e-3);
        }

      trace::exiting("registration::icp");
      
      return qk;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_ICP_HH
