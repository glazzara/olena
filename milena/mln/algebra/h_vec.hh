// Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_ALGEBRA_H_VEC_HH
# define MLN_ALGEBRA_H_VEC_HH

/// \file
///
/// Definition of a vector with homogeneous coordinates.

# include <mln/algebra/vec.hh>
# include <mln/literal/one.hh>


namespace mln
{

  // Forward declaration.
  namespace algebra {
    template <unsigned d, typename C> class h_vec;
  }


  namespace trait
  {

    // For unary traits.

    template < template <class> class Name,
	       unsigned d, typename C >
    struct set_precise_unary_< Name, algebra::h_vec<d, C> >
    {
      typedef mln_trait_unary(Name, C) V;
      typedef algebra::h_vec<d, V> ret;
    };

    // For binary traits.

    template < template <class, class> class Name,
	       unsigned d, typename C,
	       typename Q >
    struct set_precise_binary_< Name,
				algebra::h_vec<d, C>, algebra::h_vec<d, Q> >
    {
      typedef mln_trait_binary(Name, C, Q) V;
      typedef algebra::h_vec<d, V> ret;
    };

    template < template <class, class> class Name,
	       unsigned d, typename C,
	       typename S >
    struct set_precise_binary_< Name,
				algebra::h_vec<d, C>, mln::value::scalar_<S> >
    {
      typedef mln_trait_binary(Name, C, S) V;
      typedef algebra::h_vec<d, V> ret;
    };

  } // end of namespace mln::trait



  namespace algebra
  {


    /*! \brief N-Dimensional vector with homogeneous coordinates.
      \ingroup mlnalgebratypes
     */
    template <unsigned d, typename C>
    class h_vec : public vec<d + 1, C>
    {
    public:
      /// Dimension is the 'natural' one (3 for 3D), not the one of the vector (dim + 1).
      enum { dim = d };

      /// Constructor without argument.
      h_vec();
      /// Constructor with the underlying vector.
      h_vec(const vec<d+1, C>& other);

      h_vec& operator=(const vec<d+1, C>& rhs);

      /// Back to the natural (non-homogeneous) space.
      vec<d,C> to_vec() const;
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
    h_vec<d,C>::h_vec(const vec<d+1, C>& other)
      : vec<d+1, C>(other)
    {
    }

    template <unsigned d, typename C>
    inline
    h_vec<d,C>& h_vec<d,C>::operator=(const vec<d+1, C>& rhs)
    {
      if (& rhs == this)
	return *this;
      this->vec<d+1, C>::operator=(rhs);
      return *this;
    }

    template <unsigned d, typename C>
    inline
    vec<d,C> h_vec<d,C>::to_vec() const
    {
      const C w = this->data_[d];
      mln_assertion(w != 0);

      vec<d,C> tmp;
      for (unsigned i = 0; i < d; ++i)
	tmp[i] = static_cast<C>(this->data_[i] / w);
      return tmp;
    }

    // Immersion of a vector in its homogeneous space.

    template <unsigned n, typename T>
    inline
    h_vec<n, T>
    vec<n,T>::to_h_vec() const
    {
      h_vec<n, T> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = this->data_[i];
      tmp[n] = literal::one;
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::algebra

} // end of namespace mln


#endif // ! MLN_ALGEBRA_H_VEC_HH
