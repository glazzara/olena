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

#ifndef MLN_TOPO_ADJ_M_FACE_ITER_HH
# define MLN_TOPO_ADJ_M_FACE_ITER_HH

/// \file
///
/// Definition of forward and backward iterators on all the
/// m-faces transitively adjacent to a (reference) n-face in a
/// complex.

# include <set>
# include <vector>

# include <mln/topo/internal/complex_relative_iterator_base.hh>
# include <mln/topo/face.hh>


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;
    namespace internal
    {
      template <unsigned D> class adj_m_face_iterator;
    }


    /*-------------------------------.
    | topo::adj_m_face_fwd_iter<D>.  |
    `-------------------------------*/

    /*!
      \internal
      \brief Forward iterator on all the m-faces transitively
      adjacent to a (reference) n-face in a complex.

      \tparam D The dimension of the complex this iterator belongs to.

      The dimension parameter (\a m_) must be lower or equal to D.

      If \a m_ is equal to the dimension of the reference face, then
      the iterated set is empty.
    */
    template <unsigned D>
    class adj_m_face_fwd_iter
      : public internal::forward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 adj_m_face_fwd_iter<D> >,
        public internal::adj_m_face_iterator<D>
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef adj_m_face_fwd_iter<D> self_;
      typedef internal::forward_complex_relative_iterator_base< topo::face<D>,
								algebraic_face<D>,
								self_ > super_;
      typedef internal::adj_m_face_iterator<D> impl_;

    public:
      /// Construction.
      /// \{
      /// Construct an iterator, with an invalid reference face, and
      /// a target dimension equal to 0.
      adj_m_face_fwd_iter();
      /// Constructs an iterator, with \a f_ref as reference face, and
      /// a target dimension equal to \a m.
      template <typename Fref>
      adj_m_face_fwd_iter(const Fref& f_ref, unsigned m);
      /// \}

      /// Compute the set of faces adjacent to the reference face.
      ///
      /// The computation is delegated to
      /// mln::topo::internal::adj_m_face_iterator.
      void update_adj_faces_();
    };


    /*-------------------------------.
    | topo::adj_m_face_bkd_iter<D>.  |
    `-------------------------------*/

    /*!
      \internal
      \brief Backward iterator on all the m-faces transitively
      adjacent to a (reference) n-face in a complex.

      \tparam D The dimension of the complex this iterator belongs to.

      The dimension parameter (\a m_) must be lower or equal to D.

      If \a m_ is equal to the dimension of the reference face, then
      the iterated set is empty.
    */
    template <unsigned D>
    class adj_m_face_bkd_iter
      : public internal::backward_complex_relative_iterator_base< topo::face<D>,
								  algebraic_face<D>,
								  adj_m_face_bkd_iter<D> >,
        public internal::adj_m_face_iterator<D>
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef adj_m_face_bkd_iter<D> self_;
      typedef internal::backward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 self_ > super_;
      typedef internal::adj_m_face_iterator<D> impl_;

    public:
      /// Construction.
      /// \{
      /// Construct an iterator, with an invalid reference face, and
      /// a target dimension equal to 0.
      adj_m_face_bkd_iter();
      /// Constructs an iterator, with \a f_ref as reference face, and
      /// a target dimension equal to \a m.
      template <typename Fref>
      adj_m_face_bkd_iter(const Fref& f_ref, unsigned m);
      /// \}

      /// Compute the set of faces adjacent to the reference face.
      ///
      /// The computation is delegated to
      /// mln::topo::internal::adj_m_face_iterator.
      void update_adj_faces_();
    };


    /*-----------------------------------------.
    | topo::internal::adj_m_face_iterator<D>.  |
    `-----------------------------------------*/

    namespace internal
    {

      template <unsigned D>
      class adj_m_face_iterator
      {
      protected:
	adj_m_face_iterator();
	adj_m_face_iterator(unsigned m);

      public:
	/// Get the target dimension.
	unsigned m() const;
	/// Set the target dimension.
	void set_m(unsigned m);

      protected:
	/// The actual implementation of the computation of the set of
	/// faces adjacent to the reference face.
	void update_adj_faces__(const topo::face<D>& center,
				std::vector< algebraic_face<D> >& adj_faces);

	/// The dimension of the iterated faces.
	unsigned m_;
      };

    } // end of namespace mln::topo::internal



