// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_REGISTRATION_GET_RTRANSF_HH
# define MLN_REGISTRATION_GET_RTRANSF_HH

# include <mln/core/site_set/p_array.hh>
# include <mln/fun/x2x/all.hh>
# include <mln/algebra/quat.hh>
# include <mln/algebra/vec.hh>
# include <mln/math/jacobi.hh>
# include <mln/registration/get_rot.hh>

namespace mln
{

  namespace registration
  {

    using namespace fun::x2x;

    template <typename P, typename M>
    composed<rotation<P::dim, float>, translation<P::dim, float> >
    get_rtransf(const p_array<P>& C,
                const algebra::vec<P::dim,float>& mu_C,
                const p_array<P>& ck,
                const M& map);


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename M>
    composed<rotation<P::dim, float>, translation<P::dim, float> >
    get_rtransf(const p_array<P>& c,
                const algebra::vec<P::dim,float>& mu_c,
                const p_array<P>& ck,
                const M& map)
    {
      //mu_xk = center map(Ck)
      algebra::vec<P::dim,float> mu_xk(literal::zero);

      std::cout << c.nsites() << std::endl;
      for (unsigned i = 0; i < c.nsites(); ++i)
      {
        if (not map.has(ck[i]))
        {
          std::cout << ck[i] << "  " << i << std::endl;
        }
        else
          mu_xk += convert::to< algebra::vec<P::dim,float> >
            (map(convert::to< algebra::vec<P::dim,float> >(ck[i])));

      }
      mu_xk /= c.nsites();

      // qR
      rotation<P::dim, float> tqR = get_rot(c, mu_c, ck, map, mu_xk);

      // qT
      const algebra::vec<P::dim,float> qT = mu_xk - tqR(mu_c);
      translation<P::dim, float> tqT(qT);
      return compose(tqR,tqT);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace registration

} // end of namespace mln

#endif // ! MLN_REGISTRATION_GET_RTRANSF_HH
