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
# include <mln/debug/iota.hh>

# include <mln/accu/volume.hh>
# include <mln/morpho/tree/data.hh>
# include <mln/morpho/tree/compute_attribute_image.hh>

namespace mln
{

  namespace n_cmpt
  {

    template < typename I >
    void n_cmpt5(const I& (((((((ima))))))));

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

    template < typename I, typename N>
    I
    n_cmpt3(const I& ima, const N& nbh,
            unsigned lambda)
    {
      std::cout << "/ima/" << std::endl;
      debug::println(ima);

      // compute volume image
      //---------------------

      typedef p_array<mln_psite(I)> S;
      typedef mln_ch_value(I,unsigned) V;
      typedef accu::volume<I> A;

      S sp = level::sort_psites_decreasing(ima);
      morpho::tree::data<I,S> t(ima, sp, nbh);
      V volume = morpho::tree::compute_attribute_image(A(), t);
      sp = level::sort_psites_increasing(volume);
      std::cout << "/volume/" << std::endl;
      debug::println(volume);


      // get /volume/ regional minima
      //-----------------------------

      unsigned label;
      mln_ch_value(I, unsigned) min_v = labeling::regional_minima(volume, nbh, label);
      std::cout << "/volume/ regional minima" << std::endl;
      debug::println(min_v);

      // number of minima
      unsigned cmpts = label;
      if (lambda > cmpts)
        std::cout << "warning : lambda value is to hight." << std::endl;

      std::cout << cmpts << std::endl;


      // two pass algo
      //--------------

      // init fused image
      mln_ch_value(I, bool) fused;
      initialize(fused, volume);
      mln::level::fill(fused, false);

      // prepare union find
      typedef mln_psite(V) P;

      //data
      mln_ch_value(V, accu::volume<V>)  data(volume.domain());

      //deja_vu
      mln_ch_value(V, bool)  deja_vu(volume.domain());
      mln::level::fill(deja_vu, false);

      //parent
      mln_ch_value(V, P) parent(volume.domain());
      {
        mln_fwd_piter(S) p(sp);
        for_all(p)
        {
          parent(p) = p;

          // Mandatory since we propagate fused
          if (min_v(p) != 0) // p in a reg min of the attribute image
            fused(p) = true; // ok
        }
      }

      // UNION FIND ON VOLUME
      mln_fwd_piter(S) p(sp);
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        std::cout << p << std::endl;
        //if (volume(p) > lambda)
        // goto step2;
        for_all(n)
        {
          if (volume.domain().has(n) && deja_vu(n))
          {
            //do_union(n, p);
            P r = find_root(parent, n);
            if (r != p)
            {
              // One cmpt less if
              if (volume(r) != volume(p)) // r and p have differerent volumes
                if (fused(p)) // p already belong to a cmpt (fused for an another n)
                  if (cmpts >= lambda) // union is still alowed
                    cmpts--;

              if (cmpts >= lambda ||
                  volume(r) == volume(p) ||
                  not fused(p))
              {
                parent(r) = p;

                // This test is mandatory. Sometimes (--_) points are fused
                // tought none of these points belongs to a component (attached
                // to a local minima).  In this case fused(p) must not be true
                // since it can be fused again without removing a component.
                // looking if r is fused should be enought.
                // This test force minima to be initialized fused.
                if (fused(r))
                  fused(p) = true;

                // If I try to fuse with something never fused I am on a plateau.
                // not fused(r) => ( volume(r) == volume(p) )
                mln_invariant(fused(r) || volume(r) == volume(p));
                // fused(r) and ( volume(r) == volume(p) ) happen on minima plateau.

                // fused(n) = true; // useless ? probably yes because when we
                // want to know that we really fuse component, we look at
                // fused(r) not n.
                // fused(n) is not an invariant: --_. And it is ok (I think).
                // We don't have to retro-propagate fused.

                //std::cout << "volume " << volume(p) << " - " << cmpts << std::endl;
                debug::println(fused);
              }
            }
          }
        }
        deja_vu(p) = true;
      }

      I iota(ima.domain());
      debug::iota(iota);

      std::cout << std::endl;
      std::cout << "cmpts : " << cmpts << std::endl;

      // second pass
      I output(ima.domain());
      {
        mln_bkd_piter(S) p(sp);
        for_all(p)
         if (parent(p) == p) // p is root.
           output(p) = iota(p) * 10;
         else
           output(p) = output(parent(p));
      }

      return output;
    }

  } // end of namespace n_cmpt

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif /* MLN_N_CMPT3_HH */