# ifndef MLN_INCLUDE_ONLY

    /*-------------------------------.
    | topo::adj_m_face_fwd_iter<D>.  |
    `-------------------------------*/

    template <unsigned D>
    inline
    adj_m_face_fwd_iter<D>::adj_m_face_fwd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_m_face_fwd_iter<D>::adj_m_face_fwd_iter(const Fref& f_ref, unsigned m)
      : super_(f_ref), impl_(m)
    {
    }

    template <unsigned D>
    inline
    void
    adj_m_face_fwd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      // Delegate computation to base class.
      this->update_adj_faces__(*this->c_, this->adj_faces_);
    }


    /*-------------------------------.
    | topo::adj_m_face_bkd_iter<D>.  |
    `-------------------------------*/

    template <unsigned D>
    inline
    adj_m_face_bkd_iter<D>::adj_m_face_bkd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_m_face_bkd_iter<D>::adj_m_face_bkd_iter(const Fref& f_ref, unsigned m)
      : super_(f_ref), impl_(m)
    {
    }

    template <unsigned D>
    inline
    void
    adj_m_face_bkd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      // Delegate computation to base class.
      this->update_adj_faces__(*this->c_, this->adj_faces_);
    }


    /*-----------------------------------------.
    | topo::internal::adj_m_face_iterator<D>.  |
    `-----------------------------------------*/

    namespace internal
    {

      template <unsigned D>
      inline
      adj_m_face_iterator<D>::adj_m_face_iterator()
	: m_(0)
      {
      }

      template <unsigned D>
      inline
      adj_m_face_iterator<D>::adj_m_face_iterator(unsigned m)
	: m_(m)
      {
	mln_precondition(m <= D);
      }

      template <unsigned D>
      unsigned
      adj_m_face_iterator<D>::m() const
      {
	return m_;
      }

      template <unsigned D>
      void
      adj_m_face_iterator<D>::set_m(unsigned m)
      {
	m_ = m;
      }

      template <unsigned D>
      inline
      void
      adj_m_face_iterator<D>::update_adj_faces__(const topo::face<D>& center,
						 std::vector< algebraic_face<D> >& adj_faces)
      {
	adj_faces.clear();

	if (center.n() == m_)
	  return;

	typedef std::vector < topo::algebraic_face<D> > faces_t;
	typedef std::set < topo::algebraic_face<D> > faces_set_t;

	/* FIXME: p_faces is redundant; we could use adj_faces
	   directly.  */
	/* The adjacent p-faces being built; initialized with CENTER,
	   and filled with q-faces at each step, until q reaches
	   m_.  */
	faces_t p_faces(1,
			make_algebraic_face(center, true));
	// The set of faces being built.
	/* FIXME: This pattern is recurring in Milena---using an
	   std::set (or any fast associative container) to improve
	   the lookup speed of an std::vector; we should create a
	   class for this, a bit like mln::util::set, but with a
	   garantee on the order of insertion.  */
	faces_t work_faces;
	faces_set_t work_faces_set;

	// Iteratively compute the set of locations.
	for (unsigned p = center.n(); p != m_; (p < m_ ? ++p : --p) )
	  {
	    mln_invariant (p != m_);
	    for (typename faces_t::const_iterator g = p_faces.begin();
		 g != p_faces.end(); ++g)
	      {
		mln_invariant (g->n() != m_);
		faces_t q_faces = g->n() < m_ ?
		  g->higher_dim_adj_faces() :
		  g->lower_dim_adj_faces();
		/* Traverse the higher- or lower-dimension adjacent
		   faces of G in the natural order if G's sign is
		   positive, or in the reverse order if G's sign is
		   negative.  */
		/* FIXME: Factor; the code if the two branches is the
		   same, except for the iteration order. */
		if (g->sign())
		  {
		    for (typename faces_t::const_iterator h = q_faces.begin();
			 h != q_faces.end(); ++h)
		      // Don't insert a face twice.
		      if (work_faces_set.find(*h) == work_faces_set.end())
			{
			  work_faces.push_back(*h);
			  work_faces_set.insert(*h);
			}
		  }
		else
		  {
		    for (typename faces_t::const_reverse_iterator h =
			   q_faces.rbegin();
			 /* This is crazy.  With Apple g++ 4.0, this
			    code won't compile without this cast!
			    This is solved in MacPorts g++ 4.3.  */
			 h != (typename faces_t::const_reverse_iterator) q_faces.rend();
			 ++h)
		      // Don't insert a face twice.
		      if (work_faces_set.find(*h) == work_faces_set.end())
			{
			  work_faces.push_back(*h);
			  work_faces_set.insert(*h);
			}
		  }
	      }
	    work_faces.swap(p_faces);
	    work_faces.clear();
	    work_faces_set.clear();
	  }

	adj_faces = p_faces;
      }

    } // end of namespace mln::topo::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_ADJ_M_FACE_ITER_HH
