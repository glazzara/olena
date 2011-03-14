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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/convert.hh>

#include <mln/util/couple.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/draw/line.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/internal/link_several_dmax_base.hh>
#include <scribo/primitive/link/internal/compute_anchor.hh>
#include <scribo/primitive/link/compute_several.hh>

#include <scribo/draw/bounding_boxes.hh>
#include <scribo/debug/usage.hh>


namespace scribo
{

  template <typename I, typename L>
  struct several_right_overlap_debug_functor
    : public primitive::link::internal::link_several_dmax_base<L, several_right_overlap_debug_functor<I, L> >
  {
    typedef several_right_overlap_debug_functor<I, L> self_t;
    typedef
      primitive::link::internal::link_several_dmax_base<L, self_t> super_;

  public:
    typedef mln_site(L) P;

    several_right_overlap_debug_functor(const I& input,
					const component_set<L>& comps,
					unsigned dmax)
      : super_(comps, dmax)
    {
      this->anchors_.append(anchor::Top);
      this->anchors_.append(anchor::Bottom);
      this->anchors_.append(anchor::Center);

      output_ = data::convert(value::rgb8(), input);
      scribo::draw::bounding_boxes(output_, comps, literal::blue);
      mln_postcondition(output_.is_valid());
    }


    mln::util::couple<anchor::Type, mln_site(L)>
    finalize_link_(unsigned current_object)
    {
      mln::util::couple<anchor::Type, mln_site(L)>
	c = super_::finalize_link_(current_object);

      if (c.first() != anchor::Invalid)
      {
	mln_site(L)
	  p = primitive::link::internal::compute_anchor(this->components_,
							current_object,
							c.first());
	mln::draw::line(output_, p, c.second(), literal::green);
      }

      return c;
    }

    mln_site(L)
    start_point_(unsigned current_object, anchor::Type anchor)
    {
      return primitive::link::internal::compute_anchor(this->components_,
						       current_object,
						       anchor);
    }


    void compute_next_site_(P& p)
    {
      ++p.col();
    }


    image2d<value::rgb8> output_;
  };


}


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. True for objects and False for the background." },
  { "max_nbh_dist", "The maximum lookup distance. (common value : 30)" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive::internal;
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Show sucessful/unsuccessful right links "
				"between components.",
				"input.pbm max_nbh_dist output.ppm",
				args_desc);

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  // Finding objects.
  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L> comps
    = scribo::primitive::extract::components(input, c8(), nbboxes);

  // Write debug image.
  several_right_overlap_debug_functor<I, L> functor(input,
						    comps, atoi(argv[2]));
  primitive::link::compute_several(functor);

  io::ppm::save(functor.output_, argv[3]);
}
