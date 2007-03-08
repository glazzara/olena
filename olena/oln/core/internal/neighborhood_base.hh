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

#ifndef OLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
# define OLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH

# include <oln/core/concept/neighborhood.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class neighborhood_base_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::neighborhood_base_<Exact> >
  {
    typedef Neighborhood<Exact> ret;
  };


  /// Virtual types.
  template <typename Exact>
  struct vtypes< internal::neighborhood_base_<Exact> >
  {
    typedef stc::abstract point;

    typedef stc_deferred(point) point__;
    typedef stc::final< stc_type(point__, grid) >  grid;
  };


  namespace internal
  {

    /// Base class for implementation of neighborhoods class.

    template <typename Exact>
    class neighborhood_base_ : public Neighborhood< neighb_<Exact> >
    {
    protected:
      neighborhood_base_();

    }; // end of class oln::neighborhood_base_<Exact>



# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    neighborhood_base_<Exact>::neighborhood_base_()
    {
    }

# endif
  

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
