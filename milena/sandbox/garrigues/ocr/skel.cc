# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>

# include <mln/io/pbm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/value/int_u8.hh>

# include <mln/level/fill.hh>

# include <mln/debug/println.hh>

# include "simple_point.hh"


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

  image2d<bool>
    skel(const image2d<bool>& input, unsigned nbh_i)
  {
    neighb2d nbh = int_to_neighb(nbh_i);
    image2d<bool> output;
    initialize(output, input);

    image2d<value::int_u16> dist_map = distance(value::int_u16(), input, nbh);

    typedef mln_site_(image2d<bool>) P;
    p_priority<value::int_u16, p_queue_fast<P> > q;

    // Initialization.
    {
      level::fill(output, input);
      mln_piter_(image2d<bool>) p(output.domain());
      for_all(p)
	if (output(p) &&
	    is_simple_point2d(output, nbh_i, p) && // p is a not simple point of object
	    !is_curve_extremum(output, nbh_i, p))
	{
	  output(p) = false; // Remove p from object
	  q.push(dist_map(p), p);
	}
    }

    // Propagation.
    {
      P p;
      mln_niter_(neighb2d) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();

	  for_all(n)
	    if (output.domain().has(n) &&
		output(n) &&
		is_simple_point2d(output, nbh_i, n) && // n is not simple
		!is_curve_extremum(output, nbh_i, n))
	    {
	      output(n) = false; // Remove p from object
	      q.push(dist_map(n), n);
	    }
	}
    }

    return output;
  }

} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pbm" << std::endl;
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

  image2d<bool> output = skel(input, 4);
  io::pbm::save(output, argv[2]);
}
