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

/// \file mln/registration/registration.hh
///
/// image registration

# include <mln/registration/icp.hh>
# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2p/closest_point.hh>
# include <mln/core/image/lazy_image.hh>
# include <mln/convert/to_p_array.hh>

namespace mln
{

  namespace registration
  {

    using namespace mln::fun::x2x;

    /// Register an image \p cloud over the image \p surface.
    template <typename I, typename J>
    inline
    composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> >
    registration(const Image<I>& cloud,
                 const Image<J>& surface);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      inline
      composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> >
      registration_(const I& cloud,
                    const J& surface)
      {
        //FIXME: Use convert::to< p_array<mln_psite(I)> >()
        p_array<mln_psite(I)> c = convert::to< p_array<mln_psite(I)> >(cloud);
        p_array<mln_psite(J)> x = convert::to< p_array<mln_psite(J)> >(surface);

        //init rigid transform qk
        composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> > qk;

        //working box
        const box<mln_psite(I)> working_box =
          larger_than(geom::bbox(c), geom::bbox(x)).to_larger(100);

        //make a lazy_image map via function closest_point
        fun::x2p::closest_point<mln_psite(I)> fun(x, working_box);
        lazy_image< I, fun::x2p::closest_point<mln_psite(I)>, box<mln_psite(I)> >
          map(fun, fun.domain());

        //run registration
        return registration::icp(c, map, 1e-3);

      }

    }

    template <typename I, typename J>
    inline
    composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> >
    registration(const Image<I>& cloud,
                 const Image<J>& surface)
    {
      trace::entering("registration::registration");

      mln_precondition(I::psite::dim == J::psite::dim);
      mln_precondition(I::psite::dim == 3 || J::psite::dim == 2);

      composed< rotation<I::psite::dim, float>, translation<I::psite::dim, float> >
        qk = impl::registration_(exact(cloud), exact(surface));

      trace::exiting("registration::registration");

      return qk;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::registration


} // end of namespace mln


#endif // ! MLN_REGISTRATION_REGISTRATION_HH
