
namespace mln
{


  template <typename F, typename N, typename A, typename W>
  void // util::array<unsigned>
  compute_attribute_extincted(const F& f, const N& nbh, const A& a,
			      const W& w)
  {
    typedef value::label_16 L;
    L n_basins;
    mln_ch_value(F,L) regmins = labeling::regional_minima(f, nbh, n_basins);
    
    typedef p_array<mln_psite(F)> S;
    S s = level::sort_psites_decreasing(f);
    
    typedef morpho::tree::data<F,S> tree_t;
    tree_t t(f, s, nbh);
    mln_VAR(a_ima, morpho::tree::compute_attribute_image(a, t));

    std::cout << "BEFORE:" << std::endl;
    debug::println("a_ima:", a_ima);
    debug::println("a_ima | w_line:", a_ima | (pw::value(w) == 0));
    debug::println("a_ima | basins:", a_ima | (pw::value(w) != 0));
    // debug::println("a_ima | regmins:", a_ima | (pw::value(regmins) != 0));


    extinct_attributes(t, a_ima);

    std::cout << "AFTER:" << std::endl;
    debug::println("a_ima:", a_ima);
    debug::println("a_ima | w_line:", a_ima | (pw::value(w) == 0));
    debug::println("a_ima | basins:", a_ima | (pw::value(w) != 0));
    debug::println("a_ima | regmins:", a_ima | (pw::value(regmins) != 0));
  }


    inline
    point2d p1_from_e(const point2d& e)
    {
      return e + (is_row_odd(e) ? up : left);
    }
    
    inline
    point2d p2_from_e(const point2d& e)
    {
      return e + (is_row_odd(e) ? down : right);
    }



    struct e_to_labels_t 
    {
      template <typename W, typename L>
      inline
      void
      operator()(const W& w, const point2d& e, L& l1, L& l2) const
      {
	mln_precondition(w(e) == 0);
	l1 = 0;
	l2 = 0;
	mln_niter(dbl_neighb2d) n(e2e(), e);
	for_all(n)
	  if (w.has(n) && w(n) != 0)
	    {
	      if (l1 == 0) // First label to be stored.
		l1 = w(n);
	      else
		if (w(n) != l1 && l2 == 0) // Second label to be stored.
		  l2 = w(n);
		else
		  mln_invariant(w(n) == l1 || w(n) == l2);
	    }
	mln_invariant(l1 != 0 && l2 != 0);
	if (l1 > l2)
	  std::swap(l1, l2);
	mln_postcondition(l2 >= l1);
      }
    };


} // end of namespace mln




// Trash code:


//   {
//     L n_regmins;
//     mln_VAR(regmins, labeling::regional_minima(g, cplx2d::e2e(), n_regmins));
//     mln_invariant(n_regmins == n_basins);
//     debug::println("regmins(g):", regmins);

//     debug::println("w:", w);
//     std::cout << "n basins = " << n_basins << std::endl
// 	      << std::endl;
//   }

//   // accu::count< util::pix<g_t> > a_;
//   accu::height<g_t> a_;

//   compute_attribute_extincted(g, cplx2d::e2e(), a_,
// 			      w);
