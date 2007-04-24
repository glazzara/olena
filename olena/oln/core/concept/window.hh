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

#ifndef OLN_CORE_CONCEPT_WINDOW_HH
# define OLN_CORE_CONCEPT_WINDOW_HH

# include <oln/core/equipment.hh>
# include <oln/core/concept/point.hh>


namespace oln
{

  /// Concept-class "Window".

  template <typename Exact>
  struct Window : public Any<Exact>
  {
    stc_typename(grid);
    stc_typename(point);

    stc_typename(qiter);
    stc_typename(fwd_qiter);
    stc_typename(bkd_qiter);

    Exact op_unary_minus_() const;
    unsigned size() const;

  protected:
    Window();

  }; // end of oln::Window<Exact>



# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  Window<Exact>::Window()
  {
  }

  template <typename Exact>
  Exact
  Window<Exact>::op_unary_minus_() const
  {
    return exact(this)->impl_op_unary_minus_();
  }

  template <typename Exact>
  unsigned
  Window<Exact>::size() const
  {
    return exact(this)->impl_size();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_WINDOW_HH
