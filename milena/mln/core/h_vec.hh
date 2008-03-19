// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_H_VEC_HH
# define MLN_CORE_H_VEC_HH

/*! \file mln/core/h_vec.hh
 *
 * \brief Definition of a vector with homogeneous coordinates.
 */

# include <mln/algebra/vec.hh>
# include <mln/literal/one.hh>

namespace mln
{

  // Fwd decl.
  template <unsigned d, typename C> struct h_vec;



  namespace trait
  {

    // For unary traits.

    template < template <class> class Name,
	       unsigned d, typename C >
    struct set_precise_unary_< Name, h_vec<d, C> >
    {
      typedef mln_trait_unary(Name, C) V;
      typedef h_vec<d, V> ret;
    };

    // For binary traits.

    template < template <class, class> class Name,
	       unsigned d, typename C,
	       typename Q >
    struct set_precise_binary_< Name,
				h_vec<d, C>, h_vec<d, Q> >
    {
      typedef mln_trait_binary(Name, C, Q) V;
      typedef h_vec<d, V> ret;
    };

    template < template <class, class> class Name,
	       unsigned d, typename C,
	       typename S >
    struct set_precise_binary_< Name,
				h_vec<d, C>, mln::value::scalar_<S> >
    {
      typedef mln_trait_binary(Name, C, S) V;
      typedef h_vec<d, V> ret;
    };

  } // end of namespace mln::trait


  /*! \brief N-Dimensional vector with homogeneous coordinates.
   *
   */
  template <unsigned d, typename C>
  struct h_vec : public algebra::vec<d + 1, C>
  {
    /// Dimension is the 'natural' one (3 for 3D), not the one of the vector (dim + 1).
    enum { dim = d };

    /// Constructor without argument.
    h_vec();
    /// Constructor with the underlying vector.
    h_vec(const algebra::vec<d+1, C>& other);

    h_vec& operator=(const algebra::vec<d+1, C>& rhs);

    /// Back to the natural (non-homogeneous) space.
    algebra::vec<d,C> to_vec() const;
  };



# ifndef MLN_INCLUDE_ONLY

  template <unsigned d, typename C>
  inline
  h_vec<d,C>::h_vec()
  {
    /* Safety measure: set the last component to the unit (1).  This
       way, converting an unitialized h_vec to a vector won't trigger
       division-by-zero errors if this last component were randomly
       initialized to 0.  */
    this->data_[d] = literal::one;
  }

  template <unsigned d, typename C>
  inline
  h_vec<d,C>::h_vec(const algebra::vec<d+1, C>& other)
    : algebra::vec<d+1, C>(other)
  {
  }

  template <unsigned d, typename C>
  inline
  h_vec<d,C>& h_vec<d,C>::operator=(const algebra::vec<d+1, C>& rhs)
  {
    if (& rhs == this)
      return *this;
    this->algebra::vec<d+1, C>::operator=(rhs);
    return *this;
  }

  namespace algebra
  {

    // Immersion of a vector in its homogeneous space.
    template <unsigned n, typename T>
    inline
    h_vec<n, T> vec<n,T>::to_h_vec() const
    {
      h_vec<n, T> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = this->data_[i];
      tmp[n] = literal::one;
      return tmp;
    }

  } // end of namespace mln::algebra


  template <unsigned d, typename C>
  inline
  algebra::vec<d,C> h_vec<d,C>::to_vec() const
  {
    const C w = this->data_[d];
    mln_assertion(w != 0);

    algebra::vec<d,C> tmp;
    for (unsigned i = 0; i < d; ++i)
      tmp[i] = this->data_[i] / w;
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_H_VEC_HH
