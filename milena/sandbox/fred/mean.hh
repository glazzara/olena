// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MORPHO_ACCU_MEAN_HH
# define MLN_MORPHO_ACCU_MEAN_HH

/// \file mln/canvas/algebraic.hh
///
/// Apply leveling connected filter to images.
///
/// \todo Debug, add input validity check.

# include <mln/core/macros.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/util/pix.hh>
# include <mln/accu/mean.hh>

# include "accu_trait.hh"

namespace mln {

  namespace morpho {

    namespace accu {

      template <typename I>
      /// Morphological (i.e. for pixel and pixel values) accumulator calculating mean.
      /// Valid for leveling algorithm.
      /// FIXME: is inclusion polyphormism really appliable ?
      struct mean: public Accumulator< mean<I> >,
                   private mln::accu::mean<mln_value(I)>
      {
        typedef mln::accu::mean<mln_value(I)> super;
        typedef util::pix<I> argument;
        typedef typename super::result result;
        typedef typename super::q_result q_result;

        using super::take;
        using super::take_as_init;
        using super::init;

        void take(const mln_value(I)& v);
        void take(const argument& t);
        void take(const mean<I>& m);
      };

      # ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void mean<I>::take(const typename mean<I>::argument& t)
      {
        this->take(t.v());
      }

      template <typename I>
      inline
      void mean<I>::take(const mln_value(I)& v)
      {
        this->super::take(v);
      }

      template <typename I>
      inline
      void mean<I>::take(const mean<I>& m)
      {
        this->super::take(m);
      }

      # endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::accu

  } // end of namespace mln::morpho

  namespace trait {

    template <typename I>
    struct accu_< morpho::accu::mean<I> > :
    public undefined_accu_ < morpho::accu::mean<I> >
    {
      typedef accu::when_pix::use_only_v when_pix;
    };

  } // end of namespace mln::trait

} // end of namespace mln

#endif /* ! MLN_MORPHO_ACCU_MEAN_HH */