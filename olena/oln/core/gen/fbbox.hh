// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_GEN_FBBOX_HH
# define OLN_CORE_GEN_FBBOX_HH

# include <oln/core/gen/box.hh>
# include <oln/core/concept/point.hh>
# include <oln/core/internal/f_point_to_box.hh>


namespace oln
{


  /// Bounding box function based on a point class.

  template <typename P>
  class fbbox_ : private mlc::assert_< mlc_is_a(P, Point) >
  {
  private:
    typedef stc_type(P, dim) dim__;
    enum { n = mlc_value(dim__) };

  public:

    typedef oln_f_point_to_box(P) box_t;

    fbbox_();
    operator box_t() const;

    bool is_valid() const;
    void flush();
    fbbox_<P>& take(const P& p);
    const box_t& box() const;

  private:
    bool is_valid_;
    box_t b_;

  }; // end of class oln::fbbox_<P>



# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  fbbox_<P>::fbbox_()
  {
    this->flush();
  }

  template <typename P>
  fbbox_<P>::operator typename fbbox_<P>::box_t() const
  {
    precondition(this->is_valid_);
    return this->b_;
  }

  template <typename P>
  bool fbbox_<P>::is_valid() const
  {
    return this->is_valid_;
  }

  template <typename P>
  void fbbox_<P>::flush()
  {
    is_valid_ = false;
  }

  template <typename P>
  fbbox_<P>&
  fbbox_<P>::take(const P& p)
  {
    if (not this->is_valid_)
      {
	this->b_.pmin() = p;
	this->b_.pmax() = p;
	this->is_valid_ = true;
      }
    else
      {
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < this->b_.pmin()[i])
	    this->b_.pmin()[i] = p[i];
	  else if (p[i] > this->b_.pmax()[i])
	    this->b_.pmax()[i] = p[i];
      }
    return *this;
  }

  template <typename P>
  const typename fbbox_<P>::box_t&
  fbbox_<P>::box() const
  {
    precondition(this->is_valid_);
    return this->b_;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FBBOX_HH
