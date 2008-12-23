// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef	MLN_MORPHO_RD_PARALLEL_HH
# define MLN_MORPHO_RD_PARALLEL_HH

# include "utils.hh"


namespace mln
{

  namespace morpho
  {

    namespace Rd
    {


      template <typename I, typename N>
      I parallel(const I& f, const I& g, const N& nbh,
		 bool test = true)
      {
	if (test)
	  mln_precondition(f <= g);

	I o_(f.domain());
	mln_piter(I) p(f.domain());

	// initialisation
 	I o = duplicate(f);

	bool stability;
	do
	  {
	    data::paste(o, o_); // memorisation

	    // opere
	    for_all(p)
	      o(p) = max_N(o_, p, nbh);
	    // conditionne
	    for_all(p)
	      o(p) = min(o(p), g(p));

	    stability = (o == o_);
	  }
	while (not stability);

	if (test)
	  mln_postcondition(o <= g);
	return o;
      }

    } // end of namespace mln::morpho::Rd

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_PARALLEL_HH
