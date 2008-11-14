#ifndef MARKOV_HH_
# define MARKOV_HH_

# include <cmath>


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

double compute_energy(const Image2d<int_u8>& ima, bool xi, const point2d& p)
{
  // Compute u(x,y)
  double u;
  if (v)
    u = ima(p) / 255;
  else
    u = 1 - ima(p) / 255;

  // u(x) is cst donc osef

  // u voisinage
}

  template <typename I, typename G, typename R, typename E> // I == int_u8
markov(const Image2d<I>& ima, unsigned start_temp)
{
  double espilon = 0.001;
  Image2d<bool> out(ima.domain()); // FIXME: generalize, we might not do a binarisation
  G temp(start_temp);

  R v_random(0, 1);
  R p_random(0., 1.);

  init(ima, out);

  while (temp.value() < epsilon)
  {
    qiterator p(ima);

    for_all(p)
    {
      bool v = v_random.get();


      u = compute_energy(ima, out(p), p);
      up = compute_energy(ima, v, p);

      d_u = abs(up - u);

      proba = ...;

      if (d_u < 0 or (p_random.get() > proba))
	out(p) = v;
    }
  }

  return out;
}

#endif /* !MARKOV_HH_ */
