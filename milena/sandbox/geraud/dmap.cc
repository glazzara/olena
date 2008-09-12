#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/debug/println.hh>
#include <mln/level/fill.hh>
#include <mln/level/stretch.hh>
#include <mln/accu/max.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>


namespace mln
{

  template<typename I, typename W>
  mln_ch_value(I, unsigned) dmap(const Image<I>& input_,
				 const Weighted_Window<W>& win_)
  {
    const I& input = exact(input_);
    const W& win = exact(win_);
    mln_precondition(input.has_data());

    typedef std::vector<mln_point(I)> bucket_t;

    // Initialization of distance.
    mln_ch_value(I, unsigned) distance;
    initialize(distance, input);

    // Mod determination.
    mln_accu_with_(accu::max, unsigned) max;
    for (unsigned i = 0; i < win.size(); ++i)
      max.take(win.w(i));
    unsigned mod = unsigned(max) + 1;

    // Aux data.
    std::vector<bucket_t> bucket(mod);
    unsigned bucket_size = 0;

    // Initialization.
    {
      mln_piter(I) p(input.domain());
      mln_qiter(W) q(win, p);
      for_all(p)
	if (input(p))
	  {
	    distance(p) = literal::zero;
	    for_all(q)
	      if (input.has(q) && ! input(q))
		{
		  bucket[0].push_back(p);
		  ++bucket_size;
		  break;
		}
	  }
	else
	  distance(p) = mln_max(unsigned);
    }

    // Propagation.
    {
      mln_point(I) p;
      mln_qiter(W) q(win, p);

      for (unsigned d = 0; bucket_size != 0; ++d)
	{
	  bucket_t& bucket_d = bucket[d % mod];
 	  for (unsigned i = 0; i < bucket_d.size(); ++i)
	    {
	      p = bucket_d[i];

	      if (distance(p) < d)
		// p has already been processed, having a distance less than d.
		continue;

	      for_all(q)
		if (distance.has(q) && distance(q) > d)
		  {
		    unsigned d_ = d + q.w();

		    if (d_ < distance(q))
		      {
			distance(q) = d_;
			bucket[d_ % mod].push_back(q);
			++bucket_size;
		      }
		  }
	    }
	  bucket_size -= bucket_d.size();
	  bucket_d.clear();
	}

    } // end of propagation.

    return distance;
  }

} // end of namespace mln



int main()
{
  using namespace mln;
  using value::int_u8;

  const unsigned n = 256;
  image2d<bool> ima(n, n);
  level::fill(ima, false);
  ima.at(n / 2, n / 2) = true;

  int ws[] = { 3, 2, 3,
	       2, 0, 2,
	       3, 2, 3 };

//   int ws[] = { 0, 9, 0, 9, 0,
// 	       9, 6, 4, 6, 9,
// 	       0, 4, 0, 4, 0,
// 	       9, 6, 4, 6, 9,
// 	       0, 9, 0, 9, 0 }; // coef = 4.1203

  w_window2d_int win = make::w_window2d(ws);

  image2d<unsigned> d = dmap(ima, win);

  image2d<int_u8> out(d.domain());
  level::stretch(d, out);
  io::pgm::save(out, "out.pgm");
}
