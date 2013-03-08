// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_SEQUENCE_HH
# define MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_SEQUENCE_HH

/// \file
///
/// Definition of a meta complex relative iterator using two
/// complex relative iterators sequentially.

# include <iosfwd>

# include <mln/metal/equal.hh>

# include <mln/core/concept/iterator.hh>
# include <mln/topo/complex.hh>


namespace mln
{

  namespace topo
  {

    namespace internal
    {

      template <typename I1>
      struct const_face_type_
      {
	typedef typename I1::face_type F_;
	typedef const F_& ret;
      };


      /*!
	\internal
	\brief A meta relative iterator on the faces of a complex
	using two complex relative iterators sequentially.

        The first relative iterator is used, and when it becomes
        invalid, the second one is used.

        \tparam I1 The type of the first relative iterator.
        \tparam I2 The type of the second relative iterator.
        \tparam E  The type exact type of the iterator.
      */
      template <typename I1, typename I2, typename E>
      class complex_relative_iterator_sequence : public Iterator<E>
      {
	typedef complex_relative_iterator_sequence<I1, I2, E> self_;

      public:
	/// The type of the center face.
	typedef typename I1::center_type center_type;
	/// The type of the iterated faces.
	typedef typename I1::face_type face_type;

	/// Construction and assignment.
	/// \{
	complex_relative_iterator_sequence();
	template <typename Fref>
	complex_relative_iterator_sequence(const Fref& f_ref);
	/// \}

	/// Manipulation.
	/// \{
	/// Change the center face.
	void center_at(const center_type& c);

	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();

	/// Start an iteration.
	void start();
	/// Go to the next point.
	void next_();
	/// \}

	/// Conversion.
	/// \{
	/// Return a reference to the corresponding face handle.
	operator typename const_face_type_<I1>::ret () const;
	//      that is:  const face_type&
	/// \}

      protected:
	/// Update the target face.
	void update_();

      protected:
	/// The first complex relative iterator.
	I1 iter1_;
	/// The second complex relative iterator.
	I2 iter2_;

	/// The face handle this iterator is pointing to.
	face_type f_;
      };


      /// Print an mln::topo::internal::complex_relative_iterator_sequence.
      template <typename I1, typename I2, typename E>
      inline
      std::ostream&
      operator<<(std::ostream& ostr,
		 const complex_relative_iterator_sequence<I1, I2, E>& p);



# ifndef MLN_INCLUDE_ONLY

      template <typename I1, typename I2, typename E>
      inline
      complex_relative_iterator_sequence<I1, I2, E>::complex_relative_iterator_sequence()
      {
	// Ensure I1 and I2 are compatible.
	mlc_equal(typename I1::face_type, typename I2::face_type)::check();

	invalidate();
      }

      template <typename I1, typename I2, typename E>
      template <typename Fref>
      inline
      complex_relative_iterator_sequence<I1, I2, E>::complex_relative_iterator_sequence(const Fref& f_ref)
      {
	// Ensure I1 and I2 are compatible.
	mlc_equal(typename I1::face_type, typename I2::face_type)::check();

	center_at(f_ref);
      }

      template <typename I1, typename I2, typename E>
      inline
      void
      complex_relative_iterator_sequence<I1, I2, E>::center_at(const center_type& c)
      {
	iter1_.center_at(c);
	iter2_.center_at(c);
	invalidate();
      }

      template <typename I1, typename I2, typename E>
      inline
      bool
      complex_relative_iterator_sequence<I1, I2, E>::is_valid() const
      {
	return iter1_.is_valid() || iter2_.is_valid();
      }

      template <typename I1, typename I2, typename E>
      inline
      void
      complex_relative_iterator_sequence<I1, I2, E>::invalidate()
      {
	iter1_.invalidate();
	iter2_.invalidate();
      }

      template <typename I1, typename I2, typename E>
      inline
      void
      complex_relative_iterator_sequence<I1, I2, E>::start()
      {
	iter1_.start();
	iter2_.start();
	if (is_valid())
	  update_();
      }

      template <typename I1, typename I2, typename E>
      inline
      void
      complex_relative_iterator_sequence<I1, I2, E>::next_()
      {
	/* Iterator<E>::next() ensures that either iter1_ or iter2_
	   (or both) are valid before executing this method.  */
	if (iter1_.is_valid())
	  iter1_.next();
	else
	  iter2_.next();
	if (is_valid())
	  update_();
      }

      template <typename I1, typename I2, typename E>
      inline
      void
      complex_relative_iterator_sequence<I1, I2, E>::update_()
      {
	mln_precondition(is_valid());
	if (iter1_.is_valid())
	  f_ = iter1_;
	else
	  f_ = iter2_;
      }

      template <typename I1, typename I2, typename E>
      inline
      complex_relative_iterator_sequence<I1, I2, E>::operator typename const_face_type_<I1>::ret () const
      {
	return f_;
      }


      template <typename I1, typename I2, typename E>
      inline
      std::ostream&
      operator<<(std::ostream& ostr,
		 const complex_relative_iterator_sequence<I1, I2, E>& p)
      {
	return ostr << typename I1::face_type(p);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::internal

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_SEQUENCE_HH
