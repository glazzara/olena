// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MAKE_RELABELFUN_HH
# define MLN_MAKE_RELABELFUN_HH

/// \file mln/make/relabelfun.hh
///
/// Routines to construct a function in order to relabel a labeled image.

# include <mln/core/concept/function.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/value/label.hh>

namespace mln
{

  namespace make
  {

    /// Create a i2v function from a v2b function.
    /// This function can be used to relabel a labeled image.
    ///
    /// \param[in] fv2b A v2b function.
    /// \param[in] nlabels The number of labels.
    /// \param[in] new_nlabels The number of labels after relabeling.
    ///
    /// \return a i2v function.
    ///
    /// \sa mln::labeling::relabel
    template <unsigned n, typename F>
    fun::i2v::array< value::label<n> >
    relabelfun(const Function_v2b<F>& fv2b,
	       const value::label<n>& nlabels,
	       value::label<n>&	      new_nlabels);

    /// Create a i2v function from a v2v function.
    /// This function can be used to relabel a labeled image.
    ///
    /// \param[in] fv2v A v2v function.
    /// \param[in] nlabels The number of labels.
    /// \param[in] new_nlabels The number of labels after relabeling.
    ///
    /// \return a i2v function.
    ///
    /// \sa mln::labeling::relabel
    template <unsigned n, typename F>
    fun::i2v::array< value::label<n> >
    relabelfun(const Function_v2v<F>& fv2v,
	       const value::label<n>& nlabels,
	       value::label<n>&	      new_nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename F>
    inline
    fun::i2v::array< value::label<n> >
    relabelfun(const Function_v2b<F>& fv2b_,
	       const value::label<n>& nlabels,
	       value::label<n>&	      new_nlabels)
    {
      trace::entering("make::relabelfun");

      const F& fv2b = exact(fv2b_);

      value::label<n> tmp_nlabels = literal::zero;
      typedef value::label<n> label_t;
      fun::i2v::array<label_t> fi2v(nlabels.next(), literal::zero);
      for (label_t i = 1; i <= nlabels; ++i)
	if (fv2b(i))
	{
	  fi2v(i) = ++tmp_nlabels;
	}
      new_nlabels = tmp_nlabels;
      trace::exiting("make::relabelfun");
      return fi2v;
    }


    template <unsigned n, typename F>
    inline
    fun::i2v::array< value::label<n> >
    relabelfun(const Function_v2v<F>& fv2v_,
	       const value::label<n>& nlabels,
	       value::label<n>&	      new_nlabels)
    {
      trace::entering("make::relabelfun");

      const F& fv2v = exact(fv2v_);

      value::label<n> tmp_nlabels = literal::zero;
      typedef value::label<n> label_t;
      fun::i2v::array<label_t> fi2v(nlabels.next(), literal::zero);
      for (label_t i = 1; i < nlabels.next(); ++i)
	if (fi2v(fv2v(i)) == literal::zero)
	{
	  fi2v(fv2v(i)) = ++tmp_nlabels;
	  fi2v(i) = tmp_nlabels;
	}
	else
	  fi2v(i) = fi2v(fv2v(i));
      new_nlabels = tmp_nlabels;
      trace::exiting("make::relabelfun");
      return fi2v;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_RELABELFUN_HH
