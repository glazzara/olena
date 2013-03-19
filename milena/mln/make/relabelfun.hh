// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_MAKE_RELABELFUN_HH
# define MLN_MAKE_RELABELFUN_HH

/// \file
///
/// Routines to construct a function in order to relabel a labeled image.

# include <mln/core/concept/function.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/value/next.hh>

namespace mln
{

  namespace make
  {

    /// Create a i2v function from a v2b function.
    /// This function can be used to relabel a labeled image.
    ///
    /// \param[in] fv2b		A v2b function.
    /// \param[in] nlabels	The number of labels.
    /// \param[in] new_nlabels	The number of labels after relabeling.
    ///
    /// \return a i2v function.
    ///
    /// \sa mln::labeling::relabel
    template <typename V, typename F>
    fun::i2v::array<V>
    relabelfun(const Function_v2b<F>& fv2b,
	       const V&		      nlabels,
	       V&		      new_nlabels);

    /// Create a i2v function from a v2v function.
    /// This function can be used to relabel a labeled image.
    ///
    /// \param[in] fv2v		A v2v function. This function maps an id to
    ///				an already existing one.
    /// \param[in] nlabels	The number of labels.
    /// \param[in] new_nlabels	The number of labels after relabeling.
    ///
    /// \return a i2v function.
    ///
    /// \sa mln::labeling::relabel
    template <typename V, typename F>
    fun::i2v::array<V>
    relabelfun(const Function_v2v<F>& fv2v,
	       const V&		      nlabels,
	       V&		      new_nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename F>
    inline
    fun::i2v::array<V>
    relabelfun(const Function_v2b<F>& fv2b_,
	       const V&		      nlabels,
	       V&		      new_nlabels)
    {
      mln_trace("make::relabelfun");

      const F& fv2b = exact(fv2b_);

      unsigned nlabels_i = value::next(nlabels);
      V tmp_nlabels = literal::zero;
      fun::i2v::array<V> fi2v(nlabels_i, literal::zero);
      for (V i = 1; i < nlabels_i; ++i)
	if (fv2b(i))
	  fi2v(i) = ++tmp_nlabels;
      new_nlabels = tmp_nlabels;
      return fi2v;
    }


    template <typename V, typename F>
    inline
    fun::i2v::array<V>
    relabelfun(const Function_v2v<F>& fv2v_,
	       const V&		      nlabels,
	       V&		      new_nlabels)
    {
      mln_trace("make::relabelfun");

      const F& fv2v = exact(fv2v_);

      unsigned nlabels_i = value::next(nlabels);

      util::array<V> new_labels(nlabels_i, mln_max(V));
      new_labels(0) = literal::zero;
      V tmp_nlabels = literal::zero;
      fun::i2v::array<V> fi2v(nlabels_i, mln_max(V));
      fi2v(0) = literal::zero;

      for (V i = 1; i < nlabels_i; ++i)
      {
	mln_assertion(fv2v(i) <= nlabels);
	if (new_labels(fv2v(i)) == mln_max(V))
	{
	  new_labels(fv2v(i)) = ++tmp_nlabels;
	  fi2v(i) = tmp_nlabels;
	}
	else
	  fi2v(i) = new_labels(fv2v(i));
      }
      new_nlabels = tmp_nlabels;
      return fi2v;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_RELABELFUN_HH
