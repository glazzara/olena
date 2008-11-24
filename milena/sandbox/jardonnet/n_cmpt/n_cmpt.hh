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

#ifndef MLN__N_CMPT_HH
# define MLN__N_CMPT_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/level/fill.hh>
# include <mln/util/pix.hh>
# include <mln/level/sort_psites.hh>

namespace mln
{

  namespace n_cmpt
  {

# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    inline
    mln_psite(I)
    find_root(I& parent,
              const mln_psite(I)& x)
    {
      if (parent(x) == x)
        return x;
      else
        return parent(x) = find_root(parent, parent(x));
    }

    template <typename I, typename N, typename O>
    void
    algebraic_union_find(const Image<I>& input_,
                         const Neighborhood<N>& nbh_,
                         unsigned limit,
                         Image<O>& output_)
    {
      trace::entering("canvas::morpho::algebraic_union_find");

      std::cout << "limit = " << limit << std::endl;


      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      O& output = exact(output_);

      mln_precondition(output.domain() == input.domain());

      // Local type.
      typedef mln_psite(I) P;

      typedef accu::volume<I> A;
      typedef mln_psite(I) P;
      typedef p_array<P> S;

      const S s(level::sort_psites_increasing(input));

      // Auxiliary data.
      mln_ch_value(O, bool)  deja_vu;
      mln_ch_value(O, P)     parent;

      int n_cmpt = input.domain().nsites();
      int lambda = 10;
      std::cout << "trying lambda = 10" << std::endl;

      // init
      {
        initialize(deja_vu, input);
        initialize(parent, input);
      }

      // first pass
      {

        do {
          mln_ch_value(O, A)     data;
          initialize(data, input);

          mln::level::fill(deja_vu, false);

          {
            mln_fwd_piter(S) p(s);
            for_all(p)
              parent(p) = p;
          }

          mln_fwd_piter(S) p(s); // s required.
          mln_niter(N) n(nbh, p);
          for_all(p)
          {
            // Make set.
            {
              data(p).take_as_init(make::pix(input, p)); // FIXME: algebraic so p!
            }

            for_all(n)
              if (input.domain().has(n) && deja_vu(n))
                {
                  //do_union(n, p);
                  P r = find_root(parent, n);
                  if (r != p)
                    {
                      if (input(r) == input(p) || (data(p).to_result() < lambda))
                        // Either a flat zone or the component of r is still growing.
                        {
                          data(p).take(data(r));
                          parent(r) = p;
                          if(--n_cmpt <= limit)
                            {
                              std::cout << "limit reached at " << n_cmpt << std::endl;
                              goto step2;
                            }
                        }
                      else
                        data(p).set_value(lambda);
                    }
                }
            deja_vu(p) = true;
          }
          n_cmpt =  input.domain().nsites();
          lambda *= 1.1;
          std::cout << "trying lambda = " << lambda << std::endl;
        }while (1);
      }

    step2:
      // second pass
      {
        mln_bkd_piter(S) p(s);
        for_all(p)
          if (parent(p) == p) // p is root.
            output(p) = input(p);
          else
            output(p) = output(parent(p));
      }

      trace::exiting("canvas::morpho::algebraic_union_find");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::n_cmpt

} // end of namespace mln

#endif /* MLN__N_CMPT_HH */

