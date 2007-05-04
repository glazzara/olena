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
// You should have receiv a copy of the GNU General Public License
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

#ifndef	OLN_MORPHO_CC_TARJAN_HH
# define OLN_MORPHO_CC_TARJAN_HH

# include <oln/core/concept/image.hh>
# include <oln/canvas/two_pass.hh>
# include <oln/level/fill.hh>
# include <oln/core/internal/f_ch_value.hh>

namespace oln
{

  namespace morpho
  {

    template <typename I>
    oln_plain_value(I, unsigned)
    cc_tarjan(const Image_with_Nbh<I>& f);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename I>
      struct cc_tarjan_ : public canvas::v4::two_pass<I, cc_tarjan_<I> >
      {
	typedef oln_point(I) point;

	const I& f;

	oln_plain_value(I, unsigned)  output;
	oln_plain_value(I, bool) is_processed;
	oln_plain_value(I, point) parent;
	unsigned nlabels;

	cc_tarjan_(const I& f)
	  : canvas::v4::two_pass<I, cc_tarjan_<I> >(f),
	    f(f)
	{
	}

	void impl_init()
	{
	  prepare(is_processed, with, f);
	  prepare(output, with, f);
	  prepare(parent, with, f);
	  level::fill(inplace(is_processed), false);
	  nlabels = 0;
	}

	void impl_first_pass_body(const point& p)
	{
	  parent(p) = p;
	  if (f(p) == true)
	  {
	    oln_niter(I) n(f, p);
	    for_all(n)
	      {
		if ( f(n) == true and is_processed(n) )
		  do_union(n, p);
	      }
	    is_processed(p) = true;
	  }
	}

	void impl_second_pass_body(const point& p)
	{
	  if (f(p) == true)
	    {
	      if (parent(p) == p)
		output(p) = ++nlabels;
	      else
		output(p) = output(parent(p));
	    }
	  else
	    output(p) = 0; // bg label
	}

	void impl_final()
	{
	}

	// auxiliary methods

	point find_root(const point& x) // FIXME: or w/o const&?
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}

	void do_union(const point& n,
		      const point& p)
	{
	  point r = find_root(n);
	  if (r != p)
	    parent(r) = p;
	}
      };

    } // end of namespace oln::morpho::impl

    template <typename I>
    oln_plain_value(I, unsigned)
    cc_tarjan(const Image_with_Nbh<I>& f)
    {
      impl::cc_tarjan_<I> cc(exact(f));

      cc.run();

      return cc.output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // ! OLN_MORPHO_CC_TARJAN_HH
