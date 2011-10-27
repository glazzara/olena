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

#ifndef SCRIBO_DEBUG_SAVE_COMP_DIFF_HH
# define SCRIBO_DEBUG_SAVE_COMP_DIFF_HH

/// \file
///
/// Show the difference between two object images.

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/labeling/colorize.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/ppm/save.hh>
# include <mln/literal/black.hh>
# include <mln/literal/colors.hh>
# include <mln/pw/all.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>

namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /*! \brief Show the difference between two object images.

      \param[in] comps_ref   A component set.
      \param[in] comps_new   Another component set.
      \param[in] filename The output filename.

      \ingroup grpalgodebug
    */
    template <typename L, typename L2>
    void
    save_comp_diff(const component_set<L>& comps_ref,
		   const component_set<L2>& comps_new,
		   const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


  template <typename L, typename L2>
  void
  save_comp_diff(const component_set<L>& comps_ref,
		 const component_set<L2>& comps_new,
		 const std::string& filename)
  {
    trace::entering("scribo::debug::save_comp_diff");

    image2d<value::rgb8> output;
    initialize(output, comps_ref.labeled_image());

    data::fill(output, literal::black);

    for_all_comps(i, comps_ref)
      data::fill(((output | comps_ref(i).bbox()).rw()
		  | (pw::value(comps_ref.labeled_image()) == i)).rw(),
		 literal::red);

    for_all_comps(i, comps_new)
      data::fill(((output | comps_new(i).bbox()).rw()
		  | (pw::value(comps_new.labeled_image()) == i)).rw(),
		 literal::green);

    io::ppm::save(output, filename);

    trace::exiting("scribo::debug::save_comp_diff");
  }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_SAVE_COMP_DIFF_HH
