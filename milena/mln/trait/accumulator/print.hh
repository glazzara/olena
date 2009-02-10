// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_ACCUMULATOR_PRINT_HH
# define MLN_TRAIT_ACCUMULATOR_PRINT_HH

/// \file mln/trait/accumulator/print.hh
///
/// Print the collection of traits for an accumulator type.

# include <iostream>
# include <mln/trait/accumulators.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  // Forward declaration.
  template <typename E> struct Accumulator;


  namespace trait
  {

    namespace accumulator
    {

      template <typename A>
      void print(std::ostream& ostr = std::cout);

      template <typename A>
      void print(const Accumulator<A>& ima, std::ostream& ostr = std::cout);


# ifndef MLN_INCLUDE_ONLY

      template <typename A>
      inline
      void print(std::ostream& ostr)
      {
	mlc_is_a(A, Accumulator)::check();
	typedef mln::trait::accumulator_<A> the;
	ostr << "{ "
	     << typename the::has_untake().name() << ", "
	     << typename the::has_stop().name() << ", "
	     << typename the::has_set_value().name() << ", "
	     << typename the::when_pix().name() << " }" << std::endl;
      }

      template <typename A>
      inline
      void print(const Accumulator<A>&, std::ostream& ostr)
      {
	print<A>(ostr);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::trait::accumulator

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_ACCUMULATOR_PRINT_HH
