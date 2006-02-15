// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef METALIC_PAIR_HH
# define METALIC_PAIR_HH

# include <mlc/bool.hh>
# include <mlc/uint.hh>




namespace mlc
{

  namespace internal
  {

    template <typename E1, typename E2, unsigned i>
    struct pair_elt_;

  } // end of namespace mlc::internal


  /*! \class mlc::pair_<E1, E2>
  **
  ** This class is FIXME  */

  template <typename E1, typename E2>
  struct pair_ : public mlc::abstract::type
  {
    static const unsigned size_value = 2;

    typedef E1 first_elt;
    typedef E2 second_elt;

    template <unsigned i>
    struct elt : private ensure_< or_< uint_equal_<i, 1>,
				       uint_equal_<i, 2> > >,
		 public internal::pair_elt_<E1, E2, i>
    {
    };
  };



  namespace internal
  {

    template <typename E1, typename E2>
    struct pair_elt_ <E1, E2, 1>
    {
      typedef E1 ret;
    };

    template <typename E1, typename E2>
    struct pair_elt_ <E1, E2, 2>
    {
      typedef E2 ret;
    };

  } // end of namespace mlc::internal
  
  

} // end of namespace mlc



# include <mlc/elt.hh>




#endif // ! METALIC_PAIR_HH
