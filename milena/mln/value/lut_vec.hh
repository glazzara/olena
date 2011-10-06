// Copyright (C) 2007, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_VALUE_LUT_VEC_HH
# define MLN_VALUE_LUT_VEC_HH

/// \file
///
/// Define some basic lut_vecs of values from value types.
///
/// \todo Use fun::saturate in lut_vec ctor.

# include <vector>

# include <mln/core/concept/value_set.hh>
# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  /// Forward declarations.
  namespace fun {
    namespace i2v {
      template <typename T> class array;
    } // end of namespace mln::fun::i2v
  } // end of namespace mln::fun
  namespace util{
    template <typename T> class array;
  } // end of namespace mln::util

  namespace value
  {

    // Fwd decls.
    template <typename S> struct fwd_viter_;
    template <typename S> struct bkd_viter_;


    /// Class that defines FIXME
    /*!
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

      /// \name Constructors
      /// \{

      /// Constructor from a value set and any Function_v2v.
      template <typename F>
      lut_vec(const S& vset, const Function_v2v<F>& f);

      /// Constructor from a value set and any fun::i2v::array.
      template <typename V>
      lut_vec(const S& vset, const Function_v2v< fun::i2v::array<V> >& f);

      /// Constructor from a value set and any util::array.
      template <typename V>
      lut_vec(const S& vset, const Function_v2v< util::array<V> >& f);

      /// \}

    protected:

      const S& vset_;
      std::vector<T> vec_;
      unsigned n_;
    };


    template <typename S, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const lut_vec<S,T>& lut);


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
      (void) v;
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
	vec_.push_back(f_(vset[i]));
    }

    template <typename S, typename T>
    template <typename V>
    inline
    lut_vec<S,T>::lut_vec(const S& vset, const Function_v2v< fun::i2v::array<V> >& f)
      : vset_(vset)
    {
      const fun::i2v::array<V>& f_ = exact(f);
      n_ = f_.size();
      vec_ = f_.std_vector();
    }

    template <typename S, typename T>
    template <typename V>
    inline
    lut_vec<S,T>::lut_vec(const S& vset, const Function_v2v< util::array<V> >& f)
      : vset_(vset)
    {
      const util::array<V>& f_ = exact(f);
      n_ = f_.size();
      vec_ = f_.std_vector();
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

    template <typename S, typename T>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const lut_vec<S,T>& lut)
    {
      ostr << "[ ";
      for (unsigned i = 0; i < lut.nvalues(); ++i)
	ostr << i << ':' << lut[i] << ' ';
      ostr << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value


} // end of namespace mln


# include <mln/value/viter.hh>


#endif // ! MLN_VALUE_LUT_VEC_HH
