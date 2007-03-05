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

#ifndef OLN_CORE_GEN_PLIST_HH
# define OLN_CORE_GEN_PLIST_HH

# include <list>
# include <oln/core/internal/point_set_std_based.hh>


namespace oln
{

  /// Fwd decl.
  template <typename P> class plist_;


  // Super type.
  template <typename P>
  struct super_trait_< plist_<P> >
  {
    typedef plist_<P> current__;
    typedef internal::point_set_std_based_<current__> ret;
  };


  /// Virtual types.
  template <typename P>
  struct vtypes< plist_<P> >
  {
    typedef std::list<P> std_container;
  };


  /// Point set class based on std::list.

  template <typename P>
  class plist_ : public internal::point_set_std_based_< plist_<P> >
  {
  public:
      
    plist_();
    plist_<P>&  append(const P& p);
    plist_<P>& prepend(const P& p);

  }; // end of class oln::plist_<P>.



# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  plist_<P>::plist_()
  {
  }

  template <typename P>
  plist_<P>& plist_<P>::append(const P& p)
  {
    this->con_.push_back(p);
    this->take_(p);
    return *this;
  }

  template <typename P>
  plist_<P>& plist_<P>::prepend(const P& p)
  {
    this->con_.push_front(p);
    this->take_(p);
    return *this;
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PLIST_HH
