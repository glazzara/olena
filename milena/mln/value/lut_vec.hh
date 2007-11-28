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

#ifndef MLN_VALUE_LUT_VEC_HH
# define MLN_VALUE_LUT_VEC_HH

/*! \file mln/value/lut_vec.hh
 *
 * \brief Define some basic lut_vecs of values from value types.
 */

# include <vector>

# include <mln/core/concept/value_set.hh>
# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace value
  {

    // Fwd decls.
    template <typename S> struct fwd_viter_;
    template <typename S> struct bkd_viter_;


    /*! Class that defines FIXME
     *
     * \warning This is a multi-set!!!
     * FIXME
     *
     */
    template <typename S, typename T>
    struct lut_vec : public Value_Set< lut_vec<S,T> >
    {
      /// Value associated type.
      typedef T value;

      /// Forward Value_Iterator associated type.
      typedef fwd_viter_< lut_vec<S,T> > fwd_viter;

      /// Backward Value_Iterator associated type.
      typedef bkd_viter_< lut_vec<S,T> > bkd_viter;

      /// Give the \p i-th value.
      T operator[](unsigned i) const;

      /// Give the number of values.
      unsigned nvalues() const;

      // Apply the look-up-table.  FIXME: Doc!
      T operator()(const mln_value(S)& val) const;

      /// Test if \p v belongs to this set.
      bool has(const value& v) const;

      /// Give the index of value \p v in this set.
      unsigned index_of(const value& v) const;

      /// Ctor. FIXME!
      template <typename F>
      lut_vec(const S& vset, const Function_v2v<F>& f);

    protected:

      const S& vset_;
      std::vector<T> vec_;
      unsigned n_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename T>
    inline
    bool
    lut_vec<S,T>::has(const T&) const
    {
      mln_invariant(0); // FIXME
      return false;
    }

    template <typename S, typename T>
    inline
    unsigned
    lut_vec<S,T>::index_of(const T& v) const
    {
      mln_invariant(0); // FIXME
      return 0;
    }

    template <typename S, typename T>
    template <typename F>
    inline
    lut_vec<S,T>::lut_vec(const S& vset, const Function_v2v<F>& f)
      : vset_(vset)
    {
      const F& f_ = exact(f);
      n_ = vset.nvalues();
      vec_.reserve(n_);
      for (unsigned i = 0; i < n_; ++i)
	vec_[i] = f_(vset[i]);
    }

    template <typename S, typename T>
    inline
    T
    lut_vec<S,T>::operator()(const mln_value(S)& val) const
    {
      mln_precondition(vset_.index_of(val) < n_);
      return vec_[vset_.index_of(val)];
    }

    template <typename S, typename T>
    inline
    T
    lut_vec<S,T>::operator[](unsigned i) const
    {
      mln_precondition(i < nvalues());
      return vec_[i];
    }

    template <typename S, typename T>
    inline
    unsigned
    lut_vec<S,T>::nvalues() const
    {
      return vec_.size();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value


} // end of namespace mln


# include <mln/value/viter.hh>


#endif // ! MLN_VALUE_LUT_VEC_HH
