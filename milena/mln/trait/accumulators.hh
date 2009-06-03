// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_ACCUMULATORS_HH
# define MLN_TRAIT_ACCUMULATORS_HH

/// \file mln/trait/accumulators.hh
///
/// Some base trait types for accumulators.

# include <mln/trait/accumulator/props.hh>
# include <mln/trait/undef.hh>


/// Shortcut to the accumulator property about untake method disponibility
# define mln_trait_accumulator_has_untake(A) typename mln::trait::accumulator_< A >::has_untake

/// Shortcut to the accumulator property about setvalue method disponibility
# define mln_trait_accumulator_has_set_value(A) typename mln::trait::accumulator_< A >::has_set_value

/// Shortcut to the accumulator property about stop method disponibility
# define mln_trait_accumulator_has_stop(A) typename mln::trait::accumulator_< A >::has_stop

/// Shortcut to the accumulator property about behavior when pixel is given as take() value
/// Used for instance in mln::canvas::morpho::dataing
# define mln_trait_accumulator_when_pix(A) typename mln::trait::accumulator_< A >::when_pix



namespace mln
{

  namespace trait
  {

    template <typename A>
    struct undefined_accumulator_
    {
      typedef undef has_untake;
      typedef undef has_stop;
      typedef undef has_set_value;
      typedef accumulator::when_pix::not_ok when_pix;
    };

    template <typename A>
    struct accumulator_ : undefined_accumulator_<A>
    {
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/accumulator/print.hh>


#endif // ! MLN_TRAIT_ACCUMULATORS_HH
