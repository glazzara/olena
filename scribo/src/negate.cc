// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file scribo/src/negate.cc
///
/// Negate a binary image.

#include <mln/core/image/image2d.hh>
#include <mln/logical/not.hh>
#include <mln/io/pbm/all.hh>

#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  mln::trace::entering("main");
  using namespace mln;

  if (argc != 3)
    return usage(argv, "Negate a binary image", "input.pbm output.pbm",
		 args_desc, "A binary image.");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  io::pbm::save(logical::not_(input), argv[2]);

  mln::trace::exiting("main");

}

