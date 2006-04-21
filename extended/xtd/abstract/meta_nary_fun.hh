// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef EXTENDED_ABSTRACT_META_NARY_FUN_HH
# define EXTENDED_ABSTRACT_META_NARY_FUN_HH

# include <xtd/abstract/meta_fun.hh>
# include <xtd/abstract/open_nary_fun.hh>



namespace xtd
{

  namespace abstract
  {


    /*! \class xtd::abstract::meta_nary_fun_<n, E>
    **
    ** Abstract base class for meta functions with an explicit number
    ** of arguments.
    **
    ** Parameter n is the number of arguments with n being 1, 2, or 3.
    **
    ** Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class meta_nary_fun_;



    /*! \class xtd::abstract::meta_nary_fun_<0, E>
    **
    ** Abstract base class for meta functions taking no argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 0, E >

      : public meta_fun_<E>,
	public open_nary_fun_<0, E>
    {
    protected:
      meta_nary_fun_() {}
    };



    /*! \class xtd::abstract::meta_nary_fun_<1, E>
    **
    ** Abstract base class for meta functions taking one argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 1, E >

      : public meta_fun_<E>,
	public open_nary_fun_<1, E>
    {
    protected:
      meta_nary_fun_() {}
    };



    /*! \class xtd::abstract::meta_nary_fun_<2, E>
    **
    ** Abstract base class for meta functions taking two arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 2, E >

      : public meta_fun_<E>,
	public open_nary_fun_<2, E>
    {
    protected:
      meta_nary_fun_() {}
    };



    /*! \class xtd::abstract::meta_nary_fun_<3, E>
    **
    ** Abstract base class for meta functions taking three arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 3, E >

      : public meta_fun_<E>,
	public open_nary_fun_<3, E>
    {
    protected:
      meta_nary_fun_() {}
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_META_NARY_FUN_HH
