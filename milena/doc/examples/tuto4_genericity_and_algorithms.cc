#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/var.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/fun/p2b/chess.hh>

#include <mln/extract/green.hh>

#include <mln/accu/stat/max.hh>

#include <mln/geom/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>

#include <mln/opt/at.hh>

#include <mln/data/fill.hh>

#include <mln/pw/all.hh>

#include <mln/binarization/threshold.hh>

#include <mln/labeling/colorize.hh>
#include <mln/labeling/blobs.hh>

#include <mln/literal/colors.hh>
#include <mln/literal/max.hh>


#include <tests/data.hh>
#include <doc/tools/sample_utils.hh>

struct keep_specific_colors : public mln::Function_v2b<keep_specific_colors>
{
  bool operator()(const mln::value::rgb8& v) const
  {
    return v.green() < 200u && v.blue() > 100u;
  }
};

int main()
{
  using namespace mln;

  // \{
  image2d<value::rgb8> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  // \}


  image2d<value::rgb8> lena_bak = duplicate(lena);
  // \{
  box2d roi = make::box2d(20, 20, 40, 40);
  // \}
  // \{
  data::fill((lena | roi).rw(), literal::green);
  // \}
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




  lena = duplicate(lena_bak);
  // \{
  p_array<point2d> arr;
  for (def::coord row = geom::min_row(lena); row < geom::max_row(lena); ++row)
    for (def::coord col = geom::min_row(lena); col < geom::max_col(lena); ++col)
      if (((row + col) % 2) == 0)
	arr.append(point2d(row, col));
  // \}
  // \{
  for (def::coord row = geom::min_row(lena); row < geom::max_row(lena); ++row)
    for (def::coord col = geom::min_row(lena); col < geom::max_col(lena); ++col)
      if (((row + col) % 2) == 0)
        opt::at(lena, row, col) = literal::green;
  // \}
  // \{
  data::fill((lena | fun::p2b::chess()).rw(), literal::green);
  // \}
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




  lena = duplicate(lena_bak);
  // \{
  image2d<bool> mask;
  initialize(mask, lena);
  data::fill(mask, false);
  data::fill((mask | make::box2d(10, 10, 14, 14)).rw(), true);
  data::fill((mask | make::box2d(25, 15, 29, 18)).rw(), true);
  data::fill((mask | make::box2d(50, 50, 54, 54)).rw(), true);
  // \}
  doc::pbmsave(mask, "tuto4_genericity_and_algorithms");
  // \{
  data::fill((lena | pw::value(mask)).rw(), literal::green);
  // \}
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




  lena = duplicate(lena_bak);
  // \{
  image2d<bool> lena_bw = binarization::binarization(lena, keep_specific_colors());
  value::label_8 nlabels;
  image2d<value::label_8> label = labeling::blobs(lena_bw, c8(), nlabels);
  // \}
  doc::ppmsave(labeling::colorize(value::rgb8(), label, nlabels), "tuto4_genericity_and_algorithms");

  // \{
  data::fill((lena | (pw::value(label) == pw::cst(0u))).rw(), literal::blue);
  // \}
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




// Disabled until thru_image/fun_image provide working read/write access.
//
//  lena = duplicate(lena_bak);
//  // \{
//  data::fill(extract::green(lena).rw(), literal::max);
//  // \}
//  //FIXME: we would like to save the green component in rgb8.
//  doc::pgmsave(extract::green(lena), "tuto4_genericity_and_algorithms");
//  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");
//
//
//
//
//  lena = duplicate(lena_bak);
//  // \{
//  mln_VAR(object, pw::value(label) == pw::cst(0u));
//  data::fill((extract::green(lena).rw() | object).rw(), literal::max);
//  // \}
//  //FIXME: how to display an image which is not defined on a box!?
////  doc::ppmsave(lena | object, "tuto4_genericity_and_algorithms");
//  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");

}
