// Copyright (C) 2001, 2007, 2008 EPITA Research and Development
// Laboratory
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

// File: tour2.cc.

#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/core/gen/such_as.hh>
#include <oln/core/gen/torus_image.hh>
#include <oln/core/gen/pw_value.hh>
#include <oln/core/gen/fun_ops.hh>

#include <oln/data/fill.hh>
#include <oln/debug/fill.hh>
#include <oln/debug/println.hh>


// Note to the reader: you should have read the files tour1.cc and
// tour2.cc before starting with this present file.



// We have encapsulated an algorithm into a procedure which is forward
// declared below so that it can be used in the section 'main'.
template <typename I> void algo(const I& img);


// Some functions that will be useful in the following:
bool chessboard(oln::point2d p)
{
  return (p.row() + p.col()) % 2;
}



int main()
{
  using namespace oln;

  // First our domain is 2d box:
  box2d b(point2d(0, 0), point2d(2, 2));
  //              ^^^^           ^^^^
  //              from            to

  // We define a binary image with values on that box.
  image2d<bool> img(b);

  // With an array of Booleans (1 means true, 0 means false)...
  bool vals[] = { 1, 0, 0,
		  0, 1, 0,
		  0, 1, 1 };
  // ...the debug::fill routine allows for manually initializing
  // image data:
  debug::fill(img, vals);
  std::cout << "img = " << std::endl;
  debug::println(img);
  // img =
  // | - - 
  // - | - 
  // - | | 

  image2d<int> ima(b);          // An image of integers with the same
  box2d::piter p(ima.points()); // domain as img...
  int i = 0;
  for_all(p)
    ima(p) = i++; // ...and manually filled with values.

  std::cout << "ima = " << std::endl;
  debug::println(ima);
  // ima = 
  // 0 1 2 
  // 3 4 5 
  // 6 7 8 



  // The algorithm defined at the end of this file is very close to
  // the one of the tour former file.  The major difference is that it
  // does not rely on a window but on a neighborhood.

  // In image processing, we usually say that "an image has a given
  // neighborhood" or that "we associate/embed a neighborhood to/into
  // an image". In Olena, that is really the case: the image can
  // "have" a neighborhood, meaning that a neighborhood can be added
  // to an image in order to obtain an "image with a neighborhood".

  // Joining an image with a neighborhood is performed with the
  // operator '+':
  algo(ima + c4);  // c4 is the 2D neighborhood corresponding to
		   // 4-connectivity; such as many classical
		   // neighborhoods it is provided by Olena.
  // The result is given below.

  // ---input:
  // 0 1 2 
  // 1 2 3 
  // 2 3 4 
  // ---output:
  // 0: 1 1
  // 1: 0 2 2
  // 2: 1 3
  // 1: 0 2 2
  // 2: 1 1 3 3
  // 3: 2 2 4
  // 2: 1 3
  // 3: 2 2 4
  // 4: 3 3

  // That was expectable...


  // And now for a little test: what is the result of this code?
  {
    image2d<int> test(ima.points());
    data::fill(test, ima);
    (test + c4).at(1, 1) = 9;
    debug::println(test);
  }
  // and can you tell why?
  // The answers are given in the file tour3-test.txt


  // Now let us start experimenting the genericity of Olena!


  // First, imagine that you want to restrict the domain of ima to a
  // subset of points, a region or whatever.  For instance, the
  // chessboard function takes a point as argument and returns a
  // Boolean so it is like a predicate.  We can want to consider only
  // the points of ima "such as" this predicate is verified.  The
  // "such as" mathematical symbol is '|' so let's rock:

  algo((ima | chessboard) + c8);
  // gives:

  // ---input:
  //   1   
  // 3   5 
  //   7   
  // ---output:
  // 1: 3 5
  // 3: 1 7
  // 5: 1 7
  // 7: 3 5

  // where the blanks in printing the input image denote that the
  // corresponding points do NOT belong to the image domain.

  // Another similar example is based on the binary image created at
  // the beginning of this tour:
  algo((ima | img) + c8);
  // which gives:

  // ---input:
  // 0     
  //   4   
  //   7 8 
  // ---output:
  // 0: 4
  // 4: 0 7 8
  // 7: 4 8
  // 8: 4 7



  // Second, imagine that you want your initial image to get the
  // geodesy of a torus, that is, a 2D image wrapped on a torus.
  // Points located at the image boundary have neighbors; for
  // instance, the point denoted by the 'x' cross below has for
  // 4-connectivity neighbors: t, l, r, and b (respectively for top,
  // left, right, and bottom):

  // b o o o
  // o o o o
  // t o o o
  // x r o l

  // Let us try:
  algo(torus(ima) + c8); 
  // gives:

  // ---input:
  // 0 1 2 
  // 3 4 5 
  // 6 7 8 
  // ---output:
  // 0: 8 6 7 2 1 5 3 4
  // 1: 6 7 8 0 2 3 4 5
  // 2: 7 8 6 1 0 4 5 3
  // 3: 2 0 1 5 4 8 6 7
  // 4: 0 1 2 3 5 6 7 8
  // 5: 1 2 0 4 3 7 8 6
  // 6: 5 3 4 8 7 2 0 1
  // 7: 3 4 5 6 8 0 1 2
  // 8: 4 5 3 7 6 1 2 0



  // We can have both the torus geodesy and a sub-domain:

  algo(torus(ima | chessboard) + c8);
  algo(torus(ima | img) + c8);

  // which respectively give:

  // ---input:
  //   1   
  // 3   5 
  //   7   
  // ---output:
  // 1: 7 3 5
  // 3: 1 5 7
  // 5: 1 3 7
  // 7: 3 5 1

  // and:
  
  // ---input:
  // 0     
  //   4   
  //   7 8 
  // ---output:
  // 0: 8 7 4
  // 4: 0 7 8
  // 7: 4 8 0
  // 8: 4 7 0




  // Last, the way a predicate is defined can also rely on some image
  // values.  For that the user can on the fly provide an expression
  // built with the "pw_value" facility, where "pw_" means
  // "point-wise" for short:

  algo((ima | (pw_value(ima) < 4)) + c4);

  // In this example, "pw_value(ima)" is the function that represents
  // the point-wise value of the 'ima' image, that is, the function
  // "p -> ima(p)".  This naturally leads to:

  // ---input:
  // 0 1 2 
  // 3     
  // 
  // ---output:
  // 0: 1 3
  // 1: 0 2
  // 2: 1
  // 3: 0



  // From those examples, you should realize that:


  //  +-----------------------------------------------------------+
  //  |                                                           |
  //  |  The level of "genericity" provided by Olena is rather    |
  //  |  high; it means:                                          |
  //  |                                                           |
  //  |  - taking the image dimension you work on;                |
  //  |                                                           |
  //  |  - having the type of pixel values you need;              |
  //  |                                                           |
  //  |  - choosing the neighborhood you want;                    |
  //  |                                                           |
  //  |  - changing the geodesy if you need it;                   |
  //  |                                                           |
  //  |  - being able to restrict the image domain;               |
  //  |                                                           |
  //  |  - and many other features that are addressed further     |
  //  |    in the tour...                                         |
  //  |                                                           |
  //  +-----------------------------------------------------------+

}





// The algorithm 'algo':

template <typename I>
void algo(const I& img)
{
  std::cout << "---input:" << std::endl;
  oln::debug::print(img);
  std::cout << "---output:" << std::endl;

  oln_piter(I) p(img.points()); // p iterates on img points
  oln_niter(I) n(img, p);       // n iterates in img on neighbors of p

  for_all(p)
    {
      std::cout << oln::debug::format(img(p))
		<< ':';
      for_all(n)
	std::cout << ' '
		  << oln::debug::format(img(n));
      std::cout << std::endl;
    }

  std::cout << std::endl;
}
