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

# include <mln/literal/colors.hh>
# include <mln/io/ppm/save.hh>

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
      unsigned label;

      std::cout << "/ima/" << std::endl;
      debug::println(ima);

      // get /ima/ regional minima
      mln_ch_value(I, unsigned) min = labeling::regional_minima(ima, nbh, label);

      // sort ima psites
      typedef mln_psite(I) P;
      typedef p_array<P> S;
      S sp = data::sort_psites_increasing(ima);

      // init watershed image
      mln_ch_value(I, value::rgb8) wst(ima.domain());
      mln::data::fill(wst, literal::black);

      // number of minima
      unsigned cmpts = label;

      if (lambda > cmpts)
        std::cout << "warning : lambda value is to hight." << std::endl;

      //// prepare union find
      // fused
      mln_ch_value(I, bool) fused;
      initialize(fused, ima);
      mln::data::fill(fused, false);
      // deja_vu
      mln_ch_value(I, bool)  deja_vu(ima.domain());
      mln::data::fill(deja_vu, false);
      //parent
      mln_ch_value(I, P) parent(ima.domain());
      {
        mln_fwd_piter(S) p(sp);
        for_all(p)
        {
          parent(p) = p;
          if (min(p) != 0) // p in a reg min of the attribute image
          {
            fused(p) = true; // ok
            wst(p) = literal::red;
          }
        }
      }

      std::cout << "cmpts |      volume_set      | " << std::endl;
      std::cout << cmpts << " : ";
      std::cout << std::endl;

      // union find
      mln_fwd_piter(S) p(sp);
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        std::cout << p << std::endl;
        for_all(n)
        {
          if (ima.domain().has(n) && deja_vu(n))
          {
            //do_union(n, p);
            P r = find_root(parent, n);
            if (r != p)
            {
              // One cmpt less if
              if (ima(r) != ima(p)) // r and p have differerent volumes
                if (cmpts >= lambda) // union is still alowed
                  if (fused(p)) // p already belong to a cmpt (fused for an another n)
                    cmpts--;

              if (cmpts >= lambda ||
                  ima(r) == ima(p) ||
                  not fused(p))
              {
                parent(r) = p;
                // propagate set
                fused(p) = true;

                //min_v(p) = min_v(r); //FIXME: fusion may happend with a non minima value

                fused(n) = true; // We cannot mark minima at init !   ... ?

                std::cout << "ima " << ima(p) << " - " << cmpts << std::endl;
                debug::println(fused);
              }

              // mark limit point
              if (parent(p) != parent(n)) // wrong
                wst(p) = literal::red;
            }
          }
        }
        deja_vu(p) = true;
      }

      I iota(ima.domain());
      debug::iota(iota);

      io::ppm::save(wst, "wst.ppm");

      //  step2:
      std::cout << std::endl;
      std::cout << "cmpts : " << cmpts << std::endl;

      // second pass
      I output(ima.domain());
      {
        mln_bkd_piter(S) p(sp);
        for_all(p)
         if (parent(p) == p) // p is root.
         {
           output(p) = iota(p) * 10;
         }
         else
           output(p) = output(parent(p));
      }

      return output;
    }

  } // end of namespace n_cmpt

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif /* MLN_N_CMPT3_HH */

