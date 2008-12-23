#ifndef MARKOV_HH_
# define MARKOV_HH_

# include <cmath>
# include <iomanip>
# include <random.hh>
# include <approx_exp.hh>
# include <T_gen.hh>
# include <mln/binarization/threshold.hh>
# include <mln/core/routine/duplicate.hh>

namespace mln
{

  template <typename I, typename N>
  inline
  float compute_du(const I& ima, const I& out, const mln_site(I)& p, const N& nbh)
  {
    // Compute du : energy of clique with the new value minus with the
    // old value (p_out.val())

    bool old_val = out(p);
    // Compute u(x,y)
    float du = 0;
    if (old_val == ima(p))
      du += 1;
    else
      du -= 1;
    // u(x) is cst so we don't care

    // sum the differences between new_val and the neighboors.
    int diff_sum = 0;

    mln_niter(N) n(nbh, p);
    for_all(n)
      if (old_val != out(n))
	++diff_sum;

    du -= float(5 * (2 * diff_sum - int(nbh.size()))) / nbh.size();

    return du;
  }


  template <typename I>
  void dump(const Image<I>& ima)
  {
    static int id = 0;
    std::stringstream filename, is_filename;
    filename << "trace_" << std::setw(5) << std::setfill('0')
	     << std::right << id++ << ".pbm";

    io::pbm::save(ima, filename.str());
  }

  template <typename I, typename N> // I == int_u8
  mln_ch_value(I, bool) markov(const Image<I>& ima_, const Neighborhood<N>& nbh_, unsigned start_temp)
  {
    const I &ima = exact(ima_);
    const N &nbh = exact(nbh_);

    typedef mln_ch_value(I, bool) O;
    O bin = binarization::threshold(ima, 255 / 2); // FIXME : max
    O out(bin.domain());

    temperature_generator gtemp(start_temp, 0.8);
    approx_exp my_exp(-1000, 5, 100000);
    float temp = start_temp;

    Random<bool> v_random(0, 1); // mettre max et min ?
    Random<float> p_random(0., 1.); // idem

    unsigned modifications = 42;
    unsigned turn = 1;
    bool gradient = false;


    while (!gradient || modifications)
      {
	// Trace.
	//dump(out);

        modifications = 0;
	mln_piter(O) p_out(out.domain());
        for_all(p_out) if (v_random.get())
        {
	  float d_u = compute_du(bin, out, p_out, nbh);

	  if ((d_u < 0 || !gradient && (p_random.get() < my_exp(-d_u / temp))))
	  {
	    out(p_out) = !out(p_out);
 	    ++modifications;
	  }
	}
	temp = gtemp;

	std::cout << "Turn : " << turn << " Temp : " << temp << " Modifications : " << modifications << std::endl;
	turn ++;
	if (!gradient && !modifications)
	  {
	    std::cout << "Gradient !" << std::endl;
	    modifications = 1;
	    gradient = true;
	  }
      }

    return out;
  }

} // end of namespace mln

#endif /* !MARKOV_HH_ */
