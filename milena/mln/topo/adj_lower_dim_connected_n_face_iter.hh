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

#ifndef MLN_TOPO_ADJ_LOWER_DIM_CONNECTED_N_FACE_ITER_HH
# define MLN_TOPO_ADJ_LOWER_DIM_CONNECTED_N_FACE_ITER_HH

/// \file
///
/// \brief Definition of forward and backward iterators on all the
/// n-faces sharing an adjacent (n-1)-face with a (reference) n-face
/// in a complex.

# include <set>

# include <mln/topo/internal/complex_relative_iterator_base.hh>
# include <mln/topo/algebraic_face.hh>


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;
    namespace internal
    {
      template <unsigned D> class adj_lower_dim_connected_n_face_iterator;
    }


    /*---------------------------------------------------.
    | topo::adj_lower_dim_connected_n_face_fwd_iter<D>.  |
    `---------------------------------------------------*/

    /*!
      \internal
      \brief Forward iterator on all the n-faces sharing an adjacent
      (n-1)-face with a (reference) n-face of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class adj_lower_dim_connected_n_face_fwd_iter
      : public internal::forward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 adj_lower_dim_connected_n_face_fwd_iter<D> >,
        private internal::adj_lower_dim_connected_n_face_iterator<D>
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef adj_lower_dim_connected_n_face_fwd_iter<D> self_;
      typedef internal::forward_complex_relative_iterator_base< topo::face<D>,
								algebraic_face<D>,
								self_ > super_;

    public:
      /// Construction.
      /// \{
      adj_lower_dim_connected_n_face_fwd_iter();
      template <typename Fref>
      adj_lower_dim_connected_n_face_fwd_iter(const Fref& f_ref);
      /// \}

      /// \brief Compute the set of faces adjacent to the reference face.
      ///
      /// The computation is delegated to
      /// mln::topo::internal::adj_lower_dim_connected_n_face_iterator.
      void update_adj_faces_();
    };


    /*---------------------------------------------------.
    | topo::adj_lower_dim_connected_n_face_bkd_iter<D>.  |
    `---------------------------------------------------*/

    /*!
      \internal
      \brief Backward iterator on all the n-faces sharing an adjacent
      (n-1)-face with a (reference) n-face of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class adj_lower_dim_connected_n_face_bkd_iter
      : public internal::backward_complex_relative_iterator_base< topo::face<D>,
								  algebraic_face<D>,
								  adj_lower_dim_connected_n_face_bkd_iter<D> >,
        private internal::adj_lower_dim_connected_n_face_iterator<D>
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef adj_lower_dim_connected_n_face_bkd_iter<D> self_;
      typedef internal::backward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 self_ > super_;

    public:
      /// Construction.
      /// \{
      adj_lower_dim_connected_n_face_bkd_iter();
      template <typename Fref>
      adj_lower_dim_connected_n_face_bkd_iter(const Fref& f_ref);
      /// \}

      /// \brief Compute the set of faces adjacent to the reference face.
      ///
      /// The computation is delegated to
      /// mln::topo::internal::adj_lower_dim_connected_n_face_iterator.
      void update_adj_faces_();
    };


    /*-------------------------------------------------------------.
    | topo::internal::adj_lower_dim_connected_n_face_iterator<D>.  |
    `-------------------------------------------------------------*/

    namespace internal
    {

      template <unsigned D>
      class adj_lower_dim_connected_n_face_iterator
      {
      protected:
	/// The actual implementation of the computation of the set of
	/// faces adjacent to the reference face.
	void update_adj_faces__(const topo::face<D>& center,
				std::vector< algebraic_face<D> >& adj_faces);
      };

    } // end of namespace mln::topo::internal



# ifndef MLN_INCLUDE_ONLY

    /*---------------------------------------------------.
    | topo::adj_lower_dim_connected_n_face_fwd_iter<D>.  |
    `---------------------------------------------------*/

    template <unsigned D>
    inline
    adj_lower_dim_connected_n_face_fwd_iter<D>::adj_lower_dim_connected_n_face_fwd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_lower_dim_connected_n_face_fwd_iter<D>::adj_lower_dim_connected_n_face_fwd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

    /* FIXME: Most of this code is shared by
       adj_lower_dim_connected_n_face_{fwd,bkd}_iter<D> and
       adj_higher_dim_connected_n_face_{fwd,bkd}_iter<D>).  Find a way
       to factor this, maybe through a common (implementation) base
       class.  */
    template <unsigned D>
    inline
    void
    adj_lower_dim_connected_n_face_fwd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      // Delegate computation to base class.
      this->update_adj_faces__(*this->c_, this->adj_faces_);
    }


    /*---------------------------------------------------.
    | topo::adj_lower_dim_connected_n_face_bkd_iter<D>.  |
    `---------------------------------------------------*/

    template <unsigned D>
    inline
    adj_lower_dim_connected_n_face_bkd_iter<D>::adj_lower_dim_connected_n_face_bkd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_lower_dim_connected_n_face_bkd_iter<D>::adj_lower_dim_connected_n_face_bkd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

    template <unsigned D>
    inline
    void
    adj_lower_dim_connected_n_face_bkd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      // Delegate computation to base class.
      this->update_adj_faces__(*this->c_, this->adj_faces_);
    }


    /*-------------------------------------------------------------.
    | topo::internal::adj_lower_dim_connected_n_face_iterator<D>.  |
    `-------------------------------------------------------------*/

    namespace internal
    {

      /* FIXME: Factor
	 topo::internal::adj_lower_dim_connected_n_face_iterator<D> and
	 topo::internal::adj_higher_dim_connected_n_face_iterator<D>.  */

      template <unsigned D>
      inline
      void
      adj_lower_dim_connected_n_face_iterator<D>::update_adj_faces__(const topo::face<D>& center,
								     std::vector< algebraic_face<D> >& adj_faces)
      {
	// Reset the result container.
	adj_faces.clear();

	/* A set of faces containing the same values as ADJ_FACES, used
	   to quickly check whether a candidate face has already been
	   added to the pool of adjacent faces. We use an std::set
	   because the lookup is cheaper (O(log(n)) vs O(n) for a
	   vector, where n is the size of the containers.  */
	std::set< algebraic_face<D> > faces_set;
	typedef std::vector< algebraic_face<D> > faces_t;

	// The set of (n-1)-faces adjacent to CENTER.
	faces_t shared_lower_faces = center.lower_dim_adj_faces();
	for (typename faces_t::const_iterator slf = shared_lower_faces.begin();
	     slf != shared_lower_faces.end(); ++slf)
	  {
	    /* The n-faces sharing an adjacent (n-1)-face with CENTER.
	       Note that this set contains CENTER itself.  */
	    faces_t adj_n_faces = slf->higher_dim_adj_faces();
	    for (typename faces_t::const_iterator af = adj_n_faces.begin();
		 af != adj_n_faces.end(); ++af)
	      {
		/* Ensure that 1. we are not adding CENTER to the result;
		   and 2. that we are not adding an already added
		   processed face.  */
		if (*af != center &&
		    faces_set.find(*af) == faces_set.end())
		  {
		    adj_faces.push_back(*af);
		    faces_set.insert(*af);
		  }
	      }
	  }
      }

    } // end of namespace mln::topo::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_ADJ_LOWER_DIM_CONNECTED_N_FACE_ITER_HH
