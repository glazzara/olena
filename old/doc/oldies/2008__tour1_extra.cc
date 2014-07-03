// Copyright (C) 2007 EPITA Research and Development Laboratory
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

// File: tour1_extra.cc.

#include <oln/core/1d/image1d.hh>
#include <oln/arith/plus.hh>
#include <oln/debug/println.hh>



int main()
{
  using namespace oln;


  // Let us take a few examples to illustrate how images and data are
  // managed.  In these examples, we will use the notion of "block of
  // instructions" from the C/C++ languages: "{" ... "}".




  // Example 1:

  { // A new block starts here.

    image1d<float> local_ima(1000);
    // A non-empty image is created; a memory buffer is automatically
    // allocated to store image data.  Some code using 'local_ima' can
    // be inserted here...

  } // The block ends and 'local_ima' is not more accessible.

  // At the end of the block, since the user cannot use 'local_ima'
  // anymore, the memory is automatically deallocated.




  // Example 2:

  image1d<float> ima; // An empty image---a variable.

  { // A new block.

    image1d<float> local_ima(1000); // A non-empty image.
    // Some work is performed with 'local_ima' here.
    // ...
    ima = local_ima; // Last, we state that ima IS local_ima.

  } // End of the block: 'local_ima' is no more accessible.

  assert(not ima.is_empty()); // We test that 'ima' is no more empty.

  // Since we state that "ima IS local_ima" and since 'ima' is still
  // accessible by the user, the contents of 'local_ima' is preserved.

  // Conversely to the example 1, the memory allocated for 'local_ima'
  // is thus NOT deallocated: the data of 'local_ima' is "transfered"
  // to 'ima'.  There is NO data "copy" involved during this transfer
  // so it is fast and it saves memory.




  // Example 3:

  {
    image1d<int> orig(5); // An original image.
    for (int i = 0; i < 5; ++i)
      orig.at(i) = i;
    debug::println(orig);
    // 0 1 2 3 4

    image1d<int> ima = orig; // 'ima' is 'orig'.
    ima = ima + ima;         // Point-wise sum of ima and itself.
    debug::println(ima);
    // 0 2 4 6 8

    debug::println(orig);
    // 0 1 2 3 4
  }

  // To explain this result, let us detail the point-wise sum line.
  // The assignment "ima = ima + ima" is a two-step process: first
  // "ima + ima" is computed, then the assignment is performed.  This
  // line can be rewritten as:

  //    image1d<int> anon = ima + ima;  // line (a)
  //    ima = anon;                     // line (b)

  // where 'anon' is the "anonymous" result of the sum.  We can see
  // that 'ima' is used in line (a) to compute a new image, namely
  // 'anon'.  At that precise line, 'ima' still designates 'orig'.  At
  // line (b) the definition of 'ima' changes: 'ima' was designating
  // 'orig', now it designates 'anon', that is, the sum result.
  // Eventually the data of 'orig' have never changed since it has
  // been initialized.

  // In this example, two images with effective data have been
  // created: the sum result, which is accessible through 'ima', and
  // 'orig'.

  // The same explanation in pictures.  When the sum "ima + ima" is
  // computed, the images and their data look like:
  //
  //            +-----------+
  //   orig --> | 0 1 2 3 4 |
  //            +-----------+
  //             ^
  //   ima ______|
  //
  //            +-----------+
  //   anon --> | 0 2 4 6 8 |
  //            +-----------+
  //
  // Then the assignment modifies this scheme into:
  //
  //            +-----------+
  //   orig --> | 0 1 2 3 4 |
  //            +-----------+
  //   ima ______
  //             |
  //             v
  //            +-----------+
  //   anon --> | 0 2 4 6 8 |
  //            +-----------+
  //
  // and the temporary 'anon' disappears.





  // Example 4:

  {
    image1d<int> ima(5);
    for (int i = 0; i < 5; ++i)
      ima.at(i) = i;
    debug::println(ima);
    // 0 1 2 3 4

    ima = ima + ima;
    debug::println(ima);
    // 0 2 4 6 8
  }

  // Let us re-write the assignment line:

  //    image1d<int> anon = ima + ima;  // line (a)
  //    ima = anon;                     // line (b)

  // A new image, 'anon', is created to store the sum result; this
  // image is just like a temporary object since it is anonymous.  In
  // line (a) the definition of 'ima' changes: it was the original
  // image with its data being "0 1 2 3 4" and it now designates the
  // sum result.  The original data becomes inaccessible by the user
  // so it is automatically deallocated.

  // The same explanation In pictures.  After the sum is computed, we
  // have:
  //            +-----------+
  //   ima ---> | 0 1 2 3 4 |
  //            +-----------+
  //
  //            +-----------+
  //   anon --> | 0 2 4 6 8 |
  //            +-----------+
  //
  // and the assignment of 'ima' leads to:
  //
  //            +-----------+
  //   ima      | 0 1 2 3 4 |
  //    |       +-----------+
  //    |_________
  //              |
  //              v
  //            +-----------+
  //   anon --> | 0 2 4 6 8 |
  //            +-----------+
  //
  // so the original data, unreachable, are deallocated and the
  // temporary 'anon' disappears; we end up with:
  //
  //            +-----------+
  //   ima ---> | 0 2 4 6 8 |
  //            +-----------+
}
