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

#ifndef MLN_REGISTRATION_MULTISCALE_HH
# define MLN_REGISTRATION_MULTISCALE_HH

# include <mln/registration/icp.hh>
# include <mln/fun/x2p/closest_point.hh>

namespace mln
{

  namespace registration
  {

    template <typename I, typename J>
    inline
    composed< rotation<I::psite::dim, float>, translation<I::Psite::dim, float> >
    multiscale(const Image<I>& cloud,
               const Image<J>& surface,
               const float q,
               const unsigned nb_it);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      inline
      composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
      multiscale_(const I& cloud,
                  const J& surface,
                  const float q,
                  const unsigned nb_it)
      {
        p_array<mln_psite(I)> c = convert::to< p_array<mln_psite(I)> >(cloud);
        p_array<mln_psite(J)> x = convert::to< p_array<mln_psite(I)> >(surface);

        // Shuffle cloud
        shuffle(c);

        //working box
        const box<mln_psite(I)> working_box =
          enlarge(bigger(geom::bbox(c), geom::bbox(x)), 100);

        //make a lazy_image map via function closest_point
        fun::x2p::closest_point<mln_psite(I)> fun(x, working_box);
        lazy_image<I, fun::x2p::closest_point<mln_psite(I)>, box2d >
          map(fun, fun.domain());

        //init rigid transform qk
        composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> > qk;

        //run registration
        for (int e = nb_it-1; e >= 0; e--)
          {
            unsigned l = cloud.nsites() / std::pow(q, e);
            l = (l<1) ? 1 : l;
            impl::registration_(cloud, map, qk, l, 1e-3);
          }
        return qk;
      }

    }

    template <typename I, typename J>
    inline
    composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
    multiscale(const Image<I>& cloud,
               const Image<J>& surface,
               const float q,
               const unsigned nb_it)
    {
      trace::entering("registration::registration");

      mln_precondition(I::site::dim == 3 || I::site::dim == 2);

      composed< rotation<I::site::dim, float>, translation<I::site::dim, float> >
        qk = impl::multiscale_(exact(cloud), exact(surface), q, nb_it);

      trace::exiting("registration::registration");

      return qk;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_MULTISCALE_HH
