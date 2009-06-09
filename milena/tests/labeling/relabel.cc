// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file
///
/// Tests on mln::labeling::relabel.


#include <mln/core/image/image2d.hh>
#include <mln/labeling/relabel.hh>
#include <mln/value/label_16.hh>
#include <mln/debug/println.hh>


struct not_to_remove : public mln::Function_v2b< not_to_remove >
{
  bool operator()(const mln::value::label_16& l) const
  {
    return l < mln::value::label_16(3);
  }
};


bool is_valid(const mln::value::label_16& l)
{
  return l == 0u|| l == 1u || l == 2u;
}



int main()
{
  using namespace mln;
  using value::label_16;

  label_16 vals[6][5] = {
    {0, 1, 1, 0, 0},
    {0, 1, 1, 4, 0},
    {0, 0, 0, 0, 0},
    {2, 2, 0, 3, 0},
    {2, 5, 3, 3, 3},
    {2, 5, 5, 0, 0}
  };

  image2d<label_16> lbl = make::image(vals);
  label_16 nlabels = 5;


  {
    label_16 new_nlabels;
    image2d<label_16> lbl2 = labeling::relabel(lbl,
					      nlabels,
					      new_nlabels,
					      not_to_remove());
    mln_assertion(new_nlabels == 2u);
    mln_piter_(image2d<label_16>) p(lbl2.domain());
    for_all(p)
      mln_assertion(is_valid(lbl2(p)));
  }

  {
    label_16 new_nlabels;
    labeling::relabel_inplace(lbl,
			      nlabels,
			      not_to_remove());
    mln_assertion(nlabels == 2u);
    mln_piter_(image2d<label_16>) p(lbl.domain());
    for_all(p)
      mln_assertion(is_valid(lbl(p)));
  }
}
