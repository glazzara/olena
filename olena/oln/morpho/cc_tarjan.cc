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

namespace oln
{

  namespace morpho
  {

    template <typename I>
    oln_point(I) find_root(const oln_point(I)& x)
    {
      if (parent[x] != x)
      {
	parent[x] = find_root(parent[x]);
	return parent(x);
      }
      return x;
    }

    template <typename I>
    void do_union(const oln_point(I)& n,
		  const oln_point(I)& p,
		  const oln_plain_value(I, oln_point(I))& parent)
    {
      oln_point(I) r = find_root(n);
      if (r != p)
	parent(r) = p;
    }

    template <typename I>
    oln_plain_value(I, unsigned)
      cc_tarjan(const Binary_Image<I>& input)
    {
      oln_plain_value(I, unsigned) output;
      prepare(output, with, input);

      oln_plain_value(I, oln_point(I)) parent;
      prepare(parent, with, input);

      // Init
      unsigned current_label = 0;
      oln_plain(I) is_processed;
      prepare(is_processed, with, input);
      oln_piter(I) p(input.points());
      for_all(p)
	is_processed(p) = false; // FIXME : built with.


      // Fist pass
      oln_piter(I) p(input.points());
      for_all(p)
      {
	oln_niter(I) n(p, input);
	for_all(n)
	{
	  if ( is_processed(n) )
	    do_union(n, p, parent);
	}
	is_processed(p) = true;
      }


      // Second pass
      oln_piter(I) p2(input.points());
      for_all(p2)
      {
	if ( parent(p) == p )
	  output(p) = ++current_label;
	else
	  output(p) = output(parent(p));
      }
    }

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_DILATION_HH
