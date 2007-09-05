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

#ifndef MLN_LABELING_BASE_HH
# define MLN_LABELING_BASE_HH

/*! \file mln/labeling/base.hh
 *
 * \brief Base class for labeling functors.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling.hh>


namespace mln
{

  namespace labeling
  {

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      /// Base class for labeling functors.
      template <typename I_, typename N_, typename O_>
      struct base_
      {
	typedef I_ I;
	typedef N_ N;
	typedef O_ O;
	typedef mln_point(I_) P;

	const I& input;
	const N& nbh;
	O&       output;

	mln_value(O_) nlabels;
	bool status;

	base_(const I_& input, const N_& nbh, O_& output)
	  : input(input),
	    nbh(nbh),
	    output(output)
	{
	}

	// Defaults.

	bool handles(const P&) const         { return true;  }
	bool labels(const P&) const          { return true;  }
 	void init()                          {}
	void do_no_union(const P&, const P&) {}
	void init_attr(const P&)             {}
	void merge_attr(const P&, const P&)  {}
      };

    } // end of namespace mln::labeling::impl

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_BASE_HH
