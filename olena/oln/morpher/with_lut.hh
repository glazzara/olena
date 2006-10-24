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
    typedef typename Lut::data_type value_type;
    // Look-up table type.
    typedef Lut lut_type;
  };

  // Rvalue.
  template <typename Image, typename Lut>
  struct single_vtype< morpher::with_lut<Image, Lut>, typedef_::rvalue_type >
  {
    typedef morpher::with_lut<Image, Lut> self_t;
    typedef oln_value(self_t) ret;
  };

  // FIXME: What about lvalue type?

//   // Lvalue.
//   template <typename Image>
//   struct single_vtype< morpher::slice<Image>, typedef_::lvalue_type >
//   {
//     typedef oln_type_of(Image, lvalue) ret;
//   };
  /// \}


  namespace morpher
  {
    /// Look-up table addition morpher.
    template <typename Image, typename Lut>
    class with_lut : public stc_get_supers(mlc_comma_1(with_lut<Image, Lut>))
      // FIXME: Ensure oln_value(Image) == Lut::data_type? Or just let
      // the ctor check this property?
    {
    private:
      typedef with_lut<Image, Lut> self_t;
      typedef stc_get_super(self_t) super_t;
      typedef Lut lut_t;
      typedef oln_type_of(self_t, value) value_t;
      typedef oln_type_of(self_t, rvalue) rvalue_t;
      typedef oln_type_of(self_t, psite) psite_t;
      // FIXME: Useful typedef?
//       typedef oln_type_of(Image, value) orig_value_t;

    public:
      with_lut(const Image& image, const Lut& lut);
      const lut_t& lut() const;

      rvalue_t impl_op_read(const psite_t& p) const;

      // FIXME: Implement impl_op_write() when there is value proxy?

    protected:
      lut_t lut_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Lut>
    with_lut<Image, Lut>::with_lut(const Image& image, const Lut& lut) :
      super_t(image),
      lut_(lut)
    {
      mlc::assert_equal_< oln_value(Image), typename Lut::key_type >::check();
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::rvalue_t
    with_lut<Image, Lut>::
      impl_op_read(const typename with_lut<Image, Lut>::psite_t& p) const
    {
      // FIXME: What if lut_ has no value for `this->image_(p)'?  At
      // least, document the behavior of this method (will it abort,
      // does the LUT have to provide a default value, etc.)
      return lut_(this->image_(p));
    }

    template <typename Image, typename Lut>
    const typename with_lut<Image, Lut>::lut_t&
    with_lut<Image, Lut>::lut() const
    {
      return lut_;
    }

# endif

  } // end of namespace oln::morpher



  template <typename I, typename K, typename D>
  morpher::with_lut< I, lookup_table<K, D> >
  operator + (const abstract::image<I>& image,
	      const lookup_table<K, D>& lut);


# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename K, typename D>
  morpher::with_lut< I, lookup_table<K, D> >
  operator + (const abstract::image<I>& image,
	      const lookup_table<K, D>& lut)
  {
    typedef lookup_table<K, D> lut_t;
    mlc::assert_equal_< oln_value(I), typename lut_t::key_type >::check();
    morpher::with_lut<I, lut_t> tmp(image.exact(), lut);
    return tmp;
  }

# endif

} // end of namespace oln


#endif // ! OLN_MORPHER_WITH_LUT_HH
