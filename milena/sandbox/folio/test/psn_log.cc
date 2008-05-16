
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

# include <vector>
# include <queue>
# include <map>
# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/literal/zero.hh>

#include <mln/core/image2d.hh>
# include <mln/debug/println.hh>

namespace mln
{

  namespace dt
  {

    /*! Propagation using a single neighborhood (PSN).
     *
     * \param[in]  input_   The input image.
     * \param[in]  chamfer  The chamfer window to use for distance calcul.
     * \return              A pair <distance map, closest point map>.
     *
     * \pre \p img has to be initialized.
     */
    template<typename I, typename N>
    mln_ch_value(I, unsigned)
    psn(const Image<I>& input_, const N& nbh);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename BP>
      class compare
      {
      public:
	bool
	operator()(const BP& lhs, const BP& rhs) const
	{
	  return lhs.second > rhs.second;
	}
      };

      template <typename D>
      unsigned
      sq(const D& dp)
      {
	unsigned res = 0;

	for (unsigned i = 0; i < D::dim; ++i)
	  res += std::abs(dp[i]); // FIXME: dp[i] * dp[i];

	return res;
      }

      template <typename BP>
      unsigned
      size(const std::map<unsigned, std::queue<BP> >& bucket,
	   int d)
      {
	unsigned s = 0;
	typename std::map<unsigned, std::queue<BP> >::const_iterator i;
	for (i = bucket.begin(); i != bucket.end(); ++i)
	  if (i->first >= d)
	    s += i->second.size();
	return s;
      }

      template <typename BP>
      unsigned
      size(const std::map<unsigned, std::queue<BP> >& bucket)
      {
	unsigned s = 0;
	typename std::map<unsigned, std::queue<BP> >::const_iterator i;
	for (i = bucket.begin(); i != bucket.end(); ++i)
	  s += i->second.size();
	return s;
      }

      template <typename BP>
      void
      print(const std::map<unsigned, std::queue<BP> >& bucket)
      {
	typename std::map<unsigned, std::queue<BP> >::const_iterator i;
	int d = -1;
	for (i = bucket.begin(); i != bucket.end(); ++i)
	  {
	    if (i->first != d)
	      {
		d = i->first;
		std::cout << std::endl << d << ": ";
	      }
	    std::queue<BP> qu = i->second; // copy
	    std::vector<BP> v;
	    v.push_back(qu.front()); qu.pop();
	    typename std::vector<BP>::const_iterator j;
	    for (j = v.begin(); j != v.end(); ++j)
	      std::cout << j->first << " "; // point
	  }
	std::cout << std::endl;
      }

    } // end of namespace mln::dt::impl



    // Facade.

    template<typename I, typename N>
    inline
    mln_ch_value(I, unsigned)
    psn(const Image<I>& input_, const N& nbh)
    {
      const I& input = exact(input_);
      mln_precondition(input.has_data());

      mln_ch_value(I, unsigned) D;
      initialize(D, input);

      static const unsigned M = 666; // FIXME

      // Initialization.
      typedef mln_point(I) point;
      typedef mln_dpoint(I) dpoint;
      typedef std::pair<point, dpoint> BP;

      std::map<unsigned, std::queue<BP> > bucket;
      unsigned bucket_size = 0;

      mln_fwd_piter(I) p(input.domain());
      for_all(p)
	if (input(p))
	{
	  D(p) = literal::zero;
	  bucket[0].push(BP(p, literal::zero));
	  ++bucket_size;
	}
	else
	  D(p) = M;

      debug::println(input);
      debug::println(D);
      impl::print(bucket);

      unsigned d = 0;

      while (impl::size(bucket, d) != 0)
      {

	mln_invariant(impl::size(bucket) == bucket_size);

	std::cout << "PROCESSING  d = " << d << std::endl;
	  
	std::queue<BP>& bucket_d = bucket[d];

	while (! bucket_d.empty())
	{
	  point p = bucket_d.front().first;
	  dpoint dp = bucket_d.front().second;

	  bucket_d.pop();
	  --bucket_size;

	  std::cout << "pop " << p << " at D=" << D(p) << std::endl;

	  if (D(p) == d)
	  {
	    mln_niter(N) n_(nbh, p);

	    for_all(n_)
	      if (D.has(n_))
	      {
		dpoint n = n_ - p;
		unsigned newD = impl::sq(dp + n);

		std::cout << "  n=" << n_ << " at D=" << D(n_)
			  << " and newD=" << newD
			  << (newD < D(p + n) ? "   push" : "")
			  << std::endl;

		if (newD < D(p + n)) // p + n = p + n_ - p = n_
		{
		  D(n_) = newD;
		  bucket[newD].push(BP(p + n, dp + n));
		  ++bucket_size;
		}
	      }
	  }
	}
	++d;
      }

      return D;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_CHAMFER_HH

#include <iostream>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/level/fill.hh>
#include <mln/core/neighb2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/sub_image.hh>
#include <mln/core/image_if.hh>
#include <mln/pw/value.hh>

int main()
{
  using namespace mln;

//   image2d<bool> ima(5,5);
//   bool vals[] = { 1, 1, 1, 0, 0,
// 		  0, 0, 1, 0, 0,
// 		  0, 0, 1, 0, 0,
// 		  0, 0, 0, 0, 0,
// 		  0, 0, 0, 0, 0 };

  image2d<bool> ima(3,3);
  bool vals[] = { 1, 0, 0,
		  0, 0, 0,
		  0, 0, 0};
  level::fill(ima, vals);

  image2d<bool> msk(3,3);
  bool rest[] = { 1, 0, 1,
		  1, 0, 1,
		  1, 1, 1};
  level::fill(msk, rest);

  image2d<unsigned> out;
  out = dt::psn(ima | pw::value(msk), c4());

  debug::println(ima | pw::value(msk));
  debug::println(out);

//  image2d<bool> ima = io::pbm::load("../../img/c01.pbm");

//  image2d<value::int_u8> out2(out.domain());
//  level::stretch(out, out2);

//  io::pgm::save(out2, "out.pgm");
}
