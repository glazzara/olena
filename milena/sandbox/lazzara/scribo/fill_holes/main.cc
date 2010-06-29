#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/transform.hh>
#include <mln/data/convert.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/wrap.hh>

#include <mln/logical/not.hh>

#include <mln/accu/math/count.hh>

#include <mln/labeling/blobs_and_compute.hh>

#include <mln/pw/all.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/literal/colors.hh>

#include <mln/transform/influence_zone_geodesic.hh>


#include <scribo/postprocessing/fill_object_holes.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  typedef image2d<bool> I;

  I input;
  io::pbm::load(input, argv[1]);


  typedef value::int_u16 L;
  typedef image2d<L> Li;
  typedef accu::math::count<mln_site_(Li)> A;

  typedef util::couple<Li, util::couple<util::array<unsigned>,
    util::array<A> > > res_t;


  // Holes card Image

  std::cout << "> Holes card image" << std::endl;

  L nlabels;

  res_t res = labeling::blobs_and_compute(input, c8(), nlabels, A());

  util::array<unsigned>& holes_card = res.second().first();
  image2d<unsigned>
    holes = data::transform(res.first(), holes_card);



  // Threshold Image

  std::cout << "> Threshold image" << std::endl;


  I input_i = logical::not_(input);
  res = labeling::blobs_and_compute(input_i, c8(), nlabels, A());

  float ratio = atof(argv[2]);

  util::array<unsigned>& card = res.second().first();
  for (unsigned i = 1; i < card.size(); ++i)
    card(i) = card(i) * ratio;

  image2d<unsigned>
    thres = data::transform(res.first(), card);
  thres = transform::influence_zone_geodesic(thres, c8());



  // Thresholding

  std::cout << "> Thresholding" << std::endl;

  I hole_mask;
  initialize(hole_mask, holes);
  data::fill(hole_mask, false);
  mln_piter_(I) p(input.domain());
  for_all(p)
    if (holes(p))
      hole_mask(p) = holes(p) < thres(p);

  io::pbm::save(hole_mask, "hole_mask.pbm");



  // Diff

  std::cout << "> Diff" << std::endl;
  image2d<value::rgb8> diff = data::convert(value::rgb8(), input);
  data::fill((diff | pw::value(hole_mask)).rw(), literal::red);
  io::ppm::save(diff, "diff.ppm");



  // Cleanup

  std::cout << "> Cleanup" << std::endl;

  data::fill((input | pw::value(hole_mask)).rw(), false);
  io::pbm::save(input, argv[3]);

}
