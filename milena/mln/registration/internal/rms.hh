// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_REGISTRATION_INTERNAL_RMS_HH
# define MLN_REGISTRATION_INTERNAL_RMS_HH

# include <mln/norm/l2.hh>
# include <mln/core/site_set/p_array.hh>

namespace mln
{

  namespace registration
  {

    namespace internal
    {

      template <typename P, typename M, typename T>
      float rms(const p_array<P>& a1,
                M& map,
                const unsigned length,
                const T& q1,
                const T& q2);

# ifndef MLN_INCLUDE_ONLY


      template <typename P, typename M, typename T>
      float rms(const p_array<P>& a,
                M& map,
                const T& q1,
                const T& q2)
    {
      float f = 0.f;
      for (unsigned i = 0; i < a.nsites(); ++i)
      {
        algebra::vec<P::dim,float> a2f = convert::to< algebra::vec<P::dim,float> >(a[i]);
        algebra::vec<P::dim,float> a1f =
          convert::to< algebra::vec<P::dim,float> > \
          (map(q1(convert::to< algebra::vec<P::dim,float> >(a[i]))));

        f += norm::l2(a1f - q2(a2f));
      }
      return f / a.nsites();
    }


# endif // ! MLN_INCLUDE_ONLY

    } // end of nqmespace mln::registration::internal

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_INTERNAL_RMS_HH
