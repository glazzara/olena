// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_DPOINTS_PIXTER_HH
# define MLN_CORE_DPOINTS_PIXTER_HH

# include <cassert>
# include <vector>
# include <mln/core/internal/pixel_iterator_base.hh>


namespace mln
{

  /*! \brief A generic forward iterator on image pixel of a windows and of
   *  neighborhoods.
   *
   * The parameter \c I is the image type.
   */
  template <typename I>
  class dpoints_pixter : public internal::pixel_iterator_base_< I, dpoints_pixter<I> >
  {
    typedef typename internal::pixel_iterator_base_< I, dpoints_pixter<I> > super;
  public:

    /// Image pixel value
    typedef mln_value(I) value;
    /// Image pixel rvalue
    typedef mln_rvalue(I) rvalue;
    /// Image pixel lvalue
    typedef mln_lvalue(I) lvalue;

    /*! \brief Constructor.
     *
     * \param[in] dps   Object that can provide an array of delta-points.
     * \param[in] p_ref Center point to iterate around.
     * \param[in] ima   Image to iterate.
     */
    template <typename Dps, typename Pref>
    dpoints_pixter(const Dps& dps,
		   const GenPoint<Pref>& p_ref,
		   I& image);

    template <typename Dps, typename Pref>
    dpoints_pixter(const Dps& dps,
		   const GenPixel<Pref>& p_ref,
		   I& image);

    /// Start an iteration.
    void start();

    /// Go to the next pixel.
    void next_();

    /// Invalidate the iterator.
    void invalidate();

    /// Test the iterator validity.
    bool is_valid() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved. 
    void update();

    /// The value around which this iterator moves.
    const value& center_value() const;

  private:

    /// offset of each dpoints
    std::vector<int> offset_;

    /// current offset
    int i_;

    /// reference pixel / point in the image
    value** value_ref_;
    //    or:
    const mln_point(I)* p_ref_;


    template <typename Dps>
    void init_(const Dps& dps);
  };


#ifndef MLN_INCLUDE_ONLY

  template <typename I>
  template <typename Dps, typename Pref>
  dpoints_pixter<I>::dpoints_pixter(const Dps& dps,
				    const GenPoint<Pref>& p_ref,
				    I& image)
    : super(image)
  {
    p_ref_ = internal::force_exact<Pref>(p_ref).pointer();
    value_ref_ = 0;
    init_(dps);
  }


  template <typename I>
  template <typename Dps, typename Pref>
  dpoints_pixter<I>::dpoints_pixter(const Dps& dps,
				    const GenPixel<Pref>& p_ref,
				    I& image)
    : super(image)
  {
    p_ref_ = 0;
    value_ref_ = internal::force_exact<Pref>(p_ref).address();
    init_(dps);
  }

  template <typename I>
  const mln_value(I)&
  dpoints_pixter<I>::center_value() const
  {
    mln_invariant(value_ref_ != 0 || p_ref_ != 0);
    if (p_ref_)
      return image_(*p_ref_);
    else
      return ** value_ref_;
  }

  template <typename I>
  template <typename Dps>
  void
  dpoints_pixter<I>::init_(const Dps& dps)
  {
    for (unsigned i = 0; i < dps.nelements(); ++i)
      offset_.push_back(this->image_.offset(dps.element(i)));

    if (dps.nelements() > 1)
      for (unsigned i = dps.nelements() - 1; i > 0; --i)
	offset_[i] -= offset_[i - 1];

    // offset_[0] is absolute; other offsets are relative.
    invalidate();
  }

  template <typename I>
  void dpoints_pixter<I>::update()
  {
    if (is_valid())
      {
	if (p_ref_)
	  this->value_ptr_ = & image_(*p_ref_) + offset_[i_];
	else
	  this->value_ptr_ = * value_ref_ + offset_[i_];
      }
  }

  template <typename I>
  void dpoints_pixter<I>::start()
  {
    i_ = 0;
    update();
  }

  template <typename I>
  void dpoints_pixter<I>::next_()
  {
    ++i_;
    this->value_ptr_ += offset_[i_];    
  }

  template <typename I>
  bool dpoints_pixter<I>::is_valid() const
  {
    return i_ != offset_.size();
  }

  template <typename I>
  void dpoints_pixter<I>::invalidate()
  {
    i_ = offset_.size();
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // MLN_CORE_DPOINTS_PIXTER_HH
