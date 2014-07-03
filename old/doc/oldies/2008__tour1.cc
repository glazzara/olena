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

// File: tour1.cc.

#include <oln/core/1d/image1d.hh>

#include <oln/arith/plus.hh>    // arith

#include <oln/data/fill.hh>    // data
#include <oln/data/paste.hh>

#include <oln/debug/println.hh> // debug


int main()
{
  // Olena is organized in a namespace hierarchy.  Everything is
  // declared by Olena within the 'oln::' namespace, and possibly a
  // sub-namespace such as 'oln::arith::' (arithmetic operations on
  // images), 'oln::morpho::' (morphological operations), etc.  For
  // the sake of simplicity, we will neglect the 'oln::' prefix in
  // this file.

  using namespace oln;

  // There are three kinds of image dimensions supported for the
  // moment: 1D, 2D, and 3D.  The respective basic image types are
  // respectively image1d, image2d, and image3d.  All are template'd,
  // parametrized by the kind of value associated to each point.
  //
  // We will start to use the 1D images, because there are less values
  // to assign (!) and because they are easy to display.  Although we
  // will be in 1D, we will talk about "points" and "pixels", just
  // like in the classical 2D world.

  // Here are the two most basic ways to build an image:

  image1d<int> img1a;	   // which builds an empty image;
  image1d<int> img1b(20);  // which builds an image with 20 pixels.

  // The type of values enclosed in those images is 'int' for
  // integers.

  // The empty image has no data and its definition domain is still
  // unset.  We do not know yet the number of pixels it contains.
  // However, it is really useful to have such an "empty image"
  // because it is a placeholder for the result of some processing, or
  // another image.

  // Trying to access the pixel value from an empty image leads to an
  // error at run-time.

  // Assigning an image to another one is NOT a costly operation.
  // Actually img1a behaves like some mathematical variable.  Put
  // differently it is just a name to designate an image.  After the
  // following instruction

  img1a = img1b;

  // Both variables img1a and img1b can be used equivalently to access
  // the same data.  Just like in maths we can state that "img1a IS
  // img2b" and we only have ONE actual image in memory.  The image
  // definition---its domain and its pixel values--is shared by img1a
  // AND img1b.


  // Since an image is a function that maps values to points, to read
  // or modify a pixel value, we just the same notation than the one
  // of functions:
 
  img1a.at(0)  = 7;   // Assign value  7 to the  1st pixel of img1a.
  img1a.at(10) = 51;  // Assign value 51 to the 11th pixel of img1a.

  std::cout << "img1b.at(10) = " << img1b.at(10)
	    << std::endl; // Print 51 since img1b is img1a.

  // As you can see, Olena provides a unique easy syntax to access an
  // image value, whatever we want to read this value or to modify it:
  // "ima(p)", where p is a variable that represents a point of the 1D
  // domain.  In "val = ima(p)" we read the pixel value, whereas in
  // "ima(p) = new_val" we write in an image.  For 1D images, a 1D
  // point behaves just like an index so the user can write "ima(i)"
  // with i being an integer.


  // A single image variable can be used to designate one image and
  // then another one.

  image1d<int> img2a(20); // An image (not empty).
  image1d<int> img2b(20); // Another one (not empty).
  image1d<int> img;       // A variable.

  img = img2a; // img is img2a
  img.at(5) = 19; // so img2a at index 5 is set to 19.

  img = img2b; // img is NOW img2b
  img.at(5) = 69; // so img2b at index 5 is set to 69.

  std::cout << (100 * img2a.at(5) + img2b.at(5)) // Gives 1969.
	    << std::endl;



  // In the Olena library, all image types behave like image1d:
  //
  //  +-----------------------------------------------------------+
  //  |                                                           |
  //  |  - an "empty" image actually is a mathematical variable   |
  //  |                                                           |
  //  |        => just think in a mathemetical way when dealing   |
  //  |           with images;                                    |
  //  |                                                           |
  //  |                                                           |
  //  |  - no dynamic memory allocation/deallocation is required  |
  //  |                                                           |
  //  |        the user never has to use "new / delete" (the C++  |
  //  |        equivalent for the C "malloc / free") so she does  |
  //  |        not have to manipulate pointers or to directly     |
  //  |        access memory                                      |
  //  |                                                           |
  //  |        => Olena prevents the user from making mistakes;   |
  //  |                                                           |
  //  |                                                           |
  //  |  - image data/values can be shared between several        |
  //  |    variables and the memory used for image data is        |
  //  |    handled by the library                                 |
  //  |                                                           |
  //  |        => memory management is automatic.                 |
  //  |                                                           |
  //  |                                                           |
  //  +-----------------------------------------------------------+




  image1d<char> img3a(0, 19);  // These 1D image data go from the
                               // index 0 to the index 19; it thus has
                               // 20 points.

  // Memory has been allocated so data can be stored but pixel values
  // have not been initialized yet.  So we fill img3a, that is, all
  // pixels of img3a, with the value 'a':
  data::fill(img3a, 'a');

  // The "fill" algorithm is located in the sub-namespace "data"
  // since this algorithm deals with the "data" of pixel values.

  // The full name of this routine is "oln::data::fill".  To access
  // to a particular algorithm, the proper file shall be included.
  // The file names of algorithms strictly map their C++ name; so
  // oln::data::fill is defined in the file "oln/data/fill.hh".

  // Most algorithms in Olena are constructed following the classical
  // scheme: "output algo(input)", where the input image is only read.
  // However some few algorithms take an input image in order to
  // modify it.

  // Obsolete:  To enforce this particular feature, the user shall
  // Obsolete:  explicitly state that the image is provided so that its data is
  // Obsolete:  modified "inplace".

  // Obsolete:  The algorithm call shall be "data::fill(inplace(ima), val)".
  // Obsolete:  When forgetting the "inplace(..)" statement it does not compile.


  // We then define below a second image to play with.  As you can see
  // this image has data for the indices 5 to 14 (so it has 10
  // points).  The definition domain of a 1D image can start from
  // any index, even a negative one.
  image1d<char> img3b(5, 14);

  // We initialize the image values.
  data::fill(img3b, 'b');

  // Last we now paste the contents of img3b in img3a...
  data::paste(img3b, img3a);

  // ...and print the result.
  debug::println(img3a);

  // Before pasting, the couple of images looked like:
  //
  // index = 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1
  //         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
  //                           
  // img3a = a a a a a a a a a a a a a a a a a a a a
  // img3b =           b b b b b b b b b b
  //
  // so after pasting we get:
  // img3a = a a a a a b b b b b b b b b b a a a a a
  //
  // Just remember that a point is an absolute location on the
  // discrete domain.  For the 1D case, an index is thus NOT an
  // offset.  When we write "imgb(5)", we designate the first pixel of
  // imgb since this image starts at index 5.


  // With this simple example we can see that images defined on
  // different domains (or set of points) can interoperate.  The set
  // of points an image is defined on can be accessed and printed.
  // The following code:

  std::cout << "img3a.points() = " << img3a.points() << std::endl
	    << "img3b.points() = " << img3b.points() << std::endl;

  // gives:
  // img3a.points() = { (0) .. (19) }
  // img3b.points() = { (5) .. (14) }

  // The notion of point sets plays an important role in Olena.  Many
  // tests are performed at run-time to ensure that the program is
  // correct.

  // For instance, the algorithm data::paste tests that the set of
  // points of img3b (whose values are to be pasted) is a subset of
  // the destination image.  We can reproduce this same test:

  std::cout << (img3b.points() <= img3a.points() ?
		"true" :
		"false") << std::endl;  // which gives 'true'.

  // Accessing the value of an image at a given point also performs a
  // test to make sure that this point belongs to the image domain
  // (and consequently that data access in memory is valid).  For
  // instance, calling "img3a(9)" actually runs a code equivalent to
  // this test:

  point1d p(9);
  std::cout << (img3a.points().has(p) ?
		"true" :
		"false") << std::endl; // which gives 'true'.

  // The type of 1D image points is "point1d".  The 1D image type
  // features the method "operator()(point1d p)"; when we write
  // "img3a(9)" it is just a shortcut for "img3a( point1d(9) )".

  // When having a point we can access its index following:
  std::cout << p.ind() << std::endl;  // which gives 9.

  // To know if a point belongs to an image domain or not, we can run
  // this shorter test:
  std::cout << (img3a.has(p) ?
		"true" :
		"false") << std::endl; // which gives 'true'.


  // Since the notion of point is independent from the image it
  // applies on, we can form expressions where p is used on several
  // images:

  img3a(p) = 'M', img3b(p) = 'W';  // At index 9, both values change.
  debug::println(img3a);
  debug::println(img3b);
  // we obtain:
  // a a a a a b b b b M b b b b b a a a a a
  // b b b b W b b b b b


  // To modify all the values of an image we can iterate over its
  // domain with an index i:

  for (int i = 5; i <= 14; ++i)
    img3b.at(i) = 'a' + i - 5;
  debug::println(img3b);
  // a b c d e f g h i j

  // There is a safer way to iterate that prevents from providing a
  // wrong value for the minimum or maximum index value:
  for (int i = img3b.min_ind(); i <= img3b.max_ind(); ++i)
    img3b.at(i) = 'b' + i - img3b.min_ind();
  debug::println(img3b);
  // b c d e f g h i j k


  // The Olena library was built with the following rationale:
  //
  //  +-----------------------------------------------------------+
  //  |                                                           |
  //  |  If you do not want to be "generic", if you are used to   |
  //  |  the classical way of browsing pixel values, you can!     |
  //  |                                                           |
  //  +-----------------------------------------------------------+


  // Indeed, the loops above depict the "classical" way to iterate
  // over the contents of a 1D image.  The next tour file introduces
  // the solution provided by Olena to write better loops...


  // Jump to tour1_extra.cc if you want some more information about
  // image data management.

  // Jump to tour2.cc to continue the tour.
}
