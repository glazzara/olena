// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \brief Compute a compressed look-up table (LUT) showing whether a
/// 3D point is simple or not, in parallel, using Intel Threading
/// Building Blocks.

#include "simple_point_3d_lut_tbb.hh"

int
main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv[0]);

  std::string nbhs = argv[1];
  simple_point_lut_t simple_point_lut;
  if      (nbhs == "6_26")  simple_point_lut = simple_point_3d_lut_tbb__6_26();
  else if (nbhs == "26_6")  simple_point_lut = simple_point_3d_lut_tbb__26_6();
  else if (nbhs == "6p_18") simple_point_lut = simple_point_3d_lut_tbb__6p_18();
  else if (nbhs == "18_6p") simple_point_lut = simple_point_3d_lut_tbb__18_6p();
  else
    usage(argv[0]);

  // Display the compressed (8-bit packs) LUT.
  display_simple_point_lut(simple_point_lut);
}
