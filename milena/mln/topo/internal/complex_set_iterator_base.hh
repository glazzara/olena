// Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH
# define MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH

/// \file
///
/// Definition of an implementation (factoring) class for
/// (set) iterators on faces of a complex.

# include <mln/topo/internal/complex_iterator_base.hh>


namespace mln
{

  namespace topo
  {

    namespace internal
    {

      // Forward declaration.
      template <typename F, typename E>
      class complex_iterator_base;


      /*!
	\internal
	\brief Factoring class for (set) iterators on mln::complex.

        \tparam F The type of the face handle.
        \tparam E The type exact type of the iterator.
      */
      template <typename F, typename E>
      class complex_set_iterator_base : public complex_iterator_base<F, E>
      {
	typedef complex_set_iterator_base<F, E> self_;

      public:
	/// The type of the iterated faces.
	typedef F face_type;
	// FIXME: Maybe we could just get the dimension D of the face's
	// complex, an define complex_type as mln::complex<D>?
	typedef typename F::complex_type complex_type;

	/// Construction and assignment.
	/// \{
	complex_set_iterator_base();
	complex_set_iterator_base(const complex_type& c);
	/// \}

	/// Manipulation.
	/// \{
	/// Change the target complex.
	void set_cplx(const complex_type& c);

	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();
	/// \}
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename E>
      inline
      complex_set_iterator_base<F, E>::complex_set_iterator_base()
      {
	invalidate();
      }

      template <typename F, typename E>
      inline
      complex_set_iterator_base<F, E>::complex_set_iterator_base(const complex_type& c)
      {
	this->f_.set_cplx(c);
	// Invalidate f_.
	invalidate();
      }

      template <typename F, typename E>
      inline
      void
      complex_set_iterator_base<F, E>::set_cplx(const complex_type& c)
      {
	this->f_.set_cplx(c);
	// Invalidate f_.
	invalidate();
      }

      template <typename F, typename E>
      inline
      bool
      complex_set_iterator_base<F, E>::is_valid() const
      {
	return this->f_.is_valid();
      }

      template <typename F, typename E>
      inline
      void
      complex_set_iterator_base<F, E>::invalidate()
      {
	this->f_.invalidate();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::internal

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH
