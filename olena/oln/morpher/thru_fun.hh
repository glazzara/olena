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

#ifndef OLN_MORPHER_THRU_FUN
# define OLN_MORPHER_THRU_FUN

# include <xtd/abstract/open_nary_fun.hh>
# include <xtd/res.hh>
# include <oln/morpher/internal/image_value_morpher.hh>



namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Fun> struct thru_fun;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Fun>
  struct set_super_type< morpher::thru_fun<Image, Fun> >
  {
    typedef morpher::thru_fun<Image, Fun> self_t;
    typedef morpher::internal::image_value_morpher<Image, self_t> ret;
  };


  template <typename Image, typename Fun>
  struct vtypes< morpher::thru_fun<Image, Fun> >
  {
  private:
    typedef oln_type_of(Image, rvalue)     old_value_type;
  public:
    typedef mlc::true_ is_computed_type;
    typedef xtd_res_1(Fun, old_value_type) value_type;
  };


  template <typename Image, typename Fun>
  struct single_vtype< morpher::thru_fun<Image, Fun>, typedef_::rvalue_type >
  {
    typedef morpher::thru_fun<Image, Fun> self_t;
    typedef oln_type_of(self_t, value) ret;
  };
  


  namespace morpher
  {

    /// 'Image thru Function' morpher.
    template <typename Image, typename Fun>
    class thru_fun : public internal::image_value_morpher< Image,
							   thru_fun<Image, Fun> >
    {
    private:

      typedef thru_fun<Image, Fun> self_t;
      typedef internal::image_value_morpher<Image, self_t> super_t;

      typedef oln_type_of(self_t, psite)  psite_t;
      typedef oln_type_of(self_t, rvalue) rvalue_t;

    public:

      thru_fun(const Image& image, const Fun& fun);

      rvalue_t impl_op_read(const psite_t& p) const;

    protected:

      Fun fun_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Fun>
    thru_fun<Image, Fun>::thru_fun(const Image& image, const Fun& fun) :
      super_t(image),
      fun_(fun)
    {
    }
    
    template <typename Image, typename Fun>
    typename thru_fun<Image, Fun>::rvalue_t
    thru_fun<Image, Fun>::impl_op_read(const typename thru_fun<Image, Fun>::psite_t& p) const
    {
      return fun_(this->image_(p));
    }
    
# endif

  } // end of namespace oln::morpher


} // end of namespace oln




namespace xtd
{

  // open_nary_fun_<1, Fun>::operator()(const A& a) const
  // where A is an oln::abstract::image<I>

  template <typename Fun, typename A>
  struct case_ < tag::fun_operator_1, mlc::pair_<Fun, A>, 2 >
    : where_< mlc_is_a(A, oln::abstract::image) >
  {
    typedef stc_to_exact(A) I;
    typedef oln::morpher::thru_fun<I, Fun> res;

    static res impl(const Fun& fun, // target
		    const oln::abstract::image<I>& ima)
    {
      res tmp(ima.exact(), fun);
      return tmp;
    }
  };

} // end of namespace xtd




#endif // ! OLN_MORPHER_THRU_FUN
