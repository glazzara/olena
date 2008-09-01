// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef METALIC_ARRAY_1D_HXX
# define METALIC_ARRAY_1D_HXX

# include <mlc/array/nd.hh>

namespace mlc
{

  //
  //  mlc::array1d_info
  //
  ////////////////////////////////////////
    

  // card_	-> total number of elements (1-indexed)

  // center_	-> position of the central element (0-indexed)
  //		   domain : [ 0, card [

  // i_	-> number of elements that have been eaten yet (0-indexed)
  //		   domain : [ 0, card ] -> watch out !!


  template < unsigned card_, unsigned center_ = card_ / 2, unsigned i_ = card_>
  struct array1d_info
  {
    enum {
      card = card_,
      center = center_,
      i = i_,
      well_formed = true,
      get_real_center = center_
    };

    typedef array1d_info< card_, center_, i + 1 > next_elt;

  };
    
  template < unsigned card_, unsigned i_ >
  struct array1d_info <card_, internal::unknown_, i_ >
  {
    enum {
      card = card_,
      center = internal::unknown_,
      i = i_,
      well_formed = true,
      get_real_center = i_ / 2
    };

    typedef array1d_info< card_, internal::unknown_, i + 1 > next_elt;

  };

  // fwd decl
  template<class, class> struct array1d;


  namespace internal 
  {

    // fwd decl

    template<class T, class Info>
    struct array1d_elt_;

    // for error messages

    template<class U>
    struct here_a_value_is_not_of_type_;


    //
    //  mlc::internal::array1d_start_ decl
    //
    ////////////////////////////////////////

    template<class T>
    struct array1d_start_ {

      array1d_elt_< T, array1d_info< unknown_, unknown_, 1 > > operator=(T val);
      array1d_elt_< T, array1d_info< unknown_, 0, 1 > > operator=(x_<T> val);
      array1d_elt_< T, array1d_info< unknown_, 0, 1 > > operator=(x_<void> val);

      T ptr[max_card_]; // could be static
    };



    //
    //  mlc::internal::array1d_elt_
    //
    ////////////////////////////////////////

    template<class T, class Info>
    struct array1d_elt_
    {
      typedef array1d_elt_<T, typename Info::next_elt> next_elt_t_;

      typedef array1d_elt_< T, array1d_info< Info::card, Info::i, Info::i + 1> > eat_center_t_;

      typedef array1d< array1d_info< Info::i, Info::get_real_center, Info::i > , T> array1d_t_;

    public:
	
      // Constructor

      array1d_elt_(T* ptr, array1d_start_<T>* arr) : ptr(ptr), arr(arr)
      {
      }



      //
      // Overloading "operator,"
      //


      // elt, elt

      next_elt_t_ operator,(T val)
      {
	is_true<Info::card == unknown_>::ensure();
	*ptr = val;
	return next_elt_t_(ptr + 1, arr);
      }


      // elt, x(elt)		-> center

      eat_center_t_ operator,(x_<T> val)
      {
	is_true<Info::center == unknown_>::ensure();
	*ptr = val.ue; // FIXME : give a *name* to this variable !!
	return eat_center_t_(ptr+1, arr);
      }


      // elt, x()		-> center

      eat_center_t_ operator,(x_<void>)
      {
	is_true<Info::center == unknown_>::ensure();
	*ptr = T(0);
	return eat_center_t_(ptr+1, arr);
      }


      // elt, end
	
      array1d_t_ operator,(end_type)
      {
	// array is well-formed :
	is_true<Info::well_formed>::ensure();
	// centering is automatic or user-defined :
	// (commented out to allow automatic centering on even sized arrays)
	// mlc::logical_or< mlc::eq< Info::i % 2, 1 >::ret, mlc::neq< Info::center, unknown_ >::ret >::ensure();
	return array1d_t_(arr->ptr);
      }
	

      // else -> error

      template<class U>
      void operator,(array1d< Info, U >);

      template<class U>
      here_a_value_is_not_of_type_<T> operator,(U u) const;

      T* ptr;
      array1d_start_<T>* arr;
    };



    //
    //  mlc::internal::array1d_start_  impl
    //
    ////////////////////////////////////////

    template<class T> inline 
    array1d_elt_< T, array1d_info< unknown_, unknown_, 1 > >
    array1d_start_<T>::operator=(T val)
    {
      ptr[0] = val;
      return array1d_elt_< T, array1d_info< unknown_, unknown_, 1 > >(ptr+1, this);
    }

    template<class T> inline 
    array1d_elt_< T, array1d_info< unknown_, 0, 1 > >
    array1d_start_<T>::operator=(x_<T> val)
    {
      ptr[0] = val.ue;
      // center <- 0
      return array1d_elt_< T, array1d_info< unknown_, 0, 1 > >(ptr+1, this);
    }

    template<class T> inline 
    array1d_elt_< T, array1d_info< unknown_, 0, 1 > >
    array1d_start_<T>::operator=(x_<void> val)
    {
      ptr[0] = T(0);
      // center <- 0
      return array1d_elt_< T, array1d_info< unknown_, 0, 1 > >(ptr+1,this);
    }

  } // end of internal

} // end of mlc

#endif // ! METALIC_ARRAY_1D_HXX
