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

#ifndef SCRIBO_UTIL_TEXT_HH
# define SCRIBO_UTIL_TEXT_HH

/// \file scribo/util/text.hh
///
/// A class representing text bounding boxes and their associated
/// labeled image.

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/site_set/box.hh>
# include <mln/value/label_16.hh>
# include <mln/util/couple.hh>
# include <mln/util/array.hh>

namespace scribo
{

  using namespace mln;

  namespace util
  {

    /// Store text bounding boxes and their associated labeled image.
    template <typename L>
    class text
    {
    public:
      typedef mln_site(L) site;
      typedef mln::util::array<box<site> > boxes_t;

      text();
      text(const mln::util::array<box<mln_site(L)> >& bboxes,
		  const Image<L>& lbl,
		  const mln_value(L)& nbboxes);

      const L& label_image() const;
      L& label_image();

      const mln_value(L)& nbboxes() const;
      mln_value(L)& nbboxes();

      const boxes_t& bboxes() const;
      boxes_t& bboxes();

      const box<mln_site(L)>& bbox(unsigned i) const;
      box<mln_site(L)>& bbox(unsigned i);

      bool is_valid() const;

    private:
      boxes_t bboxes_;
      L lbl_;
      mln_value(L) nbboxes_;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    inline
    text<L>::text()
    {
    }

    template <typename L>
    inline
    text<L>::text(const mln::util::array<box<mln_site(L)> >& bboxes,
		  const Image<L>& lbl,
		  const mln_value(L)& nbboxes)
      : bboxes_(bboxes), lbl_(exact(lbl)), nbboxes_(nbboxes)
    {
      mln_assertion(bboxes.nelements() == nbboxes.next());
    }

    template <typename L>
    inline
    const L&
    text<L>::label_image() const
    {
      mln_precondition(lbl_.is_valid());
      return lbl_;
    }

    template <typename L>
    inline
    L&
    text<L>::label_image()
    {
      return lbl_;
    }

    template <typename L>
    inline
    const mln_value(L)&
    text<L>::nbboxes() const
    {
      return nbboxes_;
    }

    template <typename L>
    inline
    mln_value(L)&
    text<L>::nbboxes()
    {
      return nbboxes_;
    }

    template <typename L>
    inline
    const typename text<L>::boxes_t&
    text<L>::bboxes() const
    {
      mln_precondition(bboxes_.nelements() == nbboxes_.next());
      return bboxes_;
    }

    template <typename L>
    inline
    typename text<L>::boxes_t&
    text<L>::bboxes()
    {
      return bboxes_;
    }

    template <typename L>
    inline
    const box<mln_site(L)>&
    text<L>::bbox(unsigned i) const
    {
      mln_precondition(bboxes_.nelements() == nbboxes_.next());
      mln_precondition(i < bboxes_.nelements());
      return bboxes_[i];
    }

    template <typename L>
    inline
    box<mln_site(L)>&
    text<L>::bbox(unsigned i)
    {
      mln_precondition(bboxes_.nelements() == nbboxes_.next());
      mln_precondition(i < bboxes_.nelements());
      return bboxes_[i];
    }

    template <typename L>
    inline
    bool
    text<L>::is_valid() const
    {
      return lbl_.is_valid() && bboxes_.nelements() == nbboxes_.next();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_TEXT_HH
