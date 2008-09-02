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

#ifndef MLN_DT_CANVAS_HH
# define MLN_DT_CANVAS_HH

# include <vector>

namespace mln
{

  namespace canvas
  {

    // General version.

    template <typename F>
    class dt
    {
    public:

      // Ctor.
      dt(F& f);

      typedef typename F::I I;
      typedef typename F::N N;
      typedef mln_point(I) point;

      mln_ch_value(I, unsigned) distance;

    private:

      // Functor.
      F& f;

      void init();
      void run();

      std::vector< std::vector<point> > bucket;
      unsigned bucket_size;
      unsigned mod;
    };


# ifndef MLN_INCLUDE_ONLY

    /*-------------.
    | canvas::dt.  |
    `-------------*/

    template <typename F>
    dt<F>::dt(F& f)
      : f(f),
	bucket_size(0)
    {
      trace::entering("canvas::dt");

      this->init();
      this->run();

      trace::exiting("canvas::dt");
    }

    template <typename F>
    void
    dt<F>::init()
    {
      // Preconditions.
      mln_precondition(f.input.has_data());

      f.init(); //< f call.

      initialize(distance, f.input);

      // Mod determination.
      mln::accu::max_<unsigned> accu;
      mln_fwd_piter(I) p(f.input.domain());
      mln_qiter(N) n(f.nbh, p);
      for_all(n)
	accu.take(n.w());
      mod = accu.to_result() + 1;

      bucket.resize(mod);

      // Initialization
      for_all(p)
      {
	f.init_in_for(p); //< f call.
	if (f.input(p))
	{
	  distance(p) = literal::zero;
	  for_all(n)
	    if (f.input.has(n) && ! f.input(n))
	    {
	      bucket[0].push_back(p);
	      ++bucket_size;
	      break;
	    }
	}
	else
	  distance(p) = f.max;
      }
    }

    template <typename F>
    void
    dt<F>::run()
    {
      point p;
      mln_qiter(N) n(f.nbh, p);

      for (unsigned d = 0; bucket_size != 0; ++d)
      {
	std::vector<point>& bucket_d = bucket[d % mod];
	for (unsigned i = 0; i < bucket_d.size(); ++i)
	{
	  p = bucket_d[i];

	  /* |00|    |00|    insert "a" in bucket 1    |00|
	     |ab| -> |12| -> insert "b" in bucket 1 -> |11| -> then in bucket 2.
	     when d = 2, "b" has already been processed when d = 1. */
	  if (distance(p) < d)
	    continue;

	  for_all(n)
	    if (distance.has(n) && distance(n) > d)
	    {
	      unsigned newDist = d + n.w();

	      if (newDist < distance(n))
	      {
		f.run_in_for(p, n); //< f call.
		distance(n) = newDist;
		bucket[newDist % mod].push_back(n);
		++bucket_size;
	      }
	    }
	}
	bucket_size -= bucket_d.size();
	bucket_d.clear();
      }

    } // end of method dt::run()


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_DT_CANVAS_HH
