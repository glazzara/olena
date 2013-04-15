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

#ifndef SCRIBO_FILTER_SEPARATORS_IN_ELEMENT_HH
# define SCRIBO_FILTER_SEPARATORS_IN_ELEMENT_HH

/// \file
///
/// \brief Invalidate false positive separators.
/// \todo Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <mln/draw/box_plain.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/document.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// \brief Invalidate false positive separators.
    ///
    /// \param[in,out] doc    A document structure.
    ///
    /// Warning: it does not remove separators from separator
    /// image. It only invalidate separator components in their
    /// respective component_set.
    ///
    /// \ingroup grpalgofilterelt
    //
    template <typename L>
    void
    separators_in_element(document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    separators_in_element(document<L>& doc)
    {
      mln_trace("scribo::filter::separators_in_element");

      mln_precondition(doc.is_valid());
      mln_precondition(doc.has_elements());
      mln_precondition(doc.has_hline_seps() || doc.has_vline_seps());

      if ((doc.has_hline_seps() && !doc.hline_seps_comps().nelements())
	  && (doc.has_vline_seps() && !doc.vline_seps_comps().nelements()))
	return;

      mln_ch_value(L,bool) billboard;
      initialize(billboard, doc.image());
      data::fill(billboard, false);

      for_all_comps(e, doc.elements())
	if (doc.elements()(e).is_valid())
	  mln::draw::box_plain(billboard, doc.elements()(e).bbox(), true);

      // Horizontal separators
      if (doc.has_hline_seps())
      {
	component_set<L> hline = doc.hline_seps_comps().duplicate();
	for_all_comps(c, hline)
	  if (hline(c).is_valid())
	  {
	    const mln_box(L)& b_ = hline(c).bbox();

	    const bool tl = billboard(b_.pmin());
	    const bool tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	    const bool ml = billboard.at_(b_.pcenter().row(), b_.pmin().col());
	    const bool mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());
	    const bool mr = billboard.at_(b_.pcenter().row(), b_.pmax().col());
	    const bool bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	    const bool br = billboard(b_.pmax());

	    // This separator is included in an element (picture, drawing...)
	    // => Ignore it.
	    if (tl && tr && ml && mc && mr && bl && br)
	      hline(c).update_tag(component::Ignored);
	  }
	// FIXME: warning this call may produce inconsistent data
	// Ignored components are still in the separator image...
	doc.set_hline_separators(doc.hline_seps(), hline);
      }

      // Vertical separators
      if (doc.has_vline_seps())
      {
	component_set<L> vline = doc.vline_seps_comps().duplicate();
	for_all_comps(c, vline)
	  if (vline(c).is_valid())
	  {
	    const mln_box(L)& b_ = vline(c).bbox();

	    const bool tl = billboard(b_.pmin());
	    const bool tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	    const bool ml = billboard.at_(b_.pcenter().row(), b_.pmin().col());
	    const bool mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());
	    const bool mr = billboard.at_(b_.pcenter().row(), b_.pmax().col());
	    const bool bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	    const bool br = billboard(b_.pmax());

	    // This separator is included in an element (picture, drawing...)
	    // => Ignore it.
	    if (tl && tr && ml && mc && mr && bl && br)
	      vline(c).update_tag(component::Ignored);
	  }
	// FIXME: warning this call may produce inconsistent data
	// Ignored components are still in the separator image...
	doc.set_vline_separators(doc.vline_seps(), vline);
      }

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_SEPARATORS_IN_ELEMENT_HH
