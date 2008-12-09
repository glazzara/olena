#ifndef MARKOV_HH_
# define MARKOV_HH_

# include <cmath>
# include <iomanip>
# include <random.hh>
# include <T_gen.hh>
# include <mln/binarization/threshold.hh>
# include <mln/core/routine/clone.hh>

namespace mln
{

  template <typename I, typename O, typename N>
  double compute_energy(const I& ima, const O& out, const N& nbh, bool xi, const mln_site(I) &p)
  {
    // Compute u(x,y)
    double u;
    if (xi == ima(p))
      u = 0;
    else
      u = 1;

    // u(x) is cst so we don't care

    double diff_sum = 0;
    double coeff = 0;

    mln_niter(N) n(nbh, p);
    for_all(n)
      if (ima.domain().has(n))
	{
	  diff_sum += abs(xi - out(n));
	  coeff ++;
	}

    diff_sum /= coeff;

//     std::cout << "energy : " << (u + diff_sum) << std::endl;

    return (u + diff_sum * 5);
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

    mln_ch_value(I, bool) bin = binarization::threshold(ima, 255 / 2); // FIXME : max
    mln_ch_value(I, bool) out(bin.domain());

    io::pbm::save(out, "threshold.pbm");

    temperature_generator gtemp(start_temp, 0.8);
    double temp = start_temp;

    Random<bool> v_random(0, 1); // mettre max et min ?
    Random<double> p_random(0., 1.); // idem

    unsigned modifications = 42;
    unsigned turn = 1;
    bool gradient = false;
    int diffneg = 0;

    while (!gradient || modifications)
      {
	// Trace.
// 	dump(out);

        mln_piter(I) p(bin.domain());
        modifications = 0;

        for_all(p)
        {
          bool v = v_random.get();

          double u = compute_energy(bin, out, nbh, out(p), p);
          double up = compute_energy(bin, out, nbh, v, p);

	  double d_u = up - u;
	  double proba = exp(-d_u / temp);

	  if ((d_u < 0 || !gradient && (p_random.get() < proba)) && out(p) != v)
	    {
	      if (d_u < 0)
		diffneg ++;
	      out(p) = v;
	      modifications ++;
	    }
	}
	temp = gtemp;
	std::cout << "Turn : " << turn << " Modifs : " << modifications << " DiffNeg : " << diffneg << " Temp : " << temp << std::endl;
	turn ++;
	if (!gradient && !modifications)
	  {
	    std::cout << "Gradient !" << std::endl;
	    modifications = 1;
	    gradient = true;
	  }
	diffneg = 0;
      }

    return out;
  }

} // end of namespace mln

#endif /* !MARKOV_HH_ */
