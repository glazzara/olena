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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef METALIC_ABORT_HH
# define METALIC_ABORT_HH

# include <mlc/bexpr.hh>
# include <mlc/assert.hh>
# include <mlc/cmp.hh>
# include <mlc/is_a.hh>



namespace mlc
{

  namespace ERROR
  {
    struct FIRST_PARAMETER_OF_mlc_abort_SHOULD_NOT_BE_an_abstract_bexpr;

  } // end of namespace mlc::ERROR


  namespace internal
  {

    /*! \class mlc::always_false_<T>
    **
    ** FIXME: doc
    **
    ** \see mlc::abort_<T, err>
    **
    */

    template <typename T>
    struct always_false_

      : public bexpr_is_<false>
    {
    protected:
      always_false_() {}
    };

  } // end of mlc::internal



  /*! \class mlc::abort_<T, err>
  **
  ** FIXME: doc
  **
  ** T is important here; since there is an unknown type at compile-time
  ** the parsing can run through a routine that owns an abort_ instruction.
  **
  ** If there were no free type like this, the routine produces an error
  ** at compile-time (actually there is an abort!) even if this routine
  ** is not explicitly called in the code. 
  **
  ** \note \a T can be any type... except a bexpr so that we can
  ** detect that the user has used abort_ instead of assert_.
  **
  ** \see assert_<bexpr, err>
  **
  */

  template <typename T, typename err = no_error_message>
  struct abort_ :

//     private assert_< not_<mlc_is_a(T, abstract::bexpr)>,
// 		     mlc::ERROR::FIRST_PARAMETER_OF_mlc_abort_SHOULD_NOT_BE_an_abstract_bexpr >,

    public internal::always_false_<T>
  {
  protected:
    abort_() {}
  };





} // end of namespace mlc


#endif // ! METALIC_ABORT_HH
