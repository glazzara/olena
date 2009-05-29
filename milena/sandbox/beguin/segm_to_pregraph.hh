#include <mln/value/int_u8.hh>

mln::value::int_u8 foo(unsigned u);

namespace mln

{
//  using value::int_u8;
  template <typename N>
  void segm_to_pregraph(const image2d<unsigned>& lbl, 
                        const N& nbh, 
                        int nbasins)
  {
    mln_piter(image2d<unsigned>) p(lbl.domain());
    mln_niter(N) n(nbh, p);

//    typedef metal::vec<3,mln_value(I)> V;

//    typedef Image<V> J;

    image2d<unsigned> retour;
    initialize(retour, lbl);

    int n_seuil = ((nbasins/10)+1)*10;

    std::cout << "Nbasins = " << nbasins << std::endl
              << "n_seuil = " << n_seuil << std::endl;

    for_all(p)
    {
      if(lbl(p) == 0)
      {
        std::set<unsigned> s;  //Set of neighbouring values of p
        for_all(n) if (lbl.has(n))	//Pixel processing
        {
          if (lbl(n) == 0)
            continue;
          s.insert(lbl(n));
        }
        if (s.size() < 2 )	// Shouldn't happen
          std::cout << "#" << std::endl;
        if (s.size() == 2 ) 	// Case: Boundary between two regions
        {
          std::set<unsigned>::const_iterator l1, l2;
          l1 = s.begin();
          l2 = s.begin();
          l2++;

          retour(p) = *l1 * n_seuil + *l2;

        }
        if (s.size() > 2 )	// Case: Node or part of a node
          retour(p) = 0;
      }
      else
      {
       // retour(p) = make::vec(0,0,lbl(p));
        retour(p) = lbl(p);
      }
    }
    //io::pgm::save(retour, "tmp_pregraphe.pgm");
    io::pgm::save( data::transform(retour, convert::to_fun(foo)),
		 "tmp_pregraph.pgm" );
    //return retour;
  }
}
