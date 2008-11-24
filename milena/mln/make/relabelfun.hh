// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
# include <mln/fun/l2l/relabel.hh>
# include <mln/value/label.hh>

namespace mln
{

  namespace make
  {

    /// Create a l2l function from a l2b function.
    /// This function can be used to relabel a labeled image.
    ///
    /// \param[in] f a l2b function.
    ///
    /// \return a l2l function.
    ///
    /// \sa mln::labeling::relabel
    template <unsigned n, typename F>
    mln::fun::l2l::relabel< value::label<n> >
    relabelfun(const mln::Function_l2b<F>&  fl2b,
	       const value::label<n>&	    nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename F>
    inline
    mln::fun::l2l::relabel< value::label<n> >
    relabelfun(const mln::Function_l2b<F>&  fl2b_,
	       const value::label<n>&	    nlabels,
	       value::label<n>&		    new_nlabels)
    {
      trace::entering("mln::make::relabelfun");

      const F& fl2b = exact(fl2b_);

      value::label<n> tmp_nlabels = literal::zero;
      typedef value::label<n> label_t;
      fun::l2l::relabel<label_t> fl2l(nlabels.next(), literal::zero);
      for (label_t i = 1; i <= nlabels; ++i)
	if (fl2b(i))
	{
	  fl2l(i) = ++tmp_nlabels;
	}
      new_nlabels = tmp_nlabels;
      trace::exiting("mln::make::relabelfun");
      return fl2l;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_RELABELFUN_HH
