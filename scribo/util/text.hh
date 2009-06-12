// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_UTIL_TEXT_HH
# define SCRIBO_UTIL_TEXT_HH

/// \file
///
/// A class representing text bounding boxes and their associated
/// labeled image.

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/site_set/box.hh>
# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
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
      /// The site type in the label image.
      typedef mln_site(L) site;
      /// The bounding box list type.
      typedef mln::util::array<box<site> > boxes_t;
      /// The mass center list type.
      typedef mln::util::array<mln_site(L)::vec> mass_centers_t;

      /// Constructors
      /// @{

      /// Default.
      text();

      /// \param[in] bboxes text line bounding boxes.
      /// \param[in] lbl label image associated to the bounding boxes.
      /// \param[in] nbboxes The number of bounding boxes.
      /// \param[in] mass_centers Mass centers of the components.
      text(const mln::util::array<box<mln_site(L)> >& bboxes,
	   const Image<L>& lbl,
	   const mln_value(L)& nbboxes,
	   const mln::util::array<mln_site(L)::vec>& mass_centers);

      /// \param[in] bboxes text line bounding boxes.
      /// \param[in] lbl label image associated to the bounding boxes.
      /// \param[in] nbboxes The number of bounding boxes.
      text(const mln::util::array<box<mln_site(L)> >& bboxes,
	   const Image<L>& lbl,
	   const mln_value(L)& nbboxes);
      /// @}

      /// Return the underlying label image.
      /// @{
      const L& label_image() const;
      L& label_image();
      /// @}

      /// Return the number of bounding boxes.
      const mln_value(L)& nbboxes() const;
      /// Return the number of bounding boxes.
      mln_value(L)& nbboxes();

      /// Return the bounding boxes.
      const boxes_t& bboxes() const;
      /// Return the bounding boxes.
      boxes_t& bboxes();

      /// Return the i-th bounding box.
      const box<mln_site(L)>& bbox(unsigned i) const;
      /// Return the i-th bounding box.
      box<mln_site(L)>& bbox(unsigned i);

      /// Return the mass centers.
      /// @{
      const mln::util::array<mln_site(L)::vec>& mass_centers() const;
      mln::util::array<mln_site(L)::vec>& mass_centers();
      /// @}

      /// Return the i-th mass centers.
      mln_site(L) mass_center(unsigned i) const;

      /// Is this text objet consistant/valid?
      bool is_valid() const;

    private:
      void update();

      boxes_t bboxes_;
      mass_centers_t mass_centers_;
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
		  const mln_value(L)& nbboxes,
		  const mln::util::array<mln_site(L)::vec>& mass_centers)
      : bboxes_(bboxes), mass_centers_(mass_centers),
	lbl_(exact(lbl)), nbboxes_(nbboxes)
    {
      mln_assertion(bboxes.nelements() == nbboxes.next());
    }

    template <typename L>
    inline
    text<L>::text(const mln::util::array<box<mln_site(L)> >& bboxes,
		  const Image<L>& lbl,
		  const mln_value(L)& nbboxes)
      : bboxes_(bboxes), lbl_(exact(lbl)), nbboxes_(nbboxes)
    {
      mln_assertion(bboxes.nelements() == nbboxes.next());
      update();
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
//      mln_precondition(lbl_.is_valid());
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
    const mln::util::array<mln_site(L)::vec>&
    text<L>::mass_centers() const
    {
      return mass_centers_;
    }

    template <typename L>
    inline
    mln::util::array<mln_site(L)::vec>&
    text<L>::mass_centers()
    {
      return mass_centers_;
    }

    template <typename L>
    inline
    mln_site(L)
    text<L>::mass_center(unsigned i) const
    {
      return mass_centers_[i];
    }

    template <typename L>
    inline
    bool
    text<L>::is_valid() const
    {
      return lbl_.is_valid() && bboxes_.nelements() == nbboxes_.next();
    }

    template <typename L>
    inline
    void
    text<L>::update()
    {
      mass_centers_ = labeling::compute(accu::meta::center(), lbl_, nbboxes_);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_TEXT_HH
