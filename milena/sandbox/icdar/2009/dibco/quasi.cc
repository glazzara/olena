#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/core/site_set/p_queue.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/fill.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pbm/save.hh>

#include <mln/labeling/colorize.hh>
#include <mln/fun/i2v/array.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/wrap.hh>

#include <mln/level/transform.hh>
#include <mln/level/convert.hh>

#include <mln/math/diff_abs.hh>





namespace mln
{

  float
  sim(unsigned v1, unsigned v2)
  {
    if (v1 == 0 && v2 == 0)
      return 1.f;
    return  v1 < v2  ?  float(v1) / float(v2)  :  float(v2) / float(v1); // min / max
  }


  template <typename I, typename N, typename L>
  mln_ch_value(I, bool)
    labeling__quasi_regional_minima(const Image<I>& input_, const Neighborhood<N>& nbh_,
				    unsigned threshold,
				    L& cur_lab)
  {
    trace::entering("labeling__quasi_regional_minima");
    
    const I& input = exact(input_);
    const N& nbh = exact(nbh_);
    mln_precondition(input.is_valid());
    
    typedef mln_psite(I) P;
    p_array<P> s = level::sort_psites_increasing(input);

    std::vector<bool> valid;
    valid.push_back(false); // For valid[0] where 0 is the non-label value.

    mln_ch_value(I, L) lab;
    initialize(lab, input);
    data::fill(lab, 0);

    cur_lab = 0;
    unsigned
      n_stop_unknown,
      n_stop_object,
      n_stop_background;

    p_queue<P> q;
    P p_;
    mln_niter(N) n(nbh, p_);

    mln_piter(p_array<P>) p(s);
    for_all(p)
    {
      if (lab(p) != 0)
	continue;

      mln_invariant(q.is_empty());

      // Initialization.
      ++cur_lab;
      n_stop_unknown = 0;
      n_stop_object = 0;
      n_stop_background = 0;

      mln_value(I) input_p = input(p);
      lab(p) = cur_lab;
      q.push(p);

      while (! q.is_empty())
	{
	  p_ = q.pop_front();
	  mln_invariant(lab(p_) != 0); // Already seen.
	  for_all(n) if (input.domain().has(n))
	  {
	    if (lab(n) == cur_lab)
	      continue;
	    if (lab(n) == 0)
	      {
// 		if (sim(input(n), input(p_)) > 0.7
//  		    &&
// 		    sim(input(n), input_p) > 0.8)
		if (math::diff_abs(input(n), input(p_)) < threshold
 		    &&
		    math::diff_abs(input(n), input_p) < 2 * threshold)
		  {
		    lab(n) = cur_lab;
		    q.push(n);
		  }
		else
		  ++n_stop_unknown; // Stop because of threshold.
		continue;
	      }
 	    mln_invariant(lab(n) != 0 && lab(n) != cur_lab); // Another component.

	    if (valid[lab(n)])
	      ++n_stop_object;
	    else
	      ++n_stop_background;
	  }
	}

      if (n_stop_object + n_stop_background <= n_stop_unknown / 4)
	{
	  valid.push_back(true);
	  continue;
	}
      valid.push_back(false);
    }

    fun::i2v::array<bool> f_valid;
    convert::from_to(valid, f_valid);

    mln_ch_value(I, bool) out = level::transform(lab, f_valid);

    io::pgm::save(labeling::wrap(value::int_u8(), lab),
		  "tmp_lab.pgm");

    trace::exiting("labeling__quasi_regional_minima");
    return out;
  }


} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace value;

  // trace::quiet = false;

  if (argc != 4)
    {
      std::cout << argv[0] << " input.pgm threshold output.pbm" << std::endl;
      return 1;
    }

  int threshold = std::atoi(argv[2]);


  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  unsigned n_regions;
  image2d<bool> output = labeling__quasi_regional_minima(input,
							 c4(),
							 threshold,
							 n_regions);

  std::cout << n_regions << std::endl;

  io::pbm::save(output, argv[3]);
}
