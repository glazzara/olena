#include <mln/core/image2d.hh>
#include <mln/core/sub_image.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <mln/accu/min.hh>
#include <mln/norm/l2.hh>
// #include <mln/literal/zero.hh>

namespace mln
{
  template <typename I>
  inline
  mln_ch_value(I, float)
  dmap(const Image<I>& input_)
  {
    const I& input = exact(input_);
    mln_precondition(input.has_data());

    mln_ch_value(I, float) output;
    initialize(output, input);
    accu::min_<float> min;

    mln_piter(I) p(input.domain());
    for_all(p)
      if (input(p) == true)
        {
          // Object so distance is null.
          output(p) = literal::zero;
        }
      else
        {
          min.init();
          // p is in the background so the distance has to be computed.
          mln_piter(I) q(input.domain());
          for_all(q)
            if (input(q) == true) // q is in the object.
              {
                metal::vec<2,int> vp = p.to_point(), vq = q.to_point();
                min.take(norm::l2_distance(vp, vq));
              }
          output(p) = min;
        }
    return output;
  }
} // end of namespace mln

int main()
{
  using namespace mln;

  image2d<bool> ima(3,3);
  bool vals[] = { 1, 1, 0,
                  0, 0, 1,
                  0, 0, 0 };
  level::fill(ima, vals);

  debug::println(ima);
  debug::println(dmap(ima | box2d(point2d(0,1), point2d(2,2))));
}
