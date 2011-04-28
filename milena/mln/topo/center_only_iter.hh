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

#ifndef MLN_TOPO_CENTER_ONLY_ITER_HH
# define MLN_TOPO_CENTER_ONLY_ITER_HH

/// \file
///
/// \brief Definition of a complex relative iterator iterating on its
/// center (reference face) only.
///
/// \see mln/topo/centered_iter_adapter.hh

# include <mln/topo/internal/complex_relative_iterator_base.hh>
# include <mln/topo/face.hh>


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> struct face;


    /*----------------------------.
    | topo::center_only_iter<D>.  |
    `----------------------------*/

    /** \brief Iterator on all the adjacent (n-1)-faces of the n-face
        of an mln::complex<D>.

        \tparam D The dimension of the complex this iterator belongs to.

        mln::topo::center_only_iter inherits from
        mln::topo::internal::forward_complex_relative_iterator_base,
        but it could inherit from
        mln::topo::internal::backward_complex_relative_iterator_base
        as well, since it always contains a single element, the
        center/reference face (and the traversal order is
        meaningless).

        This iterator is essentially used to implement other iterators.
        \see mln::topo::centered_iter_adapter
        \see mln::complex_lower_window
        \see mln::complex_higher_window
        \see mln::complex_lower_higher_window  */
    template <unsigned D>
    class center_only_iter
      : public internal::forward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 center_only_iter<D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef center_only_iter<D> self_;
      typedef internal::forward_complex_relative_iterator_base< topo::face<D>,
								algebraic_face<D>,
								self_ > super_;

    public:
      /// Construction.
      /// \{
      center_only_iter();
      template <typename Fref>
      center_only_iter(const Fref& f_ref);
      /// \}

      /// Compute the set of faces adjacent to the reference face.
      void update_adj_faces_();
    };



# ifndef MLN_INCLUDE_ONLY

    /*----------------------------.
    | topo::center_only_iter<D>.  |
    `----------------------------*/

    template <unsigned D>
    inline
    center_only_iter<D>::center_only_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    center_only_iter<D>::center_only_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

    template <unsigned D>
    inline
    void
    center_only_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      this->adj_faces_.clear();
      this->adj_faces_.push_back(make_algebraic_face(*this->c_, true));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_CENTER_ONLY_ITER_HH
