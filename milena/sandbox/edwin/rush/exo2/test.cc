#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/morpho/attribute/mysharpness.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/data/sort_psites.hh>
#include <mln/debug/println.hh>

#include <mln/convert/all.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/value/int_u8.hh>

#include <mln/morpho/tree/components.hh>
#include <mln/morpho/tree/propagate.hh>

#include <mln/fun/p2v/ternary.hh>
#include <mln/pw/all.hh>


namespace mln
{
  // Sharpness Attribute -> Height Attribute
  template <typename P2V>
  struct height_wrapper_ : Function_v2v< height_wrapper_<P2V> >
  {
    typedef unsigned result;

    height_wrapper_(const Function_v2v<P2V>& f) :
      f_ (exact(f))
    {
      mlc_is_a(mln_result(P2V), Accumulator)::check();
    }

    template <typename P>
    unsigned operator() (const P& p) const
    {
      return f_(p).height();
    }

  private:
    const P2V& f_;
  };

  // meta
  template <typename P2V>
  inline
  height_wrapper_<P2V>
  height_wrapper(const Function_v2v<P2V>& f)
  {
    return height_wrapper_<P2V>(f);
  }
}



int main()
{
  using namespace mln;
  using value::int_u8;


  const char arr[]= { 16, 0, 0, 0, 0, 15, 0, 14, 0, 0, 0, 16 };


  typedef image1d<int_u8> I;
  I input;
  convert::from_to(arr, input);


    /***************************/
  /* Component tree creation */
  /***************************/
  typedef p_array<point1d> S;
  typedef morpho::tree::data<I,S> tree_t;

  S s = data::sort_psites_decreasing(input);
  tree_t t(input, s, c2());


  /******************************/
  /* Compute Attribute On Image */
  /******************************/
  {
    typedef mln_ch_value_(I, unsigned) A;
    typedef morpho::attribute::volume<I> attribute_t;
    A a;

    // Attribute Pruning
    a = morpho::tree::compute_attribute_image(attribute_t (), t);
    debug::println("volume", a);
  }

  mln_ch_value_(I, unsigned) height;
  {
    typedef mln_ch_value_(I, unsigned) A;
    typedef morpho::attribute::height<I> attribute_t;

    // Attribute Pruning
    height = morpho::tree::compute_attribute_image(attribute_t (), t);
    debug::println("height", height);
  }

  {
    typedef mln_ch_value_(I, double) A;
    A a, component_img;

    // Attribute Pruning
    a = morpho::attribute::mysharpness(t);
    morpho::tree::propagate_representative(t, a);
    debug::println("sharpness", a);

    // Component filtering
    // a = duplicate((fun::p2v::ternary(pw::value(height) > pw::cst(2),
// 				   pw::value(a),
// 				   pw::cst(0.0))) | a.domain());

    //debug::println("sharpness", a);


//     p_array< mln_psite_(A) > obj_array;
//     obj_array = morpho::tree::get_components(t, a);
//     std::cout << obj_array.nsites() << std::endl;

//     component_img = morpho::tree::propagate_components(a, t, obj_array, 0);

//     debug::println("sharpness", component_img);
  }


}
