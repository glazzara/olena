// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/accu/center.hh>
#include <mln/labeling/blobs_and_compute.hh>
#include <mln/util/array.hh>
#include <mln/util/couple.hh>
#include <mln/value/label_8.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  image2d<bool> pic = io::pbm::load(MLN_IMG_DIR "/picasso.pbm");
  value::label_8 n;


  typedef image2d<value::label_8> L;
  typedef util::array<point2d> arr_t;
  typedef accu::center<point2d,point2d> accu_t;
  typedef util::array<accu_t> accu_arr_t;
  typedef util::couple<L,util::couple<arr_t, accu_arr_t> > res_t;
  res_t result = labeling::blobs_and_compute(pic, c4(), n,
					     accu_t());


  mln_assertion(result.second().first().size() == 34);
  mln_assertion(result.second().second().size() == 34);
  mln_assertion(result.second().first()[1] == point2d(10,30));
  mln_assertion(result.second().second()[33] == point2d(311,67));
  mln_assertion(n == 33);
}
