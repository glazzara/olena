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
# include <mln/core/concept/fast_iterator.hh>


namespace mln
{
  /*! \brief A generic forward iterator on image pixel of a windows and of
   *  neighborhoods.
   *
   * The parameter \c I is the image type.
   */
  template <typename I>
  class dpoints_pixter : public Fast_Iterator< dpoints_pixter<I> >
  {
  public:
    /// Image pixel value
    typedef mln_value(I) value;
    /// Image pixel rvalue
    typedef mln_rvalue(I) rvalue;
    /// Image pixel lvalue
    typedef mln_lvalue(I) lvalue;

    /*! \brief Constructo.
     *
     * \param[in] dps   Object that can provide an array of delta-points.
     * \param[in] p_ref Center point to iterate around.
     * \param[in] ima   Image to iterate.
     */
    template <typename Dps>
    dpoints_pixter(const Dps& dps, const typename I::point& p_ref,I& ima);

    /// Set the iterator at the start.
    void start();
    /// Go to the next pixel.
    void next_();
    /// Invalidate the iterator.
    void invalidate();
    ///  Is the iterator valid?
    bool is_valid() const;

    /// Get the iterator value
    rvalue operator* ();
    lvalue operator* () const;

  private:
    /// offset of each dpoints
    std::vector<int> offset_;
    /// current offset
    std::vector<int>::iterator i_;
    /// reference pixel in the image
    value* pixref_;
  };


#ifndef MLN_INCLUDE_ONLY

  template <typename I>
  template <typename Dps>
  dpoints_pixter<I>::dpoints_pixter(const Dps& dps, const typename I::point& ref, I& ima)
  {
    pixref_ = &ima(ref);
    for (typename std::vector<typename I::dpoint>::const_iterator it = dps.vec().begin();
	 it != dps.vec().end();
	 ++it)
      offset_.push_back(ima.offset(*it));
  }


  template <typename I>
  void dpoints_pixter<I>::start()
  {
    i_ = offset_.begin();
  }

  template <typename I>
  void dpoints_pixter<I>::next_()
  {
    ++i_;
  }

  template <typename I>
  bool dpoints_pixter<I>::is_valid() const
  {
    return i_ != offset_.end();
  }

  template <typename I>
  void dpoints_pixter<I>::invalidate()
  {
    i_ = offset_.end();
  }

  template <typename I>
  typename dpoints_pixter<I>::rvalue
  dpoints_pixter<I>::operator* ()
  {
    assert(this->is_valid());
    return *(pixref_ + *i_);
  }

  template <typename I>
  typename dpoints_pixter<I>::lvalue
  dpoints_pixter<I>::operator* () const
  {
    assert(this->is_valid());
    return *(pixref_ + *i_);
  }

#endif // ! MLN_INCLUDE_ONLY

}


#endif // MLN_CORE_DPOINTS_PIXTER_HH
