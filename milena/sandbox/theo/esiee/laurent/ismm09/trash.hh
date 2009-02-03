
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
