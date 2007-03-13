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

#ifndef OLN_CORE_INTERNAL_DPOINT1D_HH
# define OLN_CORE_INTERNAL_DPOINT1D_HH

# include <oln/core/internal/dpoint_base.hh>


namespace oln
{


  /// Fwd decls.
  namespace internal { template <typename Exact> struct dpoint1d_; }


  /// Super type.
  template<typename Exact>
  struct super_trait_< internal::dpoint1d_<Exact> >
  {
    typedef internal::dpoint_base_<Exact> ret;
  };


  /// Virtual types.
  template <typename Exact>
  struct vtypes< internal::dpoint1d_<Exact> >
  {
  };


  namespace internal
  {


    template <typename Exact>
    class dpoint1d_ : public dpoint_base_<Exact>
    {
      typedef dpoint_base_<Exact> super;

    public:
      stc_using(coord);

      coord  ind() const;
      coord& ind();

    protected:
      dpoint1d_();
    };



# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    dpoint1d_<Exact>::dpoint1d_()
    {
    }

    template <typename Exact>
    typename dpoint1d_<Exact>::coord
    dpoint1d_<Exact>::ind() const
    {
      return this->v_[0];
    }

    template <typename Exact>
    typename dpoint1d_<Exact>::coord &
    dpoint1d_<Exact>::ind()
    {
      return this->v_[0];
    }

# endif


  } // end of namespace oln::internal


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_DPOINT1D_HH
