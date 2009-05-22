// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_MAKE_TEXT_HH
# define SCRIBO_MAKE_TEXT_HH

/// \file scribo/make/text.hh
///
/// Construct a util::text.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>
# include <mln/labeling/relabel.hh>
# include <mln/util/array.hh>
# include <mln/make/relabelfun.hh>

# include <scribo/core/macros.hh>
# include <scribo/util/text.hh>


namespace scribo
{

  namespace make
  {


    /// Construct a util::text
    ///
    /// \param[in] bboxes Text component bounding boxes.
    /// \param[in] lbl Associated label image.
    /// \param[in] nbboxes The number of bounding boxes.
    ///
    /// \return A util::text.
    template <typename L>
    scribo::util::text<L>
    text(const mln::util::array<box<mln_site(L)> >& bboxes,
	 const Image<L>& lbl,
	 mln_value(L)& nbboxes);

    /// Construct a util::text
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] nbh_ A neighborhood to be used with labeling algorithms.
    /// \param[in] nbboxes The number of bounding boxes.
    ///
    /// \return A util::text.
    template <typename I, typename N, typename V>
    scribo::util::text<mln_ch_value(I,V)>
    text(const Image<I>& input_, const Neighborhood<N>& nbh_,
	 V& nbboxes);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    scribo::util::text<L>
    text(const mln::util::array<box<mln_site(L)> >& bboxes,
	 const Image<L>& lbl,
	 mln_value(L)& nbboxes)
    {
      trace::entering("scribo::make::text");

      mln_precondition(exact(lbl).is_valid());

      scribo::util::text<L> result = scribo::util::text<L>(bboxes, lbl,
							   nbboxes);

      trace::exiting("scribo::make::text");
      return result;
    }



    template <typename I, typename N, typename V>
    scribo::util::text<mln_ch_value(I,V)>
    text(const Image<I>& input_, const Neighborhood<N>& nbh_,
	 V& nbboxes)
    {
      trace::entering("scribo::make::text");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      typedef mln::util::array< box<mln_site(I)> > boxes_t;

      mln_ch_value(I,V) lbl = labeling::blobs(input, nbh, nbboxes);

      boxes_t cboxes = labeling::compute(accu::meta::bbox(), lbl, nbboxes);

      trace::exiting("scribo::make::text");
      return make::text(cboxes, lbl, nbboxes);
    }

    //FIXME: we want the following routine to construct a new util::text
    // from another one and a relabeling function. It avoids recomputing
    // the whole underlying data (mass centers, bboxes...)
    template <typename L, typename F>
    scribo::util::text<L>
    text(const scribo::util::text<L>& text,
	 const Function<F>& f_)
    {
      trace::entering("scribo::make::text");

      mln_precondition(text.is_valid());
      const F& f = exact(f_);

      mln_value(L) new_nbboxes;
      mln::fun::i2v::array<mln_value(L)> fv2v
	= mln::make::relabelfun(f, text.nbboxes(), new_nbboxes);
      L lbl = labeling::relabel(text.label_image(), text.nbboxes(), fv2v);

      mln::util::array< accu::bbox<mln_site(L)> > tboxes(new_nbboxes.next());
      mln::util::array< accu::center<mln_site(L)> > tcenters(new_nbboxes.next());
      for_all_components(i, text.bboxes())
      {
	tboxes[fv2v(i)].take(text.bbox(i));
	tcenters[fv2v(i)].take(text.mass_center(i));
      }

      scribo::util::text<L> new_text;
      new_text.nbboxes() = new_nbboxes;
      new_text.label_image() = lbl;
      convert::from_to(tboxes, new_text.bboxes());
      convert::from_to(tcenters, new_text.mass_centers());

      trace::exiting("scribo::make::text");
      return new_text;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::make

} // end of namespace scribo

#endif // ! SCRIBO_MAKE_TEXT_HH
