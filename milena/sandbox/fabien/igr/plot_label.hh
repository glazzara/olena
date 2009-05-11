#ifndef PLOT_LABEL_HH
# define PLOT_LABEL_HH

#include <iostream>
#include <sstream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/geom/bbox.hh>

#include <mln/io/plot/save.hh>

#include <mln/accu/mean.hh>
#include <mln/accu/median_h.hh>
#include <mln/level/compute.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>


using namespace mln;



template <typename I, typename L, typename V>
inline
void plot_label(image3d<I>& ima, image2d<L>& ima_labels, V lbl)
{
  typedef image_if<image2d<L>, mln::fun::eq_p2b_expr_<pw::value_<image2d<L> >, pw::cst_<V> > > Lx;
  Lx ima_label = ima_labels | pw::value(ima_labels) == pw::cst(lbl);
  util::array<float> arr;
  int sli_min = geom::bbox(ima).pmin().sli();
  int sli_max = geom::bbox(ima).pmax().sli();

  for (int sli = sli_min; sli <= sli_max; ++sli)
  {
    image2d<I> slice_ima = duplicate(slice(ima, sli));
    accu::mean<I> accu_mean;
    float mean = level::compute(accu_mean, slice_ima | ima_label.domain());
    arr.append(mean);
  }

  std::ostringstream slabel;
  slabel << "label_";
  if (lbl < 100)
    slabel << "0";
  if (lbl < 10)
    slabel << "0";
  // FIXME: insert label location
  slabel << lbl << ".plot";
  io::plot::save(arr, slabel.str());
}



#endif // ! PLOT_LABEL_HH
