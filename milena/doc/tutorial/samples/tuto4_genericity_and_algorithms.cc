#include <mln/essential/2d.hh>
#include <mln/binarization/threshold.hh>
#include <mln/debug/colorize.hh>
#include <tests/data.hh>
#include <doc/tutorial/tools/sample_utils.hh>

struct keep_specific_colors : public mln::Function_v2b<keep_specific_colors>
{
  bool operator()(const mln::value::rgb16& v) const
  {
    return v.green() < 20000 && v.blue() > 10000;
  }
};

int main()
{
  using namespace mln;

  // \{
  image2d<value::rgb16> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  // \}


  image2d<value::rgb16> lena_bak = duplicate(lena);
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
  value::label_16 nlabels;
  image2d<value::label_16> label = labeling::blobs(lena_bw, c8(), nlabels);
  // \}
  doc::ppmsave(debug::colorize(value::rgb16(), label, nlabels), "tuto4_genericity_and_algorithms");

  // \{
  data::fill((lena | (pw::value(label) == pw::cst(16u))).rw(), literal::green);
  // \}
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




  lena = duplicate(lena_bak);
  // \{
  data::fill(extract::green(lena).rw(), literal::max);
  // \}
  //FIXME: we would like to save the green component in rgb8.
  doc::pgmsave(extract::green(lena), "tuto4_genericity_and_algorithms");
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");




  lena = duplicate(lena_bak);
  // \{
  mln_VAR(object, pw::value(label) == pw::cst(3u));
  data::fill((extract::green(lena).rw() | object).rw(), literal::max);
  // \}
  doc::pgmsave(lena | object, "tuto4_genericity_and_algorithms");
  doc::ppmsave(lena, "tuto4_genericity_and_algorithms");

}
