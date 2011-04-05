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

// \file

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>

#include <scribo/core/document.hh>
#include <scribo/core/paragraph_set.hh>
#include <scribo/text/extract_lines_wo_merge.hh>
#include <scribo/primitive/extract/alignments.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using namespace scribo;

  typedef image2d<scribo::def::lbl_type> L;

  for (unsigned i = 1; i < 5; ++i)
  {
    std::ostringstream os;
    os << SCRIBO_IMG_DIR << "/alignment_" << i << ".png";

    document<L> doc(os.str().c_str());
    doc.open();
    doc.set_binary_image(data::convert(bool(), doc.image()));

    // Extract lines
    line_set<L>
      lines = scribo::text::extract_lines_wo_merge(doc, c8());
    doc.set_paragraphs(scribo::make::paragraph(lines));

    // Find alignments
    mln::util::couple<component_set<L>, mln_ch_value_(L,bool)>
      res = primitive::extract::alignments(doc, 3, 3);

    std::ostringstream os_out;
    image2d<bool> ref;
    os_out << SCRIBO_TESTS_DIR << "/primitive/extract/alignment_"
	   << i << ".ref.pbm";

    io::pbm::save(res.second(), "res.pbm");
    io::pbm::load(ref, os_out.str().c_str());

    mln_assertion(ref == res.second());
  }

}
