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

#ifndef OLN_CORE_ABSTRACT_FUNCTIONS_HH
# define OLN_CORE_ABSTRACT_FUNCTIONS_HH

# include <stc/any.hh>
# include <oln/core/type.hh>
# include <oln/core/abstract/image.hh>


namespace oln
{


  // Fwd decl.
  namespace morpher {
    template <typename I, typename F> class two_way;
  }


  namespace abstract
  {

    // Point -> Value.

    template <typename E>
    struct fun_p2v : public virtual stc::any__simple<E>,
		     public oln::type
    {
    protected:
      fun_p2v();
    };


    // Value -> Value.

    template <typename E>
    struct fun_v2v : public virtual stc::any__simple<E>,
		     public oln::type
    {
    protected:
      fun_v2v();
    };


    // Value1 -> Value2  *and*  Value2 -> Value1.

    template <typename E>
    struct fun_v2w2v : public virtual stc::any__simple<E>,
		       public oln::type
    {
    public:
      template <typename I>
      oln::morpher::two_way<I, E>
      operator()(oln::abstract::mutable_image<I>& input) const;
    protected:
      fun_v2w2v();
    };


    // Point -> Point.

    template <typename E>
    struct fun_p2p : public virtual stc::any__simple<E>,
		     public oln::type
    {
    protected:
      fun_p2p();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    fun_p2v<E>::fun_p2v()
    {
    }

    template <typename E>
    fun_v2v<E>::fun_v2v()
    {
    }

    template <typename E>
    fun_v2w2v<E>::fun_v2w2v()
    {
    }

    template <typename E>
    fun_p2p<E>::fun_p2p()
    {
    }

# endif

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_FUNCTIONS_HH
