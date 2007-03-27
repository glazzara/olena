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

#ifndef	OLN_MORPHO_DILATION_HH
# define OLN_MORPHO_DILATION_HH

#include <oln/debug/print.hh>

namespace oln
{

  namespace morpho
  {

    template <typename I>
    oln_point(I) find_root(I& ima,
			   const oln_point(I)& x,
			   oln_plain_value(I, oln_point(I))& parent)
    {
      std::cout << "enter_root( " << parent(x) << "," << x << " )" << std::endl;

      if (parent(x) != x)
      {
	parent(x) = find_root(ima, parent(x), parent);
	return parent(x);
      }
      return x;

      std::cout << "leave_root" << std::endl;
    }

    template <typename I>
    void do_union(I& ima,
		  const oln_point(I)& n,
		  const oln_point(I)& p,
		  oln_plain_value(I, oln_point(I))& parent)
    {
      oln_point(I) r = find_root(ima, n, parent);
      if (r != p)
	parent(r) = p;
    }

    template <typename I>
    void first_pass(const I& input,
		    oln_plain_value(I, oln_point(I))& parent,
		    oln_plain(I)& is_processed)
    {
      oln_bkd_piter(I) p(input.points());
      for_all(p)
	{
	  if ( input(p) )
	  {
	    parent(p) = p;
	    oln_niter(I) n(p, input);
	    for_all(n)
	      {
		if ( input(n) )
		{
		  if ( is_processed(n) )
		  {
		    do_union(input ,n, p, parent);
		    std::cout << "union ("<< p << ") -> parent :" << std::endl;
		    debug::print(parent);
		  }
		}
	      }
	    is_processed(p) = true;
	  }
	}
      std::cout << "pass 1" << std::endl;
    }

    template <typename I>
    void second_pass(const I& input,
		     oln_plain_value(I, oln_point(I))& parent,
		     oln_plain_value(I, unsigned)& output)
    {
      unsigned current_label = 0;

      // Second pass
      oln_fwd_piter(I) p(input.points());
      for_all(p)
	{
	  if ( input(p) )
	  {
	    if ( parent(p) == p )
	      output(p) = ++current_label;
	    else
	      output(p) = output(parent(p));
	    std::cout << "output :" << std::endl;
	    debug::print(output);
	  }
	}
    }

    template <typename I>
    oln_plain_value(I, unsigned)
      cc_tarjan(const Image_with_Nbh<I>& input)
    {
      oln_plain_value(I, unsigned) output;
      prepare(output, with, input);

      oln_plain_value(I, oln_point(I)) parent;
      prepare(parent, with, input);

      // Init
      oln_plain(I) is_processed;
      prepare(is_processed, with, input);
      oln_piter(I) p1(input.points());
      for_all(p1)
	is_processed(p1) = false; // FIXME : built with.

      first_pass(input, parent, is_processed);

      second_pass(input, parent, output);

      ::oln::debug::print(parent);
      return output;
    }

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_DILATION_HH
