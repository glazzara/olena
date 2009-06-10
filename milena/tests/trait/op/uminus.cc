// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/trait/op/uminus.hh>
#include <mln/core/concept/object.hh>


template <typename E> struct Top_Concept;
template <typename E> struct Sub_Concept;


// Top_Concept.

template <> struct Top_Concept<void> { typedef mln::Object<void> super; };

template <typename E>
struct Top_Concept
{
  typedef Top_Concept<void> category;
protected:
  Top_Concept() {}
};


// Sub_Concept.

template <> struct Sub_Concept<void> { typedef Top_Concept<void> super; };

template <typename E>
struct Sub_Concept
{
  typedef Sub_Concept<void> category;
protected:
  Sub_Concept() {}
};


// test.

struct test : Sub_Concept< test >
{
  void is_test() {}
};


namespace mln
{

  namespace trait
  {

    // FIXME: Dead code?
//     template <>
//     struct p_setrecise_unary_< op::uminus, test >
//     {
//       typedef bool ret;
//     };

//     template <typename T>
//     struct set_unary_< op::uminus, Sub_Concept, T >
//     {
//       typedef int ret;
//     };

//     template <typename T>
//     struct set_unary_< op::uminus, Top_Concept, T >
//     {
//       typedef float ret;
//     };

  } // mln::trait

} // mln


int main()
{
  using namespace mln;

    // FIXME: Dead code?

//   {
//     mln_trait_op_uminus_(test) tmp; /* bool if precise */
//     bool* b = &tmp;
//     *b = true;
//   }

//   {
//     mln_trait_op_uminus_(test) tmp; /* int if no precise def and both sub and top defs */
//     void* v = tmp;
//   }

//   {
//     mln_trait_op_uminus_(test) tmp; /* float if only top def */
//     void* v = tmp;
//   }

//   {
//     mln_trait_op_uminus_(test) tmp; // test if no def here (default is id, given for Object)
//     tmp.is_test();
//   }

}
