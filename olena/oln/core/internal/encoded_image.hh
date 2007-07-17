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

#ifndef OLN_CORE_INTERNAL_ENCODED_IMAGE_HH
# define OLN_CORE_INTERNAL_ENCODED_IMAGE_HH

# include <vector>

# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/encoded_image_psite.hh>
# include <oln/core/internal/encoded_image_pset.hh>

namespace oln
{
  // Fwd decl.
  namespace internal { template <typename Exact>  struct encoded_image_; }

  // Super type.
  template <typename Exact>
  struct super_trait_< internal::encoded_image_<Exact> >
  {
    typedef internal::primitive_image_<Exact> ret;
  };

  // Virtual types
  template <typename Exact>
  struct vtypes< internal::encoded_image_<Exact> >
  {
# ifndef OLENA_USE_SCOOP_ALT
    typedef stc_deferred(point) point__;
    typedef internal::enc_image_psite_<point__> psite;
    typedef internal::enc_image_pset_<point__> pset;
# endif
    typedef mlc::none plain;
  };

# ifdef OLENA_USE_SCOOP_ALT
  template <typename Exact>
  struct single_vtype< internal::encoded_image_<Exact>, typedef_::psite >
  {
    typedef internal::enc_image_psite_< stc_type(Exact, point) > ret;
  };

  template <typename Exact>
  struct single_vtype< internal::encoded_image_<Exact>, typedef_::pset >
  {
    typedef internal::enc_image_pset_< stc_type(Exact, point) > ret;
  };
# endif

  namespace internal
  {
    // encoded_image_
    template <typename Exact>
    class encoded_image_ : public primitive_image_< Exact >
    {
      typedef encoded_image_<Exact> current;
      typedef primitive_image_< Exact > super;
    public:
      stc_using(pset);
      stc_using(psite);
      stc_using(data);

      const pset& impl_points() const;
      bool impl_owns_(const psite& p) const;
    protected:
      encoded_image_();
    };

# ifndef OLN_INCLUDE_ONLY



    template <typename Exact>
    encoded_image_<Exact>::encoded_image_()
    {
    }

    template <typename Exact>
    const typename encoded_image_<Exact>::pset&
    encoded_image_<Exact>::impl_points() const
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename Exact>
    bool
    encoded_image_<Exact>::impl_owns_(const typename encoded_image_<Exact>::psite& p) const
    {
      assert(this->has_data());
      return this->data_->first.has(p.start_);
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace internal
} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_ENCODED_IMAGE__HH
