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

#ifndef OLN_MORPHER_WITH_LUT_HH
# define OLN_MORPHER_WITH_LUT_HH

# include <oln/morpher/internal/image_value_morpher.hh>
# include <oln/core/lookup_table.hh>
# include <oln/value/lut_value_proxy.hh>
# include <oln/core/gen/fwd_viter_lut.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Lut> struct with_lut;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Lut>
  struct set_super_type< morpher::with_lut<Image, Lut> >
  {
    typedef morpher::with_lut<Image, Lut> self_t;
    typedef morpher::internal::image_value_morpher<Image, self_t> ret;
  };

  /// Virtual types associated to oln::morpher::with_lut<Image, Lut>.
  /// \{
  template <typename Image, typename Lut>
  struct vtypes< morpher::with_lut<Image, Lut> >
  {
  private:
    typedef oln_type_of(Image, rvalue) orig_value_type;
  public:
    typedef mlc::true_ is_computed_type;
    // Value type.
    typedef typename Lut::new_value_type value_type;
    // Look-up table type.
    typedef Lut lut_type;

    // rvalue_type: see below.
    /* lvalue_type: undefined
       (see oln/morpher/internal/image_value_morpher.hh).  */

    typedef value::lut_value_proxy<Lut> value_proxy_type;
    /* Mutability.
       As the virtual type `lvalue' is undefined in with_lut<>, it is
       not point-wise mutable.  However, it is value-wise mutable,
       i.e., the values of the look-up table can be modified through
       a mutable value proxy.  */
    typedef value::mutable_lut_value_proxy<Lut> mutable_value_proxy_type;

    // fwd_viter_type: see below.
    // FIXME: implement bkd_viter_type!
  };

  // Rvalue.
  template <typename Image, typename Lut>
  struct single_vtype< morpher::with_lut<Image, Lut>, typedef_::rvalue_type >
  {
  private:
    typedef morpher::with_lut<Image, Lut> self_t;
  public:
    typedef oln_value(self_t) ret;
  };

  /// Const forward viter vtype of morpher::with_lut.
  template <typename Image, typename Lut>
  struct single_vtype< morpher::with_lut<Image, Lut>,
		       typedef_::fwd_viter_type >
  {
    typedef fwd_viter_lut<Lut> ret;
  };
  /// \}


  // FIXME: What about bkd_viter_type on morpher::with_lut?



  namespace morpher
  {
    /// Look-up table addition morpher.
    template <typename Image, typename Lut>
    class with_lut : public stc_get_supers(mlc_comma_1(with_lut<Image, Lut>))
      // FIXME: Ensure oln_value(Image) == Lut::new_value_type? Or just let
      // the ctor check this property?
    {
      typedef with_lut<Image, Lut> self_t;
      typedef stc_get_super(self_t) super_t;

    public:
      typedef Lut lut_type;
      typedef oln_type_of(self_t, value) value_type;
      typedef oln_type_of(self_t, rvalue) rvalue_type;
      typedef oln_type_of(self_t, psite) psite_type;
      typedef oln_type_of(self_t, value_proxy) value_proxy_type;
      typedef oln_type_of(self_t, mutable_value_proxy) mutable_value_proxy_type;

    public:
      with_lut(const Image& image, const Lut& lut);
      const lut_type& lut() const;

      rvalue_type impl_op_read(const psite_type& p) const;

      // FIXME: Constness of this method?
      value_proxy_type         impl_value(const value_type& v) const;
      mutable_value_proxy_type impl_value(const value_type& v);

    protected:
      lut_type lut_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Lut>
    with_lut<Image, Lut>::with_lut(const Image& image, const Lut& lut) :
      super_t(image),
      lut_(lut)
    {
      mlc::assert_equal_< oln_value(Image), typename Lut::orig_value_type >::check();
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::rvalue_type
    with_lut<Image, Lut>::impl_op_read(const typename with_lut<Image, Lut>::psite_type& p) const
    {
      // FIXME: What if lut_ has no value for `this->image_(p)'?  At
      // least, document the behavior of this method (will it abort,
      // does the LUT have to provide a default value, etc.)
      return lut_(this->image_(p));
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::value_proxy_type
    with_lut<Image, Lut>::impl_value(const value_type& v) const
    {
      return value_proxy_type(lut_, v);
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::mutable_value_proxy_type
    with_lut<Image, Lut>::impl_value(const value_type& v)
    {
      return mutable_value_proxy_type(lut_, v);
    }

    template <typename Image, typename Lut>
    const typename with_lut<Image, Lut>::lut_type&
    with_lut<Image, Lut>::lut() const
    {
      return lut_;
    }

# endif

  } // end of namespace oln::morpher


  // FIXME: Provide a version of op+ where the LUT is const?

  template <typename I, typename K, typename D>
  morpher::with_lut< I, lookup_table<K, D> >
  operator + (const abstract::image<I>& image,
	      lookup_table<K, D>& lut);


# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename K, typename D>
  morpher::with_lut< I, lookup_table<K, D> >
  operator + (const abstract::image<I>& image,
	      lookup_table<K, D>& lut)
  {
    typedef lookup_table<K, D> lut_type;
    mlc::assert_equal_< oln_value(I), typename lut_type::orig_value_type >::check();
    morpher::with_lut<I, lut_type> tmp(image.exact(), lut);
    return tmp;
  }

# endif

} // end of namespace oln


#endif // ! OLN_MORPHER_WITH_LUT_HH
