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

#ifndef OLN_CORE_INTERNAL_SYMMETRICAL_WINDOW_HH
# define OLN_CORE_INTERNAL_SYMMETRICAL_WINDOW_HH

# include <oln/core/internal/window.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class symmetrical_window_; }

#define current symmetrical_window_<Exact>
#define super   internal::window_<Exact>

  // Super type.
  template <typename Exact>
  struct super_trait_< internal::current >
  {
    typedef super ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::current >
  {
  };

  namespace internal
  {

    /// Base implementation class for symmetrical window classes.

    template <typename Exact>
    class symmetrical_window_ : public super
    {
    public:
      stc_using(dpoint);

      const Exact& impl_op_unary_minus_() const;
      Exact& impl_take(const dpoint& dp);

    protected:
      symmetrical_window_();

    }; // end of class oln::internal::symmetrical_window_<Exact>


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    current::symmetrical_window_()
    {
    }

    template <typename Exact>
    const Exact&
    current::impl_op_unary_minus_() const
    {
      return exact(*this);
    }

    template <typename Exact>
    Exact&
    current::impl_take(const typename current::dpoint& dp)
    {
      this->super::impl_take(dp);
      this->super::impl_take(-dp);
      return exact(*this);
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

# undef super
# undef current

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_SYMMETRICAL_WINDOW_HH
