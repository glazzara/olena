#ifndef MARKOV_HH_
# define MARKOV_HH_

# include <cmath>
# include <random.hh>
# include <T_gen.hh>
# include <mln/binarization/threshold.hh>

namespace mln
{

  template <typename I, typename O, typename N>
  double compute_energy(const I& ima, const O& out, const N& nbh, bool xi, const mln_site(I) &p)
  {
    // Compute u(x,y)
    double u;
    if (xi)
      u = (double) ima(p) / mln_max(mln_value(I));
    else
      u = 1. - (double) ima(p) / mln_max(mln_value(I));

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

    return 1 * (u + diff_sum);
  }

  template <typename I, typename N> // I == int_u8
  mln_ch_value(I, bool) markov(const Image<I>& ima_, const Neighborhood<N>& nbh_, unsigned start_temp)
  {
    const I &ima = exact(ima_);
    const N &nbh = exact(nbh_);

    //    mln_ch_value(I, bool) out(ima.domain()); // FIXME: generalize, we might not do a binarisation
    mln_ch_value(I, bool) out = binarization::threshold(ima, 255 / 2); // FIXME : max

    temperature_generator gtemp(start_temp, 0.8);
    double temp = start_temp;

    Random<int> v_random(0, 1); // mettre max et min ?
    Random<double> p_random(0., 1.); // idem

    unsigned modifications = 42;
    unsigned turn = 1;
    bool gradient = false;
    int diffneg = 0;

    while (!gradient || modifications)
      {
        mln_piter(I) p(ima.domain());
        modifications = 0;

        for_all(p)
        {
          bool v = v_random.get();
	  // std::cout << "Random : " << v << std::endl;

          double u = compute_energy(ima, out, nbh, out(p), p);
          double up = compute_energy(ima, out, nbh, v, p);

	  double d_u = up - u;
	  double proba = exp(-d_u / temp);

	  // std::cout << "u : " << u << " up : " << up << "Difference : " << d_u << std::endl;

	  if (d_u < 0 || !gradient && (p_random.get() < proba))
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
