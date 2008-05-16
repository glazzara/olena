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
      std::vector<float> length(c.npoints());
      mean = 0;

      for (size_t i = 0; i < c.npoints(); i++)
	{
	  float f = norm::l2(algebra::vec<3,int> (c[i] - map(c[i])));
	  length[i] = f;
	  mean += f;
	}
      mean /= c.npoints();
  
      //standar variation
      stddev = 0;
      for (size_t i = 0; i < c.npoints(); i++)
	stddev += (length[i] - mean) * (length[i] - mean);
      stddev /= c.npoints();
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
      algebra::vec<3,float> mu_newc(literal::zero);

      for (size_t i = 0; i < c.npoints(); ++i)
	{
	  algebra::vec<3,float> xki = map(c[i]);
	  algebra::vec<3,float> ci = c[i];

	  if (norm::l2(ci - xki) > nstddev)
	    {
	      newc.append(c[i]);
	      mu_newc += ci;
	    }
	}
      mu_newc /= newc.npoints();

      quat7<P::dim> qk = match(newc, mu_newc, newc, map, newc.npoints());

      qk._qT = - qk._qT; // FIXME : why?

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
      algebra::vec<3,float> mu_Xk(literal::zero);
      algebra::vec<3,float> mu_C(literal::zero);
      
      float nb_point = 0;
      for (size_t i = 0; i < c.npoints(); ++i)
	{
	  algebra::vec<3,float> xki = map(c[i]);
	  algebra::vec<3,float> ci = c[i];
	  
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
      const algebra::vec<3,float> qT = mu_C - mu_Xk;

      // qR
      quat7<P::dim> qk = final_qk(c, map, nstddev);
      qk._qT = qT;

      return qk;
    }
    
    
  } // end of namespace mln::registration

}

#endif // MLN_REGISTRATION_FINAL_QK_HH
