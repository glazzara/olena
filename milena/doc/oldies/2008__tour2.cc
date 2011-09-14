// Copyright (C) 2001, 2007 EPITA Research and Development Laboratory
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
#include <oln/core/2d/window2d.hh>
#include <oln/debug/println.hh>



// Note to the reader: If you do not have read the tour1.cc file, you
// should have a quick look at it before proceeding with this present
// file.  Some important features and practices are described in the
// former tour step that are here assumed to be known.



int main()
{
  using namespace oln;

  // As shown before, image data read access and pixel value
  // assignments can be performed using:
  // - either the parenthesis operator with takes a point as its
  //   argument,
  // - or the ".at" method which requires the coordinates of the point
  //   you want to access to.

  //   ex: ima(p), with p a 1D point, or ima.at(i), ima being a 1D
  //   image and i an index---single coordinate.


  // Objects from image2d and image3d types can be build and access to
  // in a similar way than image1d, except you have to account for
  // respectively one and two extra coordinates.  Let us take an
  // example.

  image2d<bool> img(4, 5);  // A 4x5 2D binary image.

  for (unsigned row = 0; row < img.nrows(); ++row)
    for (unsigned col = 0; col < img.ncols(); ++col)
      img.at(row, col) =
	(row > 0 and row < 3) and (col > 0 and col < 4);

  debug::println(img); // Gives:
  // - - - - - 
  // - | | | - 
  // - | | | - 
  // - - - - - 

  // When debug::print'ing binary images, the 'true' (object) and
  // 'false' (background) values are respectively depicted by the '|'
  // and '-' symbols.




  // As said before, a point is a position in an image.  Because
  // Olena supports different dimensions, it supports different
  // point types: point1d, point2d, and point3d.

  // We build a point by passing it as much coordinates as
  // needed:
  point2d p(2, 0); // a 2D point so a couple of coordinates.

  // Each point coordinate can be accessed separately...
  std::cout << "row = " << p.row() << std::endl
	    << "col = " << p.col() << std::endl;

  // ...and modified:
  p.col() = 1;
  std::cout << "col = " << p.col() << " (new value!)" << std::endl;

  // So we have a way to access image data:
  img(p) = false;
  // which is shorter than the equivalent writing:
  // ima2a.at(2, 1) = false;

  debug::println(img);
  // Gives (with hand-written coordinates):
  //  col 0 1 2 3 4
  // row    v
  //  0   - - - - -
  //  1   - | | | -
  //  2 > - - | | -
  //  3   - - - - -

  // Points in Olena are not really like mathematical vectors.  The
  // reason is twofold.  Some operations over vectors are meaningless
  // with points; for instance adding points together makes no sense.
  // Furthermore we want C++ expressions with points to be more
  // strongly typed so that errors from the user can be more easily
  // pointed out.

  // To record a displacement from one point to another one, Olena
  // introduces the notion of "delta-points".  The types for
  // delta-points are dpoint1d, dpoint2d, and dpoint3d.  We have:

  dpoint2d dp(-1, +2);  // -1 applies to row, +2 applies to column.
  img(p + dp) = false;
  debug::println(img);
  // Gives:
  //  col 0 1 2 3 4
  // row    . . v
  //  0   - - - - -
  //  1 > - | | - -
  //  2 . - - | | -
  //  3   - - - - -

  // Let us verify:
  point2d p2 = p + dp;
  std::cout << "p2 " << p2
	    << " = p " << p
	    << " + dp " << dp << std::endl;
  assert(p2 == point2d(1, 3)); // Right: p2 is (1, 3).

  // Taking the difference between a couple of points gives a
  // delta-point.
  dpoint2d dp2 = p2 - p;  // That should be equal to 'dp'.
  assert(dp2 == dp);      // Indeed.

  // Delta-points (amongst them) feature the classical arithmetical
  // operators:
  dpoint2d dp3 = dp + dp2;
  dp3 -= dp2;
  assert(dp3 == dp);


  // The notion of delta-point is at the base of the definitions of
  // classical window and neighborhood.


  // A window, or "sliding window", is a region defined around a
  // point.  In its most usual forms offered by Olena, for instance
  // window2d for the 2D case, windows are internally defined by a set
  // of delta-points.

  window2d win; // Empty window.
  win
    .take(dpoint2d(-1, -1)) // Add a delta-point.
    .take(dpoint2d( 0, 0))
    .take(dpoint2d( 1, 1))
    .take(dpoint2d( 1, 2));
  std::cout << "win = " << win << std::endl;
  // win = [ (-1, -1), (0, 0), (1, 1), (1, 2) ]

  // A window is commonly used to iterate around a fixed point.
  std::cout << "around " << p
	    << " window points are ";
  for (unsigned i = 0; i < win.size(); ++i)
    std::cout << (p + win[i])  // win[i] gives the ith element of the
	      << ' ';          // window, i.e., a delta-point.
  std::cout << std::endl;
  // The code above outputs:
  // around (2, 1) window points are (1, 0) (2, 1) (3, 2) (3, 3)


  // If we try to generalize this kind of display to all image points,
  // one straightforward code is the following:

  {
    // First version.

    std::cout << std::endl
	      << "First version:" << std::endl
	      << std::endl;

    for (unsigned row = 0; row < img.nrows(); ++row)
      for (unsigned col = 0; col < img.ncols(); ++col)
	{
	  point2d p(row, col);
	  std::cout << p << ": ";
	  for (unsigned i = 0; i < win.size(); ++i)
	    {
	      point2d q = p + win[i];  // q is a point around p;
	                               // precisely, the ith point of
	                               // the window win centered at
	                               // point p
	      if (img.has(q)) // we only print q if it actually lies
	                      // within the image
		std::cout << q << ' ';
	    }
	  std::cout << std::endl;
	}

  } // End of 1st version.

  // We obtain:
  // (0, 0): (0, 0) (1, 1) (1, 2) 
  // (0, 1): (0, 1) (1, 2) (1, 3) 
  // (0, 2): (0, 2) (1, 3) (1, 4) 
  // (0, 3): (0, 3) (1, 4) 
  // (0, 4): (0, 4) 
  // (1, 0): (1, 0) (2, 1) (2, 2) 
  // (1, 1): (0, 0) (1, 1) (2, 2) (2, 3) 
  // (1, 2): (0, 1) (1, 2) (2, 3) (2, 4) 
  // (1, 3): (0, 2) (1, 3) (2, 4) 
  // (1, 4): (0, 3) (1, 4) 
  // (2, 0): (2, 0) (3, 1) (3, 2) 
  // (2, 1): (1, 0) (2, 1) (3, 2) (3, 3) 
  // (2, 2): (1, 1) (2, 2) (3, 3) (3, 4) 
  // (2, 3): (1, 2) (2, 3) (3, 4) 
  // (2, 4): (1, 3) (2, 4) 
  // (3, 0): (3, 0) 
  // (3, 1): (2, 0) (3, 1) 
  // (3, 2): (2, 1) (3, 2) 
  // (3, 3): (2, 2) (3, 3) 
  // (3, 4): (2, 3) (3, 4) 


  // An equivalent code, given here just for you to realize that you
  // may continue to handle images and points with Olena just the way
  // you are used to it:

  /*

  { // A variation.

    int nrows = img.nrows(), ncols = img.ncols();
    unsigned n = win.size();
    for (int row = 0; row < nrows; ++row)
      for (int col = 0; col < ncols; ++col)
	{
	  std::cout << "(" << row << ", " << col << "): "; // print p
	  for (unsigned i = 0; i < n; ++i)
	    {
	      int // define q coordinates:
		r = row + win[i].row(),
		c = col + win[i].col();
	      if (r >= 0 and r < nrows and c >= 0 and c < ncols) // q is in img 
		std::cout << "(" << r << ", " << c << ") "; // print q
	    }
	  std::cout << std::endl;
	}

  } // End of a variation.

  */


  // Such samples of "classical" image processing code have 3 (three!)
  // main drawbacks:

  // - it is error-prone; note that there is rather a lot of code for
  //   a so simple algorithm;

  // - the algorithm, that is, the most important part of this code, is
  //   totally drowned in the middle of implementation details;

  // - this kind of writing only applies to a very special type of
  //   images (2D ones, rectangular, and starting at (0,0)) so it is
  //   not reusable.


  // If we express the algorithm into natural language, we can say:
  //
  //   p, a point of img
  //   q, a point of win centered at p
  //
  //   for all p
  //     print p
  //     for all q
  //       if q is in img 
  //         print q
  //       print end of line
  //     end for
  //   end for


  // The Olena library has been designed so that you can easily
  // translate your algorithms into code.  With the running example we
  // can write:

  { // Second version.

    std::cout << std::endl
	      << "Second version (same result):" << std::endl
	      << std::endl;

    box2d::piter    p (img.points());
    window2d::qiter q (win, p);

    for_all(p)
      {
	std::cout << p << ": ";
	for_all(q)
	  if (img.has(q))
	    std::cout << q << ' ';
	std::cout << std::endl;
      }

  } // End of 2nd version.

  std::cout << std::endl;


  // Above, p and q behave just like points; for instance, the
  // following expressions are valid:

  //    int r = p.row();
  //    to get the current row value,

  //    bool b = img(p);
  //    to get the pixel value at the current point,

  // or point2d pp = p + dp;
  //    where dp is a delta-point to get a point nearby p.

  // Yet, p and q are "more than points" since they allow to
  // browse/iterate over a set of points, respectivelly, the domain of
  // 'img' and the window centered at p.


  // The domain of 'img' is obtained with "img.points()" and is
  // provided to the 'p' object so that it knows how to iterate.

  // For a "basic" image, its set of points is an n-dimensional box.
  // In the 2D space, the box type is called 'box2d'.  We also have
  // 'box1d' and 'box3d' for other dimensions.

  box2d pts = img.points();
  std::cout << "img points are " << pts << std::endl;
  // Prints:
  // img points are { (0, 0) .. (3, 4) }

  // The type of iterators over a point set is obtained with the
  // expression: "name_of_the_point_set_type::piter", where 'piter'
  // means "point iterator" for short.

  // The same construction is available for iterators on window
  // points, whose types are obtained in a similar way with
  // "name_of_the_window_type::qiter".  Here the 'q' in 'qiter'
  // emphases the fact that a window is not really a set of points but
  // "a set of dpoints and a center point".



  // The second version of our example contrasts with the more
  // "classical" ones; it is:

  // - shorter,
  //   so it is less error-prone for the developer;

  // - easy to read,
  //   so the algorithm appears clearly from the code;

  // - (almost) "generic",
  //   for instance, no details within the loops indicate that we are
  //   processing a 2D image, that this image is rectangular, etc.


  //  +-----------------------------------------------------------+
  //  |                                                           |
  //  |  A major feature of Olena is to offer to its users a way  |
  //  |  to write GENERIC algorithms, that is, algorithms that    |
  //  |  accept different image types as input.                   |
  //  |                                                           |
  //  +-----------------------------------------------------------+


  // The next files of the tour give many details about what you can
  // expect from the notion of "genericity" applied to image
  // processing.

  // Now you can jump to tour3.cc


}
