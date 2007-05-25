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
    cc_tarjan(const Image_with_Nbh<I>& input);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename T, typename U>
      struct data_
      {
	typedef T I;
	typedef U N;

	const I& f;
	const N& nbh;

	oln_plain_value(I, unsigned) output;
	oln_plain(I) is_processed;
	oln_plain_value(I, oln_point(I)) parent;

	data_(const I& f, const N& nbh)
	  : f(f), nbh(nbh)
	{
	}
      };

      template <typename Data>
      struct cc_tarjan_
      {
	cc_tarjan_()
	{
	}

	oln_point(Data::I)
	find_root(Data& data,
		  const oln_point(Data::I)& x)
	{
	  if (data.parent(x) != x)
	  {
	    data.parent(x) = find_root(data, data.parent(x));
	    return data.parent(x);
	  }
	  return x;
	}

	void
	do_union(Data& data,
		 const oln_point(Data::I)& n,
		 const oln_point(Data::I)& p)
	{
	  oln_point(Data::I) r = find_root(data, n);
	  if (r != p)
	    data.parent(r) = p;
	}

	void init(Data& data)
	{
	  prepare(data.is_processed, with, data.f);
	  prepare(data.output, with, data.f);
	  prepare(data.parent, with, data.f);
	  level::fill(inplace(data.is_processed), false);
	}

	void first_pass_body(const oln_point(Data::I)& p,
			     Data& data)
	{
	  data.parent(p) = p;
	  if ( data.f(p) )
	  {
	    oln_niter(Data::N) n(data.nbh, p);
	    for_all(n) if ( data.f.has(n) )
	      {
		if ( data.f(n) == true and data.is_processed(n) )
		  do_union(data, n, p);
	      }
	    data.is_processed(p) = true;
	  }
	}

	void second_pass_body(const oln_point(Data::I)& p,
			      Data& data)
	{
	  unsigned current_label = 0;
	  if ( data.f(p) == true and data.parent(p) == p )
	    data.output(p) = ++current_label;
	  else
	    data.output(p) = data.output(data.parent(p));
	}

	void final(Data&)
	{
	}

      };

    } // end of namespace oln::morpho::impl

// Facades.

    template <typename I, typename N>
    oln_plain_value(I, unsigned)
    cc_tarjan(const Binary_Image<I>& input,
	      const Neighborhood<N>& nbh)
    {
      typedef impl::data_<I, N> data_t;

      data_t data(exact(input), exact(nbh));
      impl::cc_tarjan_< data_t > f;
      canvas::v3::two_pass(f, data);
      return data.output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_CC_TARJAN_HH
