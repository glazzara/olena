// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_TOPO_INTERNAL_COMPLEX_ITERATOR_BASE_HH
# define MLN_TOPO_INTERNAL_COMPLEX_ITERATOR_BASE_HH

/** \file mln/topo/internal/complex_iterator_base.hh

    \brief Definition of an implementation (factoring) class for
    iterators on complexes.

    Concrete iterators should inherit either from
    mln::topo::internal::complex_set_iterator_base or from
    mln::topo::internal::complex_relative_iterator_base (but not from
    mln::topo::internal::complex_iterator_base directly).

    \see mln/topo/internal/complex_set_iterator_base.hh
    \see mln/topo/internal/complex_relative_iterator_base.hh


    The hierarchy of (top) implementation classes is as follow.

    \verbatim

                       complex_iterator_base<F, E>
                                    ^
                                    |
                     ,--------------+--------------.
                     |                             |
    complex_set_iterator_base<F, E> complex_relative_iterator_base<C, F, E>

    \endverbatim
*/

# include <iosfwd>
# include <iostream>

# include <mln/core/concept/iterator.hh>
# include <mln/topo/complex.hh>


namespace mln
{

  namespace topo
  {

    namespace internal
    {
      /// Factoring class for iterators on mln::complex.
      ///
      /// \arg \p F The type of the face handle.
      /// \arg \p E The type exact type of the iterator.
      template <typename F, typename E>
      class complex_iterator_base : public Iterator<E>
      {
	typedef complex_iterator_base<F, E> self_;

      public:
	/// The type of the iterated faces.
	typedef F face;

	/// Conversion.
	///
	/// Return a reference to the corresponding face handle.
	/// \{
	/// Explicit conversion (accessor).
	const face& subject() const;
	/// Implicit conversion (conversion operator).
	operator const face&() const;
	/// \}

      protected:
	/// The face handle this iterator is pointing to.
	face f_;
      };


      /// Print an mln::topo::internal::complex_iterator_base.
      template <typename F, typename E>
      inline
      std::ostream&
      operator<<(std::ostream& ostr, const complex_iterator_base<F, E>& p);



# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename E>
      inline
      const F&
      complex_iterator_base<F, E>::subject() const
      {
	return f_;
      }

      template <typename F, typename E>
      inline
      complex_iterator_base<F, E>::operator const face&() const
      {
	return f_;
      }


      template <typename F, typename E>
      inline
      std::ostream&
      operator<<(std::ostream& ostr, const complex_iterator_base<F, E>& p)
      {
	return ostr << F(p);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::internal

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_INTERNAL_COMPLEX_ITERATOR_BASE_HH
