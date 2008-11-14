#ifndef MARKOV_HH_
# define MARKOV_HH_

# include <cmath>
# include <random.hh>
# include <T_gen.hh>

namespace mln
{

  /* I think we don't need it anymore...
  inline
  const neighb2d& neighb_h1()
  {
    static neighb2d it;
    if (it.size() == 0)
      {
	static const bool vals[] = { 0, 0, 0,
				     0, 0, 1,
				     0, 0, 0 };
	convert::from_to(vals, it);
      }
    return it;
  }
  */

  template <typename I, typename N>
  double compute_energy(const I& ima, const N& nbh, bool xi, const mln_piter(I)& p)
  {
    // Compute u(x,y)
    double u;
    if (xi)
      u = (double) ima(p) / mln_max(mln_value(I));
    else
      u = (double) (1 - ima(p)) / mln_max(mln_value(I));

    // u(x) is cst donc osef

    // u voisinage

    mln_niter(N) n(nbh, p);
    for_all(n)
      // treat each point here ;), no need to make weird neighborhoods
      // make sth with |ima(p) - ima(n)|
      abs(ima(p) - ima(n));
  }

  template <typename I, typename N> // I == int_u8
  mln_ch_value(I, bool) markov(const Image<I>& ima_, const Neighborhood<N> nbh_, unsigned start_temp)
  {
    const I &ima = exact(ima_);
    const N &nbh = exact(nbh_);

    double epsilon = 0.001;
    mln_ch_value(I, bool) out(ima.domain()); // FIXME: generalize, we might not do a binarisation
    // G temp(start_temp);
    temperature_generator temp(start_temp, 0.99);

    Random<bool> v_random(0, 1); // mettre max et min ?
    Random<double> p_random(0., 1.); // idem

    // init(ima, out); ca empeche de compiloter

    while (temp < epsilon)
      {
	mln_piter(I) p(ima.domain());

	for_all(p)
	{
	  bool v = v_random.get();

	  double u = compute_energy(ima, nbh, out(p), p);
	  double up = compute_energy(ima, nbh, v, p);

	  double d_u = abs(up - u);

	  double proba = 0.5; // FIXME

	  if (d_u < 0 or (p_random.get() > proba))
	    out(p) = v;
	}
      }

    return out;
  }

} // end of namespace mln

#endif /* !MARKOV_HH_ */
