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

#ifndef MLN_DT_CHAMFER_HH
# define MLN_DT_CHAMFER_HH

# include <queue>
# include <map>
# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/make/w_window.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/literal/zero.hh>

# include <iostream>

namespace mln
{

  namespace dt
  {

    /*! Propagation using a single neighborhood (PSN).
     *
     * \param[in]  input_   The input image.
     * \param[in]  nbh      The chamfer window to use for distance calcul.
     * \param[in]  max      Max value of the output image.
     * \return              A distance map.
     *
     * \pre \p img has to be initialized.
     */
    template<typename I, typename N>
    mln_ch_value(I, unsigned)
    psn(const Image<I>& input_, const N& nbh, unsigned max);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {
    } // end of namespace mln::dt::impl



    // Facade.

    template<typename I, typename N>
    inline
    mln_ch_value(I, unsigned)
    psn(const Image<I>& input_, const N& nbh, unsigned max)
    {
      // Preconditions.
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      // Types.
      typedef mln_point(I) point;

      // Initialization of output.
      mln_ch_value(I, unsigned) output;
      initialize(output, input);

      // Initialization.
      // {

      std::map<unsigned, std::queue<point> > bucket;
      unsigned bucket_size = 0;

      mln_fwd_piter(I) p(input.domain());
      for_all(p)
	if (input(p))
	{
	  output(p) = literal::zero;
	  bucket[0].push(p);
	  ++bucket_size;
	}
	else
	  output(p) = max;

      unsigned d = 0;

      // }

      while (bucket_size != 0)
      {
	std::queue<point> bucket_d = bucket[d];
	while (! bucket_d.empty())
	{
	  point p = bucket_d.front();
	  bucket_d.pop();
	  --bucket_size;

	  if (output(p) == d)
	  {
	    mln_qiter(N) n(nbh, p);

	    for_all(n)
	      if (output.has(n))
	      {
		unsigned newOut = output(p) + n.w();

		if (newOut < output(n))
		{
		  output(n) = newOut;
		  bucket[newOut].push(n);
		  ++bucket_size;
		}
	      }
	  }
	}
	bucket.erase(d);
	++d;
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_CHAMFER_HH

#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/data/fill.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/image/sub_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/value.hh>

int main()
{
  using namespace mln;

  image2d<bool> ima(5,5);
  bool vals[] = { 1, 0, 0, 1, 1,
		  0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0,
		  0, 0, 0, 1, 0,
		  0, 0, 0, 0, 0};
  data::fill(ima, vals);

  image2d<bool> msk(5,5);
  bool rest[] = { 1, 0, 1, 1, 1,
		  1, 0, 1, 1, 1,
		  1, 1, 0, 0, 0,
		  1, 1, 0, 1, 1,
		  1, 1, 1, 1, 1};
  data::fill(msk, rest);

  int ws[] = { 2, 1, 2,
	       1, 0, 1,
	       2, 1, 2 };
  image2d<unsigned> out;
  out = dt::psn(ima | pw::value(msk), make::w_window2d(ws), 50);

  debug::println(ima | pw::value(msk));
  debug::println(out);

//  image2d<bool> ima = io::pbm::load("../../img/c01.pbm");

//  image2d<value::int_u8> out2(out.domain());
//  level::stretch(out, out2);

//  io::pgm::save(out2, "out.pgm");
}
