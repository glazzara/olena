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

#ifndef MLN_N_CMPT2_HH
# define MLN_N_CMPT2_HH

# include <mln/level/fill.hh>
# include <mln/core/site_set/p_vaccess.hh>

namespace mln
{

  template <typename I, typename N, typename O>
  void n_cmpt(const Image<I>& ima,
              const Neighborhood<N>& nbh_,
              unsigned limit,
              Image<O>& out);


# ifndef MLN_INCLUDE_ONLY


  template <typename I, typename N, typename O>
  void n_cmpt(const Image<I>& input_,
              const Neighborhood<N>& nbh_,
              unsigned limit,
              Image<O>& output_)
  {
    const I& input = exact(input_);
    const N& nbh = exact(nbh_);
    O& output = exact(output_);

    mln_precondition(output.domain() == input.domain());

    image2d<unsigned> v_ima = level::compute_volume(input);

    // Local type
    typedef mln_psite(I) P;
    typedef accu::volume<I> A;
    typedef p_vaccess<unsigned, p_array<point2d> > PV;

    // Auxiliary data.
    mln_ch_value(O, bool)  deja_vu;
    mln_ch_value(O, P)     parent;
    PV arr;
    fill(arr, input);

    mln_fwd_piter(PV) v(arr);
    for_all(v)
    {
      std::cout << "lambda=" << v << "-------" << std::endl;
      // init
      mln_ch_value(O, A)  data;
      initialize(data, input);
      mln::level::fill(deja_vu, false);
      {
        mln_fwd_piter(S) p(s);
        for_all(p)
          parent(p) = p;
      }

      // process
      mln_fwd_piter(S) p(s); // s required.
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        // Make set.
        data(p).take_as_init(make::pix(input, p)); // FIXME: algebraic so p!
        //data(p).take_as_init(1);
        for_all(n)
        {
          if (input.domain().has(n) && deja_vu(n))
          {
            //do_union(n, p);
            P r = find_root(parent, n);
            if (r != p)
            {
              //std::cout << data(p).to_result() << std::endl;
              if (input(r) == input(p) || (data(p).to_result() <= lambda))
                // Either a flat zone or the component of r is still growing.
              {
                if(--n_cmpt < limit)
                {
                  std::cout << "find less than " << limit << " cmpts" << std::endl;
                  std::cout << "Using last lambda" << std::endl;
                  v--;
                  last = true;
                  goto end;
                }
                data(p).take(data(r));
                parent(r) = p;
              }
              else
              {
                data(p).set_value(lambda);
              }
            }
          }
        }
      }

      if (n_cmpt == limit || last)
        goto step2;
    end:
      n_cmpt = input.domain().nsites();
    }

    // second pass
    {
      mln_bkd_piter(S) p(s);
      for_all(p)
        if (parent(p) == p) // p is root.
          output(p) = input(p);
        else
          output(p) = output(parent(p));
    }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif /* MLN_N_CMPT2_HH */

