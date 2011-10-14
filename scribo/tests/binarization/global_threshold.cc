// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/make/image.hh>
#include <mln/data/compare.hh>

#include <scribo/binarization/global_threshold.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  unsigned data[2][2] = { { 12, 24 },
			  {  2,  6 } };

  bool data_ref[2][2] = { { 0, 0 },
			  { 1, 1 } };

  image2d<unsigned> input = make::image(data);
  image2d<bool> ref = make::image(data_ref);

  image2d<bool> bin = scribo::binarization::global_threshold(input, 10);

  mln_assertion(bin == ref);
}
