// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TRAIT_SITE_SET_PRINT_HH
# define MLN_TRAIT_SITE_SET_PRINT_HH

/*! \file
 *
 * \brief Print the collection of traits for a site set type.
 */

# include <iostream>
# include <mln/trait/site_sets.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  // Fwd decl.
  template <typename E> struct Site_Set;


  namespace trait
  {

    namespace site_set
    {

      template <typename S>
      void print(std::ostream& ostr = std::cout);

      template <typename S>
      void print(const Site_Set<S>& ima, std::ostream& ostr = std::cout);


# ifndef MLN_INCLUDE_ONLY

      template <typename S>
      inline
      void print(std::ostream& ostr)
      {
	mlc_is_a(S, Site_Set)::check();
	typedef mln::trait::site_set_<S> the;
	ostr << "{ "
	     << typename the::nsites()  .name() << ", "
	     << typename the::bbox()    .name() << ", "
	     << typename the::contents().name() << ", "
	     << typename the::arity()   .name() << " }" << std::endl;
      }

      template <typename S>
      inline
      void print(const Site_Set<S>&, std::ostream& ostr)
      {
	print<S>(ostr);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::trait::site_set

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_SITE_SET_PRINT_HH
