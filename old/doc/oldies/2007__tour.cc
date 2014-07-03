/* This is a short tour of the basics of Olena.  It isn't in any way
   meant to cover all the features of Olena.  Especially, some
   important objects, such as the iterators, are NOT presented here.
   */

// We will run some example with 1D, and 2D, so we include these two
// files.
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
//#include <oln/basics3d.hh>

// Let's include all types, for simplicity.
#include <ntg/all.hh>

// Some algorithms...
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>
#include <oln/morpho/watershed.hh>

#include <oln/utils/stat.hh>

// Basic conversion functions.
#include <oln/convert/basics.hh>

// Always include system headers after Olena.
#include <iostream>
#include <cmath>



// Namespaces.
// ===========
// Olena is organized in a namespace hierarchy.  Every thing is
// declared by Olena under the 'oln::' namespace, and possibly un a
// sub namespace such as 'oln::arith::' (arithmetic operations on
// images), 'oln::morpho::' (morphological operations), etc.
// For the sake of simplicity, we will neglect the 'oln::' prefix
// in this file.
using namespace oln;

// Data types are defined in the Intègre library, so they are
// in the ntg namespace.
// In this file, we will neglect the 'ntg::' prefix.
using namespace ntg;

int
main (void)
{
  // Basic value types.
  // ==================
  // Olena ships with a set of basic value types such as
  //   int_u8, int_u16, int_u32,   (common unsigned integer types)
  //   int_s8, int_s16, int_s32,   (common signed integer types)
  //   float_s, float_d,             (common float types)
  //   bin			   (binary type: false|true)
  //
  // These value are defined in the value/ subdirectory.  Actually
  // we defined all of them by including value/all.hh.
  //
  // You should use them instead of the standard C/C++ type, because
  // Olena types are equipped with additional members required to
  // write generic processings.

  // For instance the max() and min() class methods
  // will return the maximum value of a type.
  std::cout << "ntg_max_val(int_u8) = " 
	    << ntg_max_val(int_u8) << std::endl;

  // You can combine these values with the standard operators
  // as expected.
  int_u8  a = 4;
  int_u16 b = 1000;
  std::cout << "a + b = " << a + b << std::endl;

  // Sometime it's hard to figure which type a variable has, because
  // Olena seems to behave surprisingly at a first glance.
  // There are two convenient functions that you can use to
  // inspect a type.
  // typename_of<T>()   returns a std::string describing T
  // typename_of_var(V) returns a std::string describing the type of V
  std::cout << typename_of<int_u8>()
	    << " + "
	    << typename_of_var(b)
	    << " = " << typename_of_var(a + b) << std::endl;
  // The expression above will print
  // "int_u<8> + int_u<16> = int_u<17>"
  // This probably seems surprising for two reasons:
  // 1) int_u8 is printed as int_u<8>
  //    this is because int_u8 is really just a short hand for int_u<8>,
  //    Olena allows you to declare unsigned integer on 13 bits, if
  //    you want (int_u<13>); but int_u<8>, int_u<16>, and int_u<32> are
  //    more common so they have their own typedef.
  // 2) (a + b) has type int_u<17>, not int_u<16> as one would expect.
  //    The point is that one needs 17 bits to hold the result
  //    of an addition between a 8bit and a 16bit value without
  //    overflowing.

  // Olena will also ensure that you don't such a large value to
  // a variable with a smaller type:
  //  int_u16 c = a + b;
  // would fail to compile because (a + b) has type int_u<17> which
  // cannot fit in int_u<16>.
  // If you *know* that the result of (a + b) fits in c, you
  // should cast the value to int_u16 explicitly:
  int_u16 c = a + b;
  std::cout << "c = " << c << std::endl;
  // Of course assigning a+b to a variable of type int_u<17> or
  // greater is allowed and doesn't require a cast.


  // Image types.
  // ============
  // There are three kind of images supported for the moment.
  // image1d, image2d, and image3d: the names are self-explanatory.
  // All are template, parametrized by the kind of value associated
  // to each point (we call 'point' a position in the image).
  //
  // We will start to use the 1D images, because they are
  // easier to fill manually (i.e. there are less value
  // to assign), and easy to display.

  // Here are two most basic way to build an image.
  image1d<int_u8> img1a;	// Build an empty image.
  image1d<int_u8> img1b(20);	// Build an image with 20 points.

  // The empty image hasn't any data.  It's really useful
  // only as a placeholder for the result of some processing,
  // or another image.

  // Assigning an image to another one is not a costly operation.
  // In fact, img1a behave like some pointers.  After the following
  // instruction
  img1a = img1b;
  // img1a and img1b can be used equivalently to access the
  // same data (in other words, the point values are shared
  // by img1a and img1b).
  img1a(10) = 42;		// Assign value 42 to the 11th point of img1a
  std::cout << "img1b(10) ="
	    <<  img1b(10) << std::endl; // print 42, although we use img1b

  // We call these assignments 'shallow copies', because they don't copy the
  // data.  If you want to make a deep copy of an image, then use
  // the clone() method, as in Java.
  img1a = img1b.clone ();
  img1a(10) = 13;
  std::cout << "img1b(10) ="
	    <<  img1b(10) << std::endl; // still print 42

  // As show above, assignments can be made using the parenthesis
  // operator, which takes to coordinates of the point you want to
  // assign a value to in argument.  (In 1D there is only one coordinate.)
  // Let's fill our image with a saw-teeth signal.
  // (coord is the type used for coordinates)
  for (coord col = 0; col < img1a.ncols(); ++col)
    img1a(col) = col % 5;

  std::cout << "img1a:" << std::endl;
  std::cout << img1a << std::endl;
  // 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4

  // You can build image2d and image3d objects similarly, except
  // you have to account for the extra coordinate.
  image2d<bin> img2a(10, 10);
  for (coord row = 0; row < img2a.nrows(); ++row)
    for (coord col = 0; col < img2a.ncols(); ++col)
      img2a(row, col) = (row >= 3 && col >= 3 && row < 7 && col < 7);
  std::cout << "img2a:" << std::endl;
  std::cout << img2a << std::endl;
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - | | | | - - -
  // - - - | | | | - - -
  // - - - | | | | - - -
  // - - - | | | | - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -

  // Point types
  // ===========
  // As said before, a point is a position in an image.  Because
  // Olena supports different dimension, it supports different
  // types of points: point1d, point2d, point3d.

  // You build a point by passing it as much coordinate as
  // needed.
  point1d p1(10);
  point2d p2(5, 3);
  // You can address the value associated to a particular point
  // in a image by using the [] operator:
  std::cout << "img1b[p1]" << img1b[p1] << std::endl; // print 42

  img2a[p2] = false;
  std::cout << "img2a:" << std::endl;
  std::cout << img2a << std::endl;
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - | | | | - - -
  // - - - | | | | - - -
  // - - - - | | | - - -
  // - - - | | | | - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -


  // Delta Point types
  // =================
  // You can't add points together, this make no sense.  However
  // you can use a delta-point to record a displacement, and add
  // this delta-point to a point (yielding a new point).
  // The types for delta-points are dpoint1d, dpoint2d, and dpoint3d.
  dpoint1d dp1(2);
  std::cout << "p1 + dp1 = " << p1 + dp1 << std::endl;
  dpoint2d dp2(-1, 2);
  img2a[p2 + dp2] = false;
  std::cout << "img2a:" << std::endl;
  std::cout << img2a << std::endl;
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - | | | | - - -
  // - - - | | - | - - -
  // - - - - | | | - - -
  // - - - | | | | - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -


  // Windows
  // =======
  // Windows are sets of delta-points.
  //
  // There are some predefined windows.
  // For instance here are two common window2d objects:
  std::cout << "win_c4p() = " << win_c4p() << std::endl;
  // [(-1,0)(0,-1)(0,0)(0,1)(1,0)]
  std::cout << "win_c4_only() = " << win_c4_only() << std::endl;
  // [(-1,0)(0,-1)(0,1)(1,0)]

  // A window is commonly used to iterate around a fixed point.
  // For instance
  window2d w2 = win_c4_only();
  for (unsigned i = 0; i < w2.card(); ++i)
    std::cout << "img2a[p2 + w2[" << i << "]] = "
	      << "img2a[" << p2 + w2.dp(i) << "] = "
	      << img2a[p2 + w2.dp(i)] << std::endl;
  // img2a[p2 + w2[0]] = img2a[(4,3)] = |
  // img2a[p2 + w2[1]] = img2a[(5,2)] = -
  // img2a[p2 + w2[2]] = img2a[(5,4)] = |
  // img2a[p2 + w2[3]] = img2a[(6,3)] = |

  // You can build you own window using by adding delta-points
  // using the add() method.
  window1d w1;
  w1.add(-1).add(0).add(1);
  std::cout << "w1 = " << w1 << std::endl; // [(-1)(0)(1)]

  // Neighborhoods
  // =============
  // These objects are like Windows, except they have
  // some additional properties (a point is not in its neighborhood,
  // the neighbor of a point should have the point in its neighborhood).
  // For this reason, they have types distinct from the windows:
  // neighborhood1d, neighborhood2d, neighborhood3d.
  // The interface is just the same as the windows.
  neighborhood1d n1;
  n1.add(1);
  std::cout << "n1 = " << n1 << std::endl; // N[(-1)(1)]


  // Processings.
  // ============
  // We have seen how to build images and windows, that's
  // enough to call some morphological operator.
  // Windows are used as structural elements, so we can
  // apply some basic morphological operator on img1a
  // (the saw) using w1 as structural element:
  std::cout << "erosion" << std::endl;
  std::cout << morpho::erosion(img1a, w1) << std::endl;
  // 0 0 1 2 0 0 0 1 2 0 0 0 1 2 0 0 0 1 2 3
  std::cout << "dilation" << std::endl;
  std::cout << morpho::dilation(img1a, w1) << std::endl;
  // 1 2 3 4 4 4 2 3 4 4 4 2 3 4 4 4 2 3 4 4
  std::cout << "opening" << std::endl;
  std::cout << morpho::opening(img1a, w1) << std::endl;
  // 0 1 2 2 2 0 1 2 2 2 0 1 2 2 2 0 1 2 3 3
  std::cout << "closing" << std::endl;
  std::cout << morpho::closing(img1a, w1) << std::endl;
  // 1 1 2 3 4 2 2 2 3 4 2 2 2 3 4 2 2 2 3 4

  // Yet, remember that Olena is a *generic* image
  // processing library, in which processing are meant
  // to be written once but yet work on many kind of images.

  // Let's close the small hole we have added to img2d (in the
  // paragraph about delta points).
  std::cout << morpho::closing(img2a, win_c4p()) << std::endl;
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - | | | | - - -
  // - - - | | | | - - -
  // - - - - | | | - - -
  // - - - | | | | - - -
  // - - - - - - - - - -
  // - - - - - - - - - -
  // - - - - - - - - - -


  // The watershed algorithm takes a neighborhood (not a window) as
  // argument, and a type as template parameter.

  // The type parameter specifies the type of the output image value.
  // Watershed are indicated with the max() of this type, other values
  // are basin labels.
  std::cout << "watershed" << std::endl;
  std::cout << morpho::watershed_seg<int_u8>(img1a, n1) << std::endl;
  // 4 4 4 4 255 3 3 3 3 255 2 2 2 2 255 1 1 1 1 1


  // Reading and writing PNM files.
  //   // You can read image2d from pnm files.  The kind
  // of pnm format supported (1,2,3,4,5,6) is dependent on
  // the type of the image you load/save.  The extension
  // is also used to guess the format to use.
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");
  if (!lena.has_impl())
    {
      std::cerr << "Could not load " << IMGDIR "/lena.pgm" << std::endl;
      exit(1);
    }
  std::cout << "watershedling lena..." << std::endl;
  save(morpho::watershed_seg<int_u8>(lena, neighb_c4()), "output.pgm");


  // High order operators.
  //   // Olena offers a set of high order operators than can simplify
  // the writing of simple algorithm.
  // For instance `traverse(f, img)' will apply the function `f'
  // to each point of `img'.  Actually, `f' do not need to be a function,
  // it just have to _behave_ like a function; it can be a functor, i.e.,
  // an instance of a class that implements an `operator()' (this allows
  // `f' to maintain a state without using a global variable).

  // The header `utils/stat.hh' define some functor to compute
  // statistical data.  In the following example we instantiate
  // an `f_moments' functor, have it traverse the saw-teeth image,
  // and compute some moments from the data gathered during the traversal.
  utils::f_moments<int_u8> f;
  traverse(f, img1a);
  std::cout << "f.mean() = "     << f.mean()     << std::endl;
  std::cout << "f.variance() = " << f.variance() << std::endl;

  // `apply' is an high order operator similar to `traverse', but
  // it builds an image from the results of the function application.
  //
  //   o = apply(f, i);
  //
  // is a short for
  //
  //  for all point p in i: o[p] = f(i[p])
  //
  // The type of the output image may be different from the type
  // of the input image, so `apply' need a little more help from
  // `f' than `traverse' does.  Basically, `f' should define
  // a type member named `result_type' so that `apply' knows which
  // type of output image it has to create.
  //
  // (NOTE: The `result_type' presence is part of the requirement of
  // the `Adaptable Unary Function' concept in the STL.  So if you
  // have a model of `Adaptable Unary Function', you can safely pass
  // it to `apply'.)
  //
  // Some conversion functions (those for which the output type is
  // knonw) can be used this way.  For instance the
  // `convert::bound<T>' functor will convert any value to type `T'
  // and saturate for out-of-bound values.  See how the `4' are
  // converted to `3' when the saw-teeth image is converted to an
  // image2d<int_u<2> > image.
  std::cout << apply(convert::bound<int_u<2> >(), img1a) << std::endl;
  // 0 1 2 3 3 0 1 2 3 3 0 1 2 3 3 0 1 2 3 3

  // Another usefull high order operator is `fold':
  // - `fold(f, val, img)' computes `f(...f(f(val,i_0),i_1)...,i_n)',
  //   where i_0...i_n are the value associated to each image point.
  // - `fold(f, img)' computes `f(...f(f(i_0,i_1),i_2)...,i_n)'.
  // In both cases, `f' must be a model of the
  // `Adaptable Binary Function' STL concept.
}
