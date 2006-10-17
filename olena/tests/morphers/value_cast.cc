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

/// Test oln::value_cast.cc.

#include <iostream>

#include <oln/basics2d.hh>
#include <oln/value/all.hh>
#include <oln/morpher/value_cast.hh>


template <typename T>
void print(T)
{
  std::cout << "?" << std::endl;
}


void print(int)
{
  std::cout << "int" << std::endl;
}

void print(float)
{
  std::cout << "float" << std::endl;
}

void print(unsigned char)
{
  std::cout << "unsigned char" << std::endl;
}




int main()
{
  using namespace oln;

  using oln::value::gl8;

  point2d p(0,0);

  {
    image2d<int> ima(1,1);
    print( value_cast<float>(ima)(p) );
  }

  {
    image2d<bool> ima(1,1);
    print( value_cast<int>(ima)(p) );
  }

  {
    image2d<gl8> ima(1,1);
    print( value_cast<unsigned char>(ima)(p) );
  }

}
