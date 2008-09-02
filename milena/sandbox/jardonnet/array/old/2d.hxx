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

#ifndef METALIC_ARRAY_2D_HXX
# define METALIC_ARRAY_2D_HXX

# include <mlc/array/nd.hh>

namespace mlc
{

    //
    //  mlc::array2d_info
    //
    ////////////////////////////////////////
    

    // nrows_	-> total number of rows (1-indexed)
    // ncols_	-> total number of columns (1-indexed)

    // center_	-> position of the central element (0-indexed)
    //		   domain : [ 0, card [

    // i_	-> number of elements that have been eaten yet (0-indexed)
    //		   domain : [ 0, card ] -> watch out !!



    // Center is user-defined

    template < unsigned nrows_, 
	       unsigned ncols_,
	       unsigned center_ = ncols_ * (nrows_ / 2) + (ncols_ / 2),
	       unsigned i_ = nrows_ * ncols_>
    struct array2d_info
    {
      enum {
	nrows = nrows_,
	ncols = ncols_,
	center = center_,
	center_row = center_ / ncols_,
	center_col = center_ % ncols_,
	card = nrows_ * ncols_,
	i = i_,
	well_formed = ((i_ % ncols_) == 0),
	get_real_center = center_
      };

      typedef array2d_info< nrows_, ncols_, center_, i + 1 > next_elt;

    };
    
    // Center is defined automatically, if nrows and ncols are both odd

    template < unsigned nrows_, unsigned ncols_, unsigned i_ >
    struct array2d_info <nrows_, ncols_, internal::unknown_, i_ >
    {
      enum {
	nrows = nrows_,
	ncols = ncols_,
	center = internal::unknown_,
	i = i_,
	card = nrows_ * ncols_,
	well_formed = ((i_ % ncols_) == 0),
	get_real_center = i_ / 2
      };

      typedef array2d_info< nrows_, ncols_, internal::unknown_, i + 1 > next_elt;

    };

    // fwd decl
    template<class, class> struct array2d;


    namespace internal 
    {

      // fwd decl

      template<class T, class Info>
      struct array2d_elt_;

      // for error messages

      template<class U>
      struct here_a_value_is_not_of_type_;


      //
      //  mlc::internal::array2d_start_ decl
      //
      ////////////////////////////////////////

      template<class T>
      struct array2d_start_ {

	array2d_elt_< T, array2d_info< unknown_, unknown_, unknown_, 1 > > 
	operator=(T val);
	
	array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > > 
	operator=(x_<T> val);
	
	array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > > 
	operator=(x_<void> val);

	T ptr[max_card_]; // could be static
      };


      //
      //  mlc::internal::array2d_elt_
      //
      ////////////////////////////////////////

      template<class T, class Info>
      struct array2d_elt_
      {
	typedef array2d_elt_< T, typename Info::next_elt >
	next_elt_t_;

	typedef array2d_elt_< T, array2d_info< Info::nrows, Info::ncols, Info::i, Info::i + 1> >
	eat_center_t_;

	typedef array2d_elt_< T, array2d_info< Info::nrows, Info::i, Info::center, Info::i > >
	eat_lbrk_t_;

	typedef array2d< array2d_info< Info::i / Info::ncols, Info::ncols, Info::get_real_center, Info::i > , T>
	array2d_t_;

      public:
	
	// Constructor

	array2d_elt_(T* ptr, array2d_start_<T>* arr) : ptr(ptr), arr(arr)
	{
	}


	//
	// Overloading "operator,"
	//


	// elt, elt

	next_elt_t_ operator,(T val)
	{
	  is_true<Info::nrows == unknown_>::ensure();
	  *ptr = val;
	  return next_elt_t_(ptr + 1, arr);
	}


	// elt, x(elt)		-> center

	eat_center_t_ operator,(x_<T> val)
	{
	  is_true<Info::center == unknown_>::ensure();
	  *ptr = val.ue; // FIXME : give a *name* to this variable !!
	  return eat_center_t_(ptr + 1, arr);
	}


	// elt, x()		-> center

	eat_center_t_ operator,(x_<void>)
	{
	  is_true<Info::center == unknown_>::ensure();
	  *ptr = T(0);
	  return eat_center_t_(ptr + 1, arr);
	}


	// elt, lbrk

	eat_lbrk_t_ operator,(lbrk_)
	{
	  is_true<Info::ncols == unknown_>::ensure();
	  is_true<Info::ncols != 0>::ensure();
	  return eat_lbrk_t_(ptr, arr);
	}


	// elt, end
	
	array2d_t_ operator,(end_type)
	{
	  enum { nrows = Info::i / Info::ncols };
	  
	  // array is well-formed :
	  is_true<Info::well_formed == true>::ensure();
	  // centering is automatic or user-defined :
	  
	  is_true<Info::ncols != unknown_>::ensure();


	  // both nrows and ncols must be odd 
	  // or the center must be user-defined

	  is_true<	 
	    ((Info::ncols % 2 == 1) && (nrows % 2 == 1)) 
	    || (Info::center != unknown_)
	  >::ensure();
	  
	  return array2d_t_(arr->ptr);
	}
	

	// else -> error

	template<class U>
	void operator,(array2d< Info, U >);

	template<class U>
	here_a_value_is_not_of_type_<T> operator,(U u) const;

	T* ptr;
	array2d_start_<T>* arr;
      };



      //
      //  mlc::internal::array2d_start_  impl
      //
      ////////////////////////////////////////

      template<class T> inline 
      array2d_elt_< T, array2d_info< unknown_, unknown_, unknown_, 1 > >
      array2d_start_<T>::operator=(T val)
      {
	ptr[0] = val;
	return array2d_elt_< T, array2d_info< unknown_, unknown_, unknown_, 1 > >(ptr+1,this);
      }

      template<class T> inline 
      array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > >
      array2d_start_<T>::operator=(x_<T> val)
      {
	ptr[0] = val.ue;
	// center <- 0
	return array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > >(ptr+1,this);
      }

      template<class T> inline 
      array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > >
      array2d_start_<T>::operator=(x_<void> val)
      {
	ptr[0] = T(0);
	// center <- 0
	return array2d_elt_< T, array2d_info< unknown_, unknown_, 0, 1 > >(ptr+1,this);
      }

    } // end of internal

} // end of mlc

#endif // ! METALIC_ARRAY_2D_HXX
