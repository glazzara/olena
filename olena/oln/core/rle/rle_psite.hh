// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_RLE_RLE_PSITE_HH_
# define OLN_CORE_RLE_RLE_PSITE_HH_


# include <map>
# include <utility>

namespace oln
{


  // Forward declaration
  template <typename P, typename T> struct rle_image;

  /*
  ** \class rle_piste
  ** \brief psite for rle image
  **
  **
  **
  */
  template <typename P, typename T>
  class rle_psite
  {
  public:
    rle_psite(const rle_image<P, T>& ima, const P& p);
    const typename std::map<std::pair<P, unsigned>, T>::iterator& iterator_() const;
    typename std::map<std::pair<P, unsigned>, T>::iterator& iterator_();
    const P& point_() const;
  protected:
    P p_;	                               		                     /*!< point corresponding to the psite */
    typename std::map<std::pair<P, unsigned>, T>::iterator it_;              /*!< it_ on image map corresponding to p_*/
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P, typename T>
  rle_psite<P, T>::rle_psite(const rle_image<P, T>& ima, const P& p) : p_(p)//, it_(ima.points().con())
  {
    P pend;

//     for (this->it_ = ima.get_data_().begin(); this->it_ != ima.get_data_().end(); ++(this->it_))
//     {
//       pend = this->it_->first.first;
//       pend[0] += this->it_->first.second - 1;
//       if (this->it_->first.first >= p && p <= pend)
//      	return;
//     }
    it_ = ima.get_data_().end();
  }

  template <typename P, typename T>
  const typename std::map<std::pair<P, unsigned>, T>::iterator&
  rle_psite<P, T>::iterator_() const
  {
    return this->it_;
  }

  template <typename P, typename T>
  typename std::map<std::pair<P, unsigned>, T>::iterator&
  rle_psite<P, T>::iterator_()
  {
    return this->it_;
  }

  template <typename P, typename T>
  const P&
  rle_psite<P, T>::point_() const
  {
    return p_;
  }

# endif /* !OLN_INCLUDE_ONLY */

  //end of class rle_psite
}
#endif /* !OLN_CORE_RLE_RLE_PSITE_HH_ */
