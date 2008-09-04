// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_P_IMAGE2D_PIXTER_HH
# define MLN_CORE_P_IMAGE2D_PIXTER_HH

/// \file mln/core/p_image2d_pixter.hh
/// \brief Pixel iterators on a mln::p_image2d.

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/geom/size2d.hh>

namespace mln
{

  /*--------------------------.
  | p_image2d_fwd_pixter<P>.  |
  `--------------------------*/

  template <typename P>
  class p_image2d_fwd_pixter
    : public internal::forward_pixel_iterator_base_<
        typename p_image2d<P>::image_type,
        p_image2d_fwd_pixter<P>
      >
  {
    typedef internal::forward_pixel_iterator_base_<
      typename p_image2d<P>::image_type,
      p_image2d_fwd_pixter<P>
    > super_;

  public:
    /// Image type.
    typedef typename p_image2d<P>::image_type image;
    /// Point type.
    typedef point2d point;

    /// \brief Constructor.
    /// \param[in] image The mln::p_image2d this pixel iterator is bound to.
    p_image2d_fwd_pixter(p_image2d<P>& s);

    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Reference of the corresponding point.
    const point to_point() const;

  private:
    /// Row offset.
    unsigned row_offset_;
    /// End of the current row.
    mln_qlf_value(image)* eor_;
  };


  /*--------------------------.
  | p_image2d_bkd_pixter<P>.  |
  `--------------------------*/

  template <typename P>
  class p_image2d_bkd_pixter
    : public internal::backward_pixel_iterator_base_<
        typename image2d<P>::image_type,
        p_image2d_bkd_pixter<P>
      >
  {
    typedef internal::backward_pixel_iterator_base_<
      typename image2d<P>::image_type,
      p_image2d_bkd_pixter<P>
    > super_;

  public:
    /// Image type.
    typedef typename p_image2d<P>::image_type image;
    /// Point type.
    typedef point2d point;

    /// \brief Constructor.
    /// \param[in] image The mln::p_image2d this pixel iterator is bound to.
    p_image2d_bkd_pixter(p_image2d<P>& s);

    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Reference of the corresponding point.
    const point to_point() const;

  private:
    /// Row offset.
    unsigned row_offset_;
    /// Beginning of the current row.
    mln_qlf_value(image)* bor_;
  };



#ifndef MLN_INCLUDE_ONLY

  /*--------------------------.
  | p_image2d_fwd_pixter<P>.  |
  `--------------------------*/

  template <typename P>
  inline
  p_image2d_fwd_pixter<P>::p_image2d_fwd_pixter(p_image2d<P>& s)
    : super_(s.image_non_const())
  {
    mln_precondition(this->image_.has_data());
    row_offset_ = geom::max_col(this->image_) - geom::min_col(this->image_) + 1;
    eor_ = & this->image_.at(geom::min_row(this->image_),
			     geom::max_col(this->image_)) + 1;
  }

  template <typename P>
  inline
  void
  p_image2d_fwd_pixter<P>::start()
  {
    this->value_ptr_ = this->boi_ + 1;
    while(this->is_valid() && !(*this->value_ptr_))
      ++this->value_ptr_;
  }

  template <typename P>
  inline
  void
  p_image2d_fwd_pixter<P>::next_()
  {
    ++this->value_ptr_;
    while(this->is_valid() && !(*this->value_ptr_))
      ++this->value_ptr_;
  }

  template <typename P>
  inline
  const typename p_image2d_fwd_pixter<P>::point
  p_image2d_fwd_pixter<P>::to_point() const
  {
    return this->image_.point_at_offset(*this);
  }


  /*--------------------------.
  | p_image2d_bkd_pixter<P>.  |
  `--------------------------*/

  template <typename P>
  inline
  p_image2d_bkd_pixter<P>::p_image2d_bkd_pixter(p_image2d<P>& s)
    : super_(s.image_non_const())
  {
    mln_precondition(this->image_.has_data());
    row_offset_ = geom::max_col(this->image_) - geom::min_col(this->image_) + 1;
    bor_ = & this->image_.at(geom::max_row(this->image_),
			     geom::min_col(this->image_)) - 1;
  }

  template <typename P>
  inline
  void
  p_image2d_bkd_pixter<P>::start()
  {
    this->value_ptr_ = this->eoi_ - 1;
    while(this->is_valid() && !(*this->value_ptr_))
      --this->value_ptr_;
  }

  template <typename P>
  inline
  void
  p_image2d_bkd_pixter<P>::next_()
  {
    --this->value_ptr_;
    while(this->is_valid() && !(*this->value_ptr_))
      --this->value_ptr_;
  }

  template <typename P>
  inline
  const typename p_image2d_bkd_pixter<P>::point
  p_image2d_bkd_pixter<P>::to_point() const
  {
    return this->image_.point_at_offset(*this);
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_P_IMAGE2D_PIXTER_HH
