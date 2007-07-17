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

#ifndef OLN_CORE_INTERNAL_PITER_ADAPTOR_HH
# define OLN_CORE_INTERNAL_PITER_ADAPTOR_HH

# include <oln/core/internal/iterator_on_points_base.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class piter_adaptor_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::piter_adaptor_<Exact> >
  {
    typedef internal::iterator_on_points_base_<Exact> ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::piter_adaptor_<Exact> >
  {
    typedef stc::abstract adapted;

    typedef stc_deferred(adapted) adapted__;
# ifndef OLENA_USE_SCOOP_ALT
    typedef stc::final< oln_point(adapted__) > point;
# else
    typedef stc::final< stc_deferred_from (adapted__, point) > point;
# endif
  };


  namespace internal
  {

    // Base implementation class for types of iterator on points
    // defined over other types of iterator on points.

    template <typename Exact>
    class piter_adaptor_ : public internal::iterator_on_points_base_<Exact>
    {
      typedef internal::iterator_on_points_base_<Exact> super;
    public:

      stc_using(point);
      stc_typename(adapted);

      piter_adaptor_(adapted p);

      // Default impl is delegation.
      void impl_start();
      void impl_next();
      void impl_invalidate();
      bool impl_is_valid() const;
      const point* impl_point_adr() const;

    protected:
      adapted p_;

    }; // end of class oln::internal::piter_adaptor_<Exact>



# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    piter_adaptor_<Exact>::piter_adaptor_(adapted p)
      : p_(p)
    {
    }

    template <typename Exact>
    void
    piter_adaptor_<Exact>::impl_start()
    {
      this->p_.start();
    }

    template <typename Exact>
    void
    piter_adaptor_<Exact>::impl_next()
    {
      this->p_.next();
    }

    template <typename Exact>
    void
    piter_adaptor_<Exact>::impl_invalidate()
    {
      this->p_.invalidate();
    }

    template <typename Exact>
    bool
    piter_adaptor_<Exact>::impl_is_valid() const
    {
      return this->p_.is_valid();
    }

    template <typename Exact>
    const typename piter_adaptor_<Exact>::point*
    piter_adaptor_<Exact>::impl_point_adr() const
    {
      return this->p_.point_adr();
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_PITER_ADAPTOR_HH
