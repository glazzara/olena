# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/site_set/p_queue_fast.hh>

# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/value/int_u8.hh>

# include <mln/level/fill.hh>

# include <mln/debug/println.hh>

namespace mln
{

  template <typename D, typename I, typename N>
  mln_ch_value(I, D)
    distance(D, const I& input, const N& nbh)
  {
    const D M = mln_max(D);
    mln_ch_value(I, D) output;
    initialize(output, input);

    mln_ch_value(I, bool) deja_vu;
    initialize(deja_vu, input);
    level::fill(deja_vu, input);

    typedef mln_site(I) P;
    p_queue_fast<P> q;

    // Initialization.
    {
      level::fill(output, M);
      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) == true) // p in object
	  {
	    output(p) = 0;
	    for_all(n)
	      if (input.domain().has(n) && input(n) == false) // n in background
		{
		  q.push(p);
		  break;
		}
	  }
    }

    // Propagation.
    {
      P p;
      mln_niter(N) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();
	  for_all(n)
	    if (input.domain().has(n) && output(n) == M)
	      {
		output(n) = output(p) + 1;
		if (output(n) == M)
		  {
		    // Saturation so stop.
		    q.clear();
		    break;
		  }
		q.push(n);
		deja_vu(n) = true;
	      }
	}
    }

    return output;
  }

*/



} // mln



  void usage(char* argv[])
  {
    std::cerr << "usage: " << argv[0] << " input.pbm output.pgm" << std::endl;
    abort();
  }



  int main(int argc, char* argv[])
  {
    using namespace mln;
    using value::int_u8;

    if (argc != 3)
      usage(argv);

    image2d<bool> input;
    io::pbm::load(input, argv[1]);

    image2d<int_u8> output = distance(int_u8(), input, c4());
    io::pgm::save(output, argv[2]);
  }
