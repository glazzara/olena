/* core */
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>

/* io */
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

/* Tree computation */
#include <mln/data/sort_psites.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/morpho/tree/propagate.hh>
#include <mln/morpho/tree/components.hh>
#include <mln/morpho/tree/filter/filter.hh>

/* Attribute */
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/mysharpness.hh>

#include <mln/morpho/reconstruction/by_erosion/union_find.hh>

/* Functions */
#include <mln/pw/all.hh>
#include <mln/math/diff_abs.hh>

#include <iostream>
#include <cstdlib>

namespace mln
{
  // A sensible height definition for a sharp component:
  // h(n) = hc(parent(n)) - hb(n)
  // where hc = height current level
  //	   hb = height base level


  // Sharpness Attribute -> Height Attribute
//   template <typename I, typename T>
//   struct height_wrapper_ : Function_p2v< height_wrapper_<I, T> >
//   {
//     typedef unsigned result;
//     typedef typename T::function F;

//     height_wrapper_(const Image<I>& f,
// 		    const T& tree) :
//       f_ (exact(f)),
//       tree_ (tree)
//     {
//       mlc_is(mln_value(I),
// 	     morpho::attribute::height<F>)::check();
//     }

//     unsigned operator() (const mln_psite(T)& p) const
//     {
//       unsigned l =
// 	math::diff_abs(f_(tree_.parent(p)).current_level(),
// 		       f_(p).base_level());
//       mln_assertion(l >= 0);
//       return l;
//     }

//   private:
//     const I& f_;
//     const T& tree_;
//   };

//   // meta
//   template <typename I, typename T>
//   inline
//   height_wrapper_<I, T>
//   height_wrapper(const Image<I>& f, const T& tree)
//   {
//     return height_wrapper_<I, T>(f, tree);
//   }
}



void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.pgm height [reconstructed.pgm]"
	    << std::endl
	    << "\t Return the input image where sharpest component are black, "
	    << "and there's no more regional minima (-> WST)."
	    << std::endl;
  abort();
}

int main(int argc, char** argv)
{
 if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const unsigned lambda_h = atoi(argv[2]);
  const char* foutput = (argc == 4) ? argv[3] : "reconstructed.pgm";

  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  I input;
  io::pgm::load(input, finput);

  /***************************/
  /* Component tree creation */
  /***************************/
  typedef p_array<point2d> S;
  typedef morpho::tree::data<I,S> tree_t;

  S s = data::sort_psites_decreasing(input);
  tree_t t(input, s, c4());


  /******************************/
  /* Compute Attribute On Image */
  /******************************/
  typedef mln_ch_value_(I, double) A;
  A a = morpho::attribute::mysharpness(t, lambda_h);


  /************************************************/
  /* Retrieve Components (Maximising the criteria)*/
  /************************************************/
  p_array< mln_psite_(A) > obj_array;
  {
    obj_array = morpho::tree::get_components(t, a);
    std::cout << "Components: " << obj_array.nsites() << std::endl;
  }

  {
    I reconstructed;
    I marker = set_value_to_components(t, obj_array, (int_u8) 0, (int_u8) 255);
    set_value_to_components(t, input, obj_array, (int_u8) 0);

    // input is the mask.
    reconstructed = morpho::reconstruction::by_erosion::union_find(marker, input, c4());

    io::pgm::save(reconstructed, foutput);
  }
}
