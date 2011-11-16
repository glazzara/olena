// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_BASE_HH
# define MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_BASE_HH

/** \file
    Definition of an implementation (factoring) class for
    iterators on mln::complex.

    The hierarchy of classes in this file is as follows
    
    \verbatim

                    complex_relative_iterator_base<C, F, E>
                                    ^
                                    |
                     ,--------------+--------------.
                     |                             |
 forward_complex_relative_iterator_base<C, F, E>   |
                                                   |
                            backward_complex_relative_iterator_base<C, F, E>

    \endverbatim
*/

# include <vector>

# include <mln/topo/internal/complex_iterator_base.hh>

/* FIXME: By moving iterator `i_' into
   internal::complex_relative_iterator_base, we'll be able to factor
   more methods (do_next_, update_f); this requires the type of this
   iterator be passed as an extra parameter to
   internal::complex_relative_iterator_base.  */


namespace mln
{

  namespace topo
  {

    namespace internal
    {

      // Forward declaration.
      template <typename F, typename E>
      class complex_iterator_base;


      /*----------------------------------------------------------.
      | topo::internal::complex_relative_iterator_base<C, F, E>.  |
      `----------------------------------------------------------*/

      /// Factoring class for relative iterators on mln::complex.
      ///
      /// \tparam C The type of the center face handle.
      /// \tparam F The type of the face handle.
      /// \tparam E The type exact type of the iterator.
      template <typename C, typename F, typename E>
      class complex_relative_iterator_base :
	public complex_iterator_base<F, E>
      {
	typedef complex_relative_iterator_base<C, F, E> self_;

      public:
	/// The type of the center face.
	typedef C center_type;
	/// The type of the iterated faces.
	typedef F face_type;

	/// Construction.
	/// \{
	complex_relative_iterator_base();
	template <typename Fref>
	complex_relative_iterator_base(const Fref& f_ref);
	/// \}

	/// Manipulation.
	/// \{
	/// Change the center face.
	void center_at(const center_type& c);

	/// Start an iteration.
	void start();
	/// Go to the next point.
	void next_();
	/// \}

      protected:
	/// A pointer to the center face around which this iterator
	/// moves.
	const center_type* c_;

	// The type of the set of vicinity sites (adjacent face handles).
	typedef std::vector<face_type> adj_faces_t;
	/// The set of faces adjacent to the reference face.
	adj_faces_t adj_faces_;
      };



      /*------------------------------------------------------------------.
      | topo::internal::forward_complex_relative_iterator_base<C, F, E>.  |
      `------------------------------------------------------------------*/

      /// Factoring class for forward relative iterators on
      /// mln::complex.
      ///
      /// \tparam C The type of the center face handle.
      /// \tparam F The type of the face handle.
      /// \tparam E The type exact type of the iterator.
      template <typename C, typename F, typename E>
      class forward_complex_relative_iterator_base
	: public complex_relative_iterator_base<C, F, E>
      {
	typedef forward_complex_relative_iterator_base<C, F, E> self_;
	typedef complex_relative_iterator_base<C, F, E> super_;

      public:
	typedef F face_type;

      public:
	/// Construction.
	/// \{
	forward_complex_relative_iterator_base();
	template <typename Fref>
	forward_complex_relative_iterator_base(const Fref& f_ref);
	/// \}

      public:
	/// Manipulation
	/// \{
	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();

	/// Start an iteration.
	void do_start_();
	/// Go to the next point.
	void do_next_();

	/// Update the target face.
	void update_f_();
	/// \}

      protected:
	/// An iterator on the set of adjacent edges.
	typename super_::adj_faces_t::const_iterator i_;
      };


      /*-------------------------------------------------------------------.
      | topo::internal::backward_complex_relative_iterator_base<C, F, E>.  |
      `-------------------------------------------------------------------*/

      /// Factoring class for backward relative iterators on
      /// mln::complex.
      ///
      /// \tparam C The type of the center face handle.
      /// \tparam F The type of the face handle.
      /// \tparam E The type exact type of the iterator.
      template <typename C, typename F, typename E>
      class backward_complex_relative_iterator_base
	: public complex_relative_iterator_base<C, F, E>
      {
	typedef backward_complex_relative_iterator_base<C, F, E> self_;
	typedef complex_relative_iterator_base<C, F, E> super_;

      public:
	typedef F face_type;

      public:
	/// Construction.
	/// \{
	backward_complex_relative_iterator_base();
	template <typename Fref>
	backward_complex_relative_iterator_base(const Fref& f_ref);
	/// \}

      public:
	/// Manipulation
	/// \{
	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();

	/// Start an iteration.
	void do_start_();
	/// Go to the next point.
	void do_next_();

	/// Update the target face.
	void update_f_();
	/// \}

      protected:
	/// An iterator on the set of adjacent edges.
	typename super_::adj_faces_t::const_reverse_iterator i_;
      };



# ifndef MLN_INCLUDE_ONLY

