#ifndef WHITE_HH
# define WHITE_HH

#include <cmath>
#include <iostream>
#include <mln/accu/math/sum.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/binarization/threshold.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/fill.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/compute_image.hh>
#include <mln/labeling/wrap.hh>
#include <mln/util/array.hh>
#include <mln/util/fibonacci_heap.hh>
#include <mln/value/int_u32.hh>
#include <mln/value/label_16.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/pack.hh>
#include <mln/border/fill.hh>
#include <mln/labeling/colorize.hh>
#include <check_size_functor.hh>

namespace whitespace
{
  using namespace mln;

  void whitespace_compute (image2d<bool>& ima,
                           image2d<bool>& cleaned);
}

# include <white.hxx>

#endif /* !WHITE_HH */
