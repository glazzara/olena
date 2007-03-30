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

#ifndef OLN_CORE_CONCEPT_FUNCTION_HH
# define OLN_CORE_CONCEPT_FUNCTION_HH

# include <oln/core/equipment.hh>




namespace oln
{


  /*

  // Fwd decl.
  namespace morpher {
    template <typename I, typename F> class two_way;
    template <typename I, typename F> class two_way_rw;
  }
  namespace value   { template <typename I, typename F> class two_way; }

  */

  
  template <typename Exact>
  struct Function : public Any<Exact>
  {
  protected:
    Function();
  };


  // Point -> Value.

  template <typename Exact>
  struct Function_p2v : public Function<Exact>
  {
  protected:
    Function_p2v();
  };


  // Point -> bool.

  template <typename Exact>
  struct Function_p2b : public Function<Exact>
  {
  protected:
    Function_p2b();
  };


  // Value -> Value.

  template <typename Exact>
  struct Function_v2v : public Function<Exact>
  {
  protected:
    Function_v2v();
  };


  /*

  // Value1 -> Value2  *and*  Value2 -> Value1.

  template <typename Exact>
  struct Function_v2w2v : public Function<Exact>
  {
  public:

    template <typename I>
    oln::morpher::two_way<I, Exact>
    operator()(oln::abstract::mutable_image<I>& input) const;

  protected:
    Function_v2w2v();
  };


  // (Image, Point) -> Value2  *and*  (Image, Point, Value2) -> Value1.

  template <typename Exact>
  struct Function_rw : public Function<Exact>
  {
  public:

    template <typename I>
    oln::morpher::two_way_rw<I, Exact>
    operator()(oln::abstract::mutable_image<I>& input) const;

  protected:
    Function_rw();
  };

  */


  // Point -> Point.

  template <typename Exact>
  struct Function_p2p : public Function<Exact>
  {
  protected:
    Function_p2p();
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  Function<Exact>::Function()
  {
  }

  template <typename Exact>
  Function_p2b<Exact>::Function_p2b()
  {
  }

  template <typename Exact>
  Function_p2v<Exact>::Function_p2v()
  {
  }

  template <typename Exact>
  Function_v2v<Exact>::Function_v2v()
  {
  }

  /*

  template <typename Exact>
  Function_v2w2v<Exact>::Function_v2w2v()
  {
  }

  template <typename Exact>
  Function_rw<Exact>::Function_rw()
  {
  }

  */

  template <typename Exact>
  Function_p2p<Exact>::Function_p2p()
  {
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_FUNCTION_HH
