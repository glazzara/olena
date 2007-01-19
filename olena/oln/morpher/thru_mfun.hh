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

#ifndef OLN_MORPHER_THRU_MFUN_HH
# define OLN_MORPHER_THRU_MFUN_HH

# include <xtd/res.hh>
# include <xtd/abstract/open_nary_fun.hh>
# include <xtd/abstract/mutable_fun.hh>
# include <xtd/mexpr.hh>
# include <oln/morpher/internal/image_value_morpher.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Fun> struct thru_mfun;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Fun>
  struct set_super_type< morpher::thru_mfun<Image, Fun> >
  {
    typedef morpher::thru_mfun<Image, Fun> self_t;
    typedef morpher::internal::image_value_morpher<Image, self_t> ret;
  };


  template <typename Image, typename Fun>
  struct vtypes< morpher::thru_mfun<Image, Fun> >
  {
  private:
    typedef oln_vtype(Image, rvalue)  old_value_type;
    typedef morpher::thru_mfun<Image, Fun> self_t;
  public:
    typedef mlc::false_ is_computed_type;
    typedef typename xtd::res_<Fun, old_value_type>::ret value_type;
    typedef value_type& lvalue_type;

    typedef oln_value(self_t) rvalue_type;
  };


  namespace morpher
  {

    /// 'Image thru Function' morpher.
    template <typename Image, typename Fun>
    class thru_mfun : public internal::image_value_morpher< Image,
							    morpher::thru_mfun<Image, Fun> >
    {
    private:

      typedef thru_mfun<Image, Fun> self_t;
      typedef internal::image_value_morpher<Image, self_t> super_t;

      typedef oln_psite(self_t)  psite_t;
      typedef oln_rvalue(self_t) rvalue_t;
      typedef oln_lvalue(self_t) lvalue_t;

    public:

      thru_mfun(const Image& image, const Fun& fun);

      rvalue_t impl_op_read(const psite_t& p) const;
      lvalue_t impl_op_readwrite(const psite_t& p);

    protected:

      Fun fun_;
    };


    namespace ERROR
    {

      struct AN_xtd_mutator_ONLY_WORKS_ON_AN_oln_abstract_mutable_image;

    } // end of namespace oln::morpher::ERROR

  } // end of namespace oln::morpher

} // end of namespace oln



namespace xtd
{

  // open_nary_fun_<1, Fun>::operator()(A& a) const
  // where A is an oln::abstract::mutable_image<I>

  template <typename Fun, typename A>
  struct case_ < tag::mutable_fun_operator, mlc::pair_<Fun, A>, 1 >
    : where_< mlc_is_a(A, oln::abstract::image) >
  {
    typedef stc_to_exact(A) I;
    typedef oln::morpher::thru_mfun<I, Fun> res;
    typedef oln::morpher::thru_mfun<I, Fun> mutable_res;

    static res impl(const Fun& fun, // target
		    const oln::abstract::image<I>& ima)
    {
      mlc::assert_< mlc_is_a(A, oln::abstract::mutable_image),
	            oln::morpher::ERROR::AN_xtd_mutator_ONLY_WORKS_ON_AN_oln_abstract_mutable_image >::check();
      // FIXME: ima is const... so there may be a problem!
      res tmp(ima.exact(), fun);
      return tmp;
    }

    static res impl(const Fun& fun, // target
		    oln::abstract::image<I>& ima)
    {
      mlc::assert_< mlc_is_a(A, oln::abstract::mutable_image),
	            oln::morpher::ERROR::AN_xtd_mutator_ONLY_WORKS_ON_AN_oln_abstract_mutable_image >::check();
      res tmp(ima.exact(), fun);
      return tmp;
    }
  };

} // end of namespace xtd


# ifndef OLN_INCLUDE_ONLY
#  include "thru_mfun.hxx"    
# endif

#endif // ! OLN_MORPHER_THRU_MFUN_HH

