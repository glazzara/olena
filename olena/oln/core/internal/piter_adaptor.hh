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

# include <oln/core/concept/iterator_on_points.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class piter_adaptor_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::piter_adaptor_<Exact> >
  {
    typedef Iterator_on_Points<Exact> ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::piter_adaptor_<Exact> >
  {
    typedef stc::abstract point;
    typedef stc::abstract adapted;
  };


  namespace internal
  {

    // Base implementation class for types of iterator on points
    // defined over other types of iterator on points.

    template <typename Exact>
    class piter_adaptor_ : public Iterator_on_Points<Exact>
    {
      typedef Iterator_on_Points<Exact> super;
    public:

      stc_using(point);
      stc_typename(adapted);

      piter_adaptor_(adapted p);

      // Default impl is delegation.
      void impl_start();
      void impl_next();
      void impl_invalidate();
      bool impl_is_valid() const;
      point        impl_to_point()  const;
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
    typename piter_adaptor_<Exact>::point
    piter_adaptor_<Exact>::impl_to_point() const
    {
      return this->p_.to_point();
    }
    
    template <typename Exact>
    const typename piter_adaptor_<Exact>::point*
    piter_adaptor_<Exact>::impl_point_adr() const
    {
      return this->p_.point_adr();
    }

# endif

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_PITER_ADAPTOR_HH
