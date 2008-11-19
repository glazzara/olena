#ifndef MLN_REGISTRATION_FINAL_QK_HH
# define MLN_REGISTRATION_FINAL_QK_HH

namespace mln
{

  namespace registration
  {

    template <typename P, typename M>
    void mean_stddev(const p_array<P>& c,
		     const M&          map,
		     float& mean,
		     float& stddev)
    {
      //mean + length
      std::vector<float> length(c.nsites());
      mean = 0;

      for (unsigned i = 0; i < c.nsites(); i++)
	{
	  float f = norm::l2(convert::to< algebra::vec<P::dim,int> > (c[i] - map(c[i])));
	  length[i] = f;
	  mean += f;
	}
      mean /= c.nsites();

      //standar variation
      stddev = 0;
      for (unsigned i = 0; i < c.nsites(); i++)
	stddev += (length[i] - mean) * (length[i] - mean);
      stddev /= c.nsites();
      stddev = math::sqrt(stddev);
    }


    //final qk : only use point less than nstddev (2*stddev);
    template <typename P, typename M>
    quat7<P::dim>
    final_qk(const p_array<P>& c,
	     const M&    map,
	     float       nstddev)
    {
      p_array<P> newc;
      algebra::vec<P::dim,float> mu_newc(literal::zero);

      for (unsigned i = 0; i < c.nsites(); ++i)
	{
          algebra::vec<P::dim,float> ci = c[i];
	  algebra::vec<P::dim,float> xki = map(c[i]);

	  if (norm::l2(ci - xki) < nstddev)
	    {
	      newc.append(c[i]);
	      mu_newc += ci;
	    }
	}
      if (newc.nsites() == 0)
        return quat7<P::dim>();
      mu_newc /= newc.nsites();

      quat7<P::dim> qk = match(newc, mu_newc, newc, map, newc.nsites());

      //qk._qT = - qk._qT; // FIXME : why?

      return qk;
    }

    //final > nstddev for translation; < nstddev for rotation
    template <typename P, typename M>
    quat7<P::dim>
    final_qk2(const p_array<P>& c,
	      const M&    map,
	      float       nstddev)
    {
      //mu_Xk = center map(Ck)
      algebra::vec<P::dim,float> mu_Xk(literal::zero);
      algebra::vec<P::dim,float> mu_C(literal::zero);

      float nb_point = 0;
      for (unsigned i = 0; i < c.nsites(); ++i)
        {
          algebra::vec<P::dim,float> xki = map(c[i]);
          algebra::vec<P::dim,float> ci = c[i];

          if (norm::l2(ci - xki) > nstddev)
            {
              mu_C += ci;
              mu_Xk += xki;
              nb_point++;
            }
        }
      mu_C  /= nb_point;
      mu_Xk /= nb_point;

      // qT
      const algebra::vec<P::dim,float> qT = mu_C - mu_Xk;

      // qR
      quat7<P::dim> qk = final_qk(c, map, nstddev);
      qk._qT = qT;

      return qk;
    }


  } // end of namespace mln::registration

}

#endif // MLN_REGISTRATION_FINAL_QK_HH
