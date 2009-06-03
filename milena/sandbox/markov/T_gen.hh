// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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



#ifndef MLN_TEMPERATURE_GENERATOR_HH
# define MLN_TEMPERATURE_GENERATOR_HH

class temperature_generator
{
public:

  temperature_generator(float t0, float alpha) : t_(t0), alpha_(alpha) {}

  operator float()
  {
    float tmp = t_;
    t_ *= alpha_;
    return tmp;
  }

private:
  float t_;
  float alpha_;
};

// Example.
// #include <iostream>
// int main()
// {
//   temperature_generator temp_gen(100, 0.9);
//   float t;

//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
//   t = temp_gen;
//   std::cout << t << std::endl;
// }

#endif // ! MLN_TEMPERATURE_GENERATOR_HH
