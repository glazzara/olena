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

#ifndef	OLN_MORPHO_RD_PARALLEL_HH
# define OLN_MORPHO_RD_PARALLEL_HH

# include <oln/morpho/Rd/utils.hh>


namespace oln
{

  namespace morpho
  {

    namespace Rd
    {

      template <typename I, typename N>
      I parallel(const I& f, const I& g, const N& nbh)
      {
	precondition(f <= g);
	I o_(f.points());
	oln_piter(I) p(f.points());

	// initialisation
	I o = level::clone(f);

	bool stability;
	do
	  {
	    level::paste(o, inplace(o_)); // memorisation

	    // opere
	    for_all(p)
	      o(p) = max_N(o_, p,nbh);
	    // conditionne
	    for_all(p)
	      o(p) = min(o(p), g(p));

	    stability = (o == o_);
	  }
	while (not stability);

	postcondition(o <= g);
	return o;
      }

    } // end of namespace oln::morpho::Rd

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_RD_PARALLEL_HH
