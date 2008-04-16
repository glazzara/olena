# include "fllt.hh"
//# include "compute_level_set.hh"

# include "compute_level_set_fast2.hh"

# include <mln/core/image2d.hh>
# include <mln/core/cast_image.hh>
# include <mln/core/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/level/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/load.hh>

int main()
{

  using namespace mln;

  typedef point2d P;
  typedef int V;

  //trace::quiet = false;

//   V ws[81] = {5,5,5,5,5,5,5,5,5,
// 		5,5,5,5,5,5,5,5,5,
// 		5,5,5,5,5,5,8,8,5,
// 	        5,5,5,9,5,5,8,8,5,
// 		5,5,5,9,5,5,8,8,5,
// 		5,5,5,5,5,5,8,8,5,
// 		5,5,5,5,5,5,8,8,5,
// 		5,5,5,5,5,5,5,5,5,
// 		5,5,5,5,5,5,5,5,5};

//   w_window2d_int w_win = make::w_window2d(ws);
//   image2d<V> ima = convert::to_image(w_win);

  image2d<value::int_u8> ima_ = io::pgm::load<value::int_u8>("small.pgm");
  image2d<V> ima(ima_.domain());

  level::fill(ima, ima_);

//   {
//     image2d<fllt_node(P, V)*> low_reg(ima.domain());
//     fllt_tree(P, V) lower_tree;
//     lower_tree
//       = fllt::compute_level_set<V, fllt::lower<V> >(ima, low_reg);
//     fllt::draw_tree(ima, lower_tree);
//   }


  {
    image2d<fllt_node(P, V)*> low_reg(ima.domain());
    fllt_tree(P, V) lower_tree;
    lower_tree
      = fllt_fast::compute_level_set_fast<V, fllt::upper<V> >(ima, low_reg);
    //fllt::draw_tree(ima, lower_tree);
  }
}
