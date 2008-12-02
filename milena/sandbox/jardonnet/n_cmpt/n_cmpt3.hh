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

#ifndef MLN_N_CMPT3_HH
# define MLN_N_CMPT3_HH

# include <mln/labeling/regional_minima.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/util/set.hh>

# include <mln/debug/println.hh>

# include <mln/accu/volume.hh>
# include <mln/morpho/tree/data.hh>
# include <mln/morpho/tree/compute_attribute_image.hh>

namespace mln
{

  namespace n_cmpt
  {

    template < typename I >
    void n_cmpt3(const I& (((((((ima))))))));

# ifndef MLN_INCLUDE_ONLY

    template<typename I>
    mln_ch_value(I, util::set<mln_psite(I)>)
    minima_sets(const I& ima)
    {
      mln_ch_value(I, util::set<mln_psite(I)>) ima_set(ima.domain());

      mln_piter(I) p(ima.domain());
      for_all(p)
      {
        if (ima(p) != literal::zero)
          ima_set(p).insert(p);
      }
      return ima_set;
    }

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

    template < typename I >
    unsigned count_minima(const I& ima)
    {
      unsigned cmpt = 0;
      mln_piter(I) p(ima.domain());
      for_all(p)
      {
        if (ima(p) != literal::zero)
          cmpt++;
      }
      return cmpt;
    }

    template < typename I, typename N>
    I
    n_cmpt3(const I& ima, const N& nbh,
            unsigned lambda)
    {
      unsigned label;

      // get /ima/ regional minima
      mln_ch_value(I, unsigned) min = labeling::regional_minima(ima, nbh, label);
      std::cout << "/ima/ regional minima" << std::endl;
      debug::println(min);

      // compute volume image
      typedef p_array<mln_psite(I)> S;
      typedef image2d<unsigned> V;
      typedef accu::volume<I> A;
      S sp = level::sort_psites_decreasing(ima);
      morpho::tree::data<I,S> t(ima, sp, nbh);
      V volume = morpho::tree::compute_attribute_image(A(), t);

      // get /volume/ regional minima
      mln_ch_value(I, unsigned) min_v = labeling::regional_minima(volume, nbh, label);
      std::cout << "/volume/ regional minima" << std::endl;
      debug::println(min_v);

      // tester minima de ima == minima de attr
      //mln_assertion(min == min_v);

      mln_ch_value(I, util::set<mln_psite(I)>) volume_set =  minima_sets(volume);

      // number of minima
      int cmpts = count_minima(min_v);

      std::cout << "Nb of regionnal minima : " << cmpts << std::endl;

      // prepare union find
      typedef mln_psite(V) P;
      //data
      mln_ch_value(V, accu::volume<V>)  data;
      initialize(data, volume);
      //deja_vu
      mln_ch_value(V, bool)  deja_vu;
      initialize(deja_vu, volume);
      mln::level::fill(deja_vu, false);
      //parent
      mln_ch_value(V, P) parent;
      initialize(parent, volume);
      {
        mln_fwd_piter(S) p(sp);
        for_all(p)
          parent(p) = p;
      }

      // union find sur volume
      mln_fwd_piter(S) p(sp);
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        // Make set.
        data(p).take_as_init(make::pix(volume, p));
        for_all(n)
        {
          if (volume.domain().has(n) && deja_vu(n))
          {
            //do_union(n, p);
            P r = find_root(parent, n);
            if (r != p)
            {
              if (volume(r) != volume(p) && (data(p).to_result() > lambda))
              {
                data(p).set_value(lambda);
                continue;
              }

              if (volume(r) != volume(p))
              {
                std::cout << "1: volume"<<r<<" != volume"<<p<<"" << " with data"<<p<< " = " <<
                  data(p).to_result() << std::endl;
                if (not volume_set(p).is_empty())
                {
                  std::cout << "2: not volume_set"<<p<<".is_empty()" << std::endl;
                  if (volume_set(p) != volume_set(r))
                  {
                    std::cout << "3: volume_set"<<p<<" != volume_set"<<r<< std::endl;
                    cmpts--;
                  }
                }
              }
              // propagate set
              volume_set(p).insert(volume_set(r));
              assert(data(p).to_result() != 0);
              // propagate attribute
              data(p).take(data(r));
              // build tree
              parent(r) = p;
            }
          }
        }
        deja_vu(p) = true;
      }

      std::cout << "Nb cmpts after processing : " << cmpts << std::endl;

      // second pass
      I output(ima.domain());
      {
      mln_bkd_piter(S) p(sp);
      for_all(p)
        if (parent(p) == p) // p is root.
          output(p) = ima(p);
        else
          output(p) = output(parent(p));
      }
      return output;
    }

  } // end of namespace n_cmpt

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif /* MLN_N_CMPT3_HH */

