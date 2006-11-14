// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_MORPHER_ADD_ISUBSET
# define OLN_MORPHER_ADD_ISUBSET

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/type/binary.hh>
# include <oln/morpher/internal/image_extension.hh>
# include <oln/core/gen/topo_add_isubset.hh>
# include <oln/core/gen/piter_isubset.hh>

# include <xtd/abstract/fun_nary_expr.hh>
# include <oln/core/gen/pw_value.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Isubset> struct add_isubset;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Isubset>
  struct set_super_type< morpher::add_isubset<Image, Isubset> >
  {
    typedef morpher::add_isubset<Image, Isubset> self_t;
    typedef morpher::internal::image_extension<Image, self_t> ret;
  };

  template <typename Image, typename Isubset>
  struct vtypes< morpher::add_isubset<Image, Isubset> >
  {
    // Topology type.
    typedef topo_add_isubset< oln_topo(Image), Isubset > topo_type;

    // Isubset type.
    typedef Isubset subset_type;
  };


  namespace morpher
  {
    /// Isubset addition morpher.
    template <typename Image, typename Isubset>
    class add_isubset : public morpher::internal::image_extension< Image, 
								   add_isubset<Image, Isubset> >
    // FIXME: Uncomment. stc_get_supers(mlc_comma_1(add_isubset<Image, Isubset>))
    {
    private:

      typedef add_isubset<Image, Isubset> self_t;
      typedef internal::image_extension<Image, self_t> super_t;
      typedef oln_topo(self_t) topo_t;
      // FIXME: use of oln_direct_type_of here!!! otherwise we get some compilation trouble...

    public:

      // FIXME: Handle the constness.

      add_isubset(const Image& image, const Isubset& isubset);
      const topo_t& impl_topo() const;

    protected:
      topo_t topo_;
    };



# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Isubset>
    add_isubset<Image, Isubset>::add_isubset(const Image& image, const Isubset& isubset) :
      super_t(image),
      topo_(image.topo(), isubset)
    {
      // mlc::assert_equal_<oln_type_of(Image, grid), oln_type_of(Isubset, grid)>::check();
      // FIXME: check that Image is without a isubset
    }

    template <typename Image, typename Isubset>
    const typename add_isubset<Image, Isubset>::topo_t&
    add_isubset<Image, Isubset>::impl_topo() const
    {
      return topo_;
    }

# endif


  } // end of namespace oln::morpher



  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
	      const abstract::binary_image<S>& isubset);

  // FIXME: Register this operator.
  // FIXME: Add mutable version.


  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
	      const xtd::abstract::fun_nary_expr_<1,S>& fsubset);



# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
	      const abstract::binary_image<S>& isubset)
  {
    mlc::assert_equal_<oln_type_of(I, grid), oln_type_of(S, grid)>::check();
    // FIXME: check that Image does not have yet a subset
    morpher::add_isubset<I, S> tmp(image.exact(), isubset.exact());
    return tmp;
  }

  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
	      const xtd::abstract::fun_nary_expr_<1,S>& fsubset)
  {
    morpher::add_isubset<I, S> tmp(image.exact(), fsubset.exact_());
    return tmp;
  }

# endif

} // end of namespace oln


#endif // ! OLN_MORPHER_ADD_ISUBSET
