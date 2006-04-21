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

#ifndef METALIC_ABSTRACT_BEXPR_HH
# define METALIC_ABSTRACT_BEXPR_HH

# include <mlc/abstract/type.hh>
# include <mlc/flags.hh>



namespace mlc
{

  namespace abstract
  {

    /*! \class mlc::abstract::bexpr
    **
    ** Abstract base class for mlc Boolean expression types.
    **
    ** When you define a new class for a Boolean expression type, you
    ** should not directly derive from this class from fom its
    ** subclass: bexpr_<b>.
    **
    ** \see bexpr_<b>
    */

    struct bexpr : public mlc::abstract::type
    {
      // typedef void eval;
    };


  } // end of namespace mlc::abstract

} // end of namespace mlc



#endif // ! METALIC_ABSTRACT_BEXPR_HH