      /*----------------------------------------------------------.
      | topo::internal::complex_relative_iterator_base<C, F, E>.  |
      `----------------------------------------------------------*/

      template <typename C, typename F, typename E>
      inline
      complex_relative_iterator_base<C, F, E>::complex_relative_iterator_base()
	: c_(0)
      {
	// Check for required methods in E.
	void (E::*m)() = & E::update_adj_faces_;
	(void) m;

 	exact(this)->invalidate();
      }

      template <typename C, typename F, typename E>
      template <typename Fref>
      inline
      complex_relative_iterator_base<C, F, E>::complex_relative_iterator_base(const Fref& f_ref)
      {
	// Check for required methods in E.
	void (E::*m)() = & E::update_adj_faces_;
	(void) m;

	center_at(f_ref);
      }

      template <typename C, typename F, typename E>
      inline
      void
      complex_relative_iterator_base<C, F, E>::center_at(const C& c)
      {
 	c_ = &c;
 	exact(this)->invalidate();
      }

      template <typename C, typename F, typename E>
      inline
      void
      complex_relative_iterator_base<C, F, E>::start()
      {
	exact(this)->do_start_();
	if (exact(this)->is_valid())
	  exact(this)->update_f_();
      }

      template <typename C, typename F, typename E>
      inline
      void
      complex_relative_iterator_base<C, F, E>::next_()
      {
	exact(this)->do_next_();
	if (exact(this)->is_valid())
	  exact(this)->update_f_();
      }



      /*------------------------------------------------------------------.
      | topo::internal::forward_complex_relative_iterator_base<C, F, E>.  |
      `------------------------------------------------------------------*/

      template <typename C, typename F, typename E>
      inline
      forward_complex_relative_iterator_base<C, F, E>::forward_complex_relative_iterator_base()
      {
      }

      template <typename C, typename F, typename E>
      template <typename Fref>
      inline
      forward_complex_relative_iterator_base<C, F, E>::forward_complex_relative_iterator_base(const Fref& f_ref)
	: super_(f_ref)
      {
      }

      template <typename C, typename F, typename E>
      inline
      bool
      forward_complex_relative_iterator_base<C, F, E>::is_valid() const
      {
	return i_ != this->adj_faces_.end();
      }

      template <typename C, typename F, typename E>
      inline
      void
      forward_complex_relative_iterator_base<C, F, E>::invalidate()
      {
	i_ = this->adj_faces_.end();
      }

      template <typename C, typename F, typename E>
      inline
      void
      forward_complex_relative_iterator_base<C, F, E>::do_start_()
      {
	exact(this)->update_adj_faces_();
	i_ = this->adj_faces_.begin();
      }

      template <typename C, typename F, typename E>
      inline
      void
      forward_complex_relative_iterator_base<C, F, E>::do_next_()
      {
	++i_;
      }

      template <typename C, typename F, typename E>
      inline
      void
      forward_complex_relative_iterator_base<C, F, E>::update_f_()
      {
	mln_precondition(is_valid());
	this->f_ = *i_;
      }


      /*-------------------------------------------------------------------.
      | topo::internal::backward_complex_relative_iterator_base<C, F, E>.  |
      `-------------------------------------------------------------------*/

      template <typename C, typename F, typename E>
      inline
      backward_complex_relative_iterator_base<C, F, E>::backward_complex_relative_iterator_base()
      {
      }

      template <typename C, typename F, typename E>
      template <typename Fref>
      inline
      backward_complex_relative_iterator_base<C, F, E>::backward_complex_relative_iterator_base(const Fref& f_ref)
	: super_(f_ref)
      {
      }

      template <typename C, typename F, typename E>
      inline
      bool
      backward_complex_relative_iterator_base<C, F, E>::is_valid() const
      {
	return i_ != this->adj_faces_.rend();
      }

      template <typename C, typename F, typename E>
      inline
      void
      backward_complex_relative_iterator_base<C, F, E>::invalidate()
      {
	i_ = this->adj_faces_.rend();
      }

      template <typename C, typename F, typename E>
      inline
      void
      backward_complex_relative_iterator_base<C, F, E>::do_start_()
      {
	exact(this)->update_adj_faces_();
	i_ = this->adj_faces_.rbegin();
      }

      template <typename C, typename F, typename E>
      inline
      void
      backward_complex_relative_iterator_base<C, F, E>::do_next_()
      {
	++i_;
      }

      template <typename C, typename F, typename E>
      inline
      void
      backward_complex_relative_iterator_base<C, F, E>::update_f_()
      {
	mln_precondition(is_valid());
	this->f_ = *i_;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::internal

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_INTERNAL_COMPLEX_RELATIVE_ITERATOR_BASE_HH
