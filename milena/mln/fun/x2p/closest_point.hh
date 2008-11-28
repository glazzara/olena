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

#ifndef MLN_FUN_X2P_CLOSEST_POINT_HH
# define MLN_FUN_X2P_CLOSEST_POINT_HH

# include <mln/algebra/vec.hh>
# include <mln/norm/l2.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/core/site_set/box.hh>

namespace mln
{

  namespace fun
  {

    namespace x2p
    {

      ///FIXME: doxygen + concept checking
      template <typename P>
      struct closest_point
      {
        typedef algebra::vec<P::dim, float> input;
        typedef P result;

        closest_point(const p_array<P>& X, const box<P>& box)
          : box_(box), X(X)
          , log_functor_call(0)

        { }

        result
        //inline
        operator () (const input& Ck) const
        {
          ++log_functor_call;

          algebra::vec<P::dim,float> Cki = Ck;
          algebra::vec<P::dim,float> best_x = convert::to< algebra::vec<P::dim,float> >(X[0]);
          float best_d = norm::l2(Cki - best_x);
          for (unsigned j = 1; j < X.nsites(); ++j)
            {
              algebra::vec<P::dim,float> Xj = convert::to< algebra::vec<P::dim,float> >(X[j]);
              float d = norm::l2(Cki - Xj);
              if (d < best_d)
                {
                  best_d = d;
                  best_x = Xj;
                }
            }
          return convert::to<P>(best_x);
        }

        const box<P>& domain() const
        {
          return box_;
        }

        const box<P> box_;
        const p_array<P>& X;

        // log call to the functor
        mutable unsigned log_functor_call;
      };

    } // end of namespace mln::fun::x2p

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_X2P_CLOSEST_POINT_HH
