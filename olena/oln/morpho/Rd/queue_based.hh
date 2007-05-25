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

#ifndef	OLN_MORPHO_RD_QUEUE_BASED_HH
# define OLN_MORPHO_RD_QUEUE_BASED_HH

# include <queue>
# include <oln/morpho/Rd/utils.hh>


namespace oln
{

  namespace morpho
  {

    namespace Rd
    {

      template <typename I, typename N>
      I queue_based(const I& f, const I& g, const N& nbh,
		    bool echo = false)
      {
	if (echo) std::cout << std::endl;

	typedef oln_point(I) point;
	std::queue<point> q;
	I o;

	// initialisation
	{
	  o = regional_maxima(f, nbh);
	  if (echo) debug::println(o);

	  oln_piter(I) p(f.points());
	  oln_niter(N) n(nbh, p);

	  for_all(p)
	    if (o(p) != 0)
	      {
		bool ok = false;
		for_all(n) if (f.has(n))
		  if (o(n) == 0)
		    ok = true;
		if (ok)
		  q.push(p);
	      }
	}

	// propagation
	{
	  point p;
	  oln_niter(N) n(nbh, p);
	  while (not q.empty())
	    {
	      p = q.front();
	      if (echo) std::cout << std::endl << "pop " << p << " :";
	      q.pop();
	      for_all(n) if (f.has(n))
		{
		  if (o(n) < o(p) and o(n) != g(n))
		    {
		      o(n) = min(o(p), g(n));
		      if (echo) std::cout << " push " << n;
		      q.push(n);
		    }
		}
	    }
	  if (echo) std::cout << std::endl;
	}

	return o;
      }

    } // end of namespace oln::morpho::Rd

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_RD_QUEUE_BASED_HH
