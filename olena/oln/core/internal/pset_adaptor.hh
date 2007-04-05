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

#ifndef OLN_CORE_INTERNAL_PSET_ADAPTOR_HH
# define OLN_CORE_INTERNAL_PSET_ADAPTOR_HH

# include <oln/core/internal/point_set_base.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class pset_adaptor_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::pset_adaptor_<Exact> >
  {
    typedef internal::point_set_base_<Exact> ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::pset_adaptor_<Exact> >
  {
    typedef stc::abstract adapted;

    typedef stc_deferred(adapted) adapted__;
    typedef stc::final< oln_point(adapted__) >     point;
    typedef stc::final< oln_fwd_piter(adapted__) > fwd_piter;
    typedef stc::final< oln_bkd_piter(adapted__) > bkd_piter;
    typedef stc::final< oln_box(adapted__) >       box;
  };


  namespace internal
  {

    // Base implementation class for types of iterator on points
    // defined over other types of iterator on points.

    template <typename Exact>
    class pset_adaptor_ : public internal::point_set_base_<Exact>
    {
      typedef internal::point_set_base_<Exact> super;
    public:
      stc_typename(adapted);

      stc_using(point);
      stc_using(box);

      unsigned impl_npoints() const;
      bool impl_has(const point& p) const;
      const box& impl_bbox() const;

      const adapted& adapted_() const { return this->ps_; }

    protected:
      adapted ps_;
      pset_adaptor_();
      pset_adaptor_(const adapted& ps);

    }; // end of class oln::internal::pset_adaptor_<Exact>



# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    pset_adaptor_<Exact>::pset_adaptor_()
      : ps_()
    {
    }

    template <typename Exact>
    pset_adaptor_<Exact>::pset_adaptor_(const adapted& ps)
      : ps_(ps)
    {
    }

    template <typename Exact>
    unsigned
    pset_adaptor_<Exact>::impl_npoints() const
    {
      return this->ps_.npoints();
    }

    template <typename Exact>
    bool
    pset_adaptor_<Exact>::impl_has(const typename pset_adaptor_<Exact>::point& p) const
    {
      return this->ps_.has(p);
    }

    template <typename Exact>
    const typename pset_adaptor_<Exact>::box&
    pset_adaptor_<Exact>::impl_bbox() const
    {
      return this->ps_.bbox();
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_PSET_ADAPTOR_HH
