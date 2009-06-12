// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

# include <mln/data/fill.hh>
# include <mln/core/site_set/p_vaccess.hh>
# include <mln/data/sort_psites.hh>
# include <mln/accu/shape/volume.hh>
# include <mln/morpho/tree/data.hh>
# include <mln/morpho/tree/compute_attribute_image.hh>
# include <mln/value/int_u.hh>

namespace mln
{

  template <typename I, typename N, typename O>
  void n_cmpt(const Image<I>& ima,
              const Neighborhood<N>& nbh_,
              unsigned limit,
              Image<O>& out);


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

  template <typename I, typename N>
  I n_cmpt(const Image<I>& input_,
           const Neighborhood<N>& nbh_,
           unsigned limit)
  {
    const I& input = exact(input_);
    const N& nbh = exact(nbh_);
    typedef I O;
    O output(input.domain());
    mln_precondition(output.domain() == input.domain());

    // compute volume image
    typedef p_array<mln_psite(I)> S;
    typedef accu::shape::volume<I> A;
    S sp = data::sort_psites_increasing(input);
    morpho::tree::data<I,S> t(input, sp, c4());
    image2d<unsigned> volume = morpho::tree::compute_attribute_image(A(), t);

    // Local type
    typedef mln_psite(I) P;

    // Auxiliary data.
    mln_ch_value(O, bool)  deja_vu;
    mln_ch_value(O, P)     parent;
    initialize(deja_vu, input);
    initialize(parent, input);
    bool       last = false;
    unsigned n_cmpt = input.domain().nsites();

    // Construct iterable value set
    typedef p_array<P> Arr;
//     typedef p_vaccess<value::int_u<10>, Arr> SV;
//     SV s = convert::to<SV>(volume);
    for
    Arr s = convert::to<Arr>(volume);

    mln_piter(Arr) last_v(s.nsites());
    mln_piter(Arr) v(s.nsites());
    for_all(v)
    {
      std::cout << "lambda=" << v << "-------" << std::endl;

      // init
      mln_ch_value(O, A)  data;
      initialize(data, input);
      mln::data::fill(deja_vu, false);
      {
        mln_fwd_piter(S) p(sp);
        for_all(p)
          parent(p) = p;
      }

      // process
      mln_fwd_piter(Arr) p(s(v));
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        data(p).take_as_init(make::pix(input, p));
        for_all(n)
        {
          if (input.domain().has(n) && deja_vu(n))
          {
            // do_union(n, p);
            P r = find_root(parent, n);
            if (r != p)
            {
              if (input(r) == input(p) || (data(p).to_result() <= (value::int_u<10>(v))))
              // Either a flat zone or the component of r is still growing.
              {
                if(--n_cmpt < limit)
                {
                  std::cout << "find less than " << limit << " cmpts" << std::endl;
                  std::cout << "Using last lambda" << std::endl;
                  v = last_v;
                  last = true;
                  goto end;
                }
                data(p).take(data(r));
                parent(r) = p;
              }
              else
                data(p).set_value(value::int_u<10>(v));
            }
          }
        }
      }

      if (n_cmpt == limit || last)
        goto step2;
    end:
      n_cmpt = input.domain().nsites();
      last_v = v;
    }
  step2:
    // second pass
    {
      mln_bkd_piter(S) p(sp);
      for_all(p)
        if (parent(p) == p) // p is root.
          output(p) = input(p);
        else
          output(p) = output(parent(p));
    }

    return output;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif /* MLN_N_CMPT2_HH */

