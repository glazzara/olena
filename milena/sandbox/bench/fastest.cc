// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/util/timer.hh>

#include <mln/core/box_runend_piter.hh>

# define loop(n) for (unsigned i = 0; i < n; ++i)



namespace mln
{


  // ptr

  template <typename I>
  void bench_0(I& ima)
  {
    const unsigned n = ima.nelements();
    mln_value(I)* ptr = ima.buffer();
    for (unsigned i = 0; i < n; ++i)
      *ptr++ = 0;
  }

  template <typename I>
  void bench_0(I& ima1, const I& ima2)
  {
    const unsigned n = ima1.nelements();
    mln_value(I)* ptr1 = ima1.buffer();
    const mln_value(I)* ptr2 = ima2.buffer();
    for (unsigned i = 0; i < n; ++i)
      *ptr1++ = *ptr2++;
  }


  // piter

  template <typename I>
  void bench_1(I& ima)
  {
    mln_piter(I) p(ima.domain());
    for_all(p)
      ima(p) = 0;
  }


  // pixter

  template <typename I>
  void bench_2(I& ima)
  {
    mln_fwd_pixter(I) pxl(ima);
    for_all(pxl)
      pxl.val() = 0;
  }

  template <typename I>
  void bench_2(I& ima1, const I& ima2)
  {
    mln_fwd_pixter(I) pxl1(ima1);
    mln_fwd_pixter(const I) pxl2(ima2);
    for_all_2(pxl1, pxl2)
      pxl1.val() = pxl2.val();
  }


  // pixter -> offset

  template <typename I>
  void bench_3(I& ima)
  {
    mln_fwd_pixter(I) pxl(ima);
    for_all(pxl)
    {
      unsigned p = pxl.offset();
      ima.element(p) = 0;
    }
  }

  template <typename I>
  void bench_3(I& ima1, const I& ima2)
  {
    // border::equalize(ima1, ima2);
    mln_fwd_pixter(I) pxl(ima1);
    for_all(pxl)
    {
      unsigned p = pxl.offset();
      ima1.element(p) = ima2.element(p);
    }
  }


  // pixter -> point

  template <typename I>
  void bench_4(I& ima)
  {
    mln_fwd_pixter(I) pxl(ima);
    for_all(pxl)
    {
      unsigned o = pxl.offset();
      mln_psite(I) p = ima.point_at_index(o);
      ima(p) = 0;
    }
  }



  // box_runstart_piter -> ptr

  template <typename I>
  void bench_5(I& ima)
  {
    mln_box_runstart_piter(I) s(ima.domain());
    const unsigned n = s.run_length();
    for_all(s)
    {
      mln_value(I)* ptr = & ima(s);
      for (unsigned i = 0; i < n; ++i)
	{
	  *ptr++ = 0;
	}
    }
  }


  // box_runstart_piter -> point

  template <typename I>
  void bench_6(I& ima)
  {
    typedef mln_site(I) P;
    const unsigned dim_1 = P::dim - 1;
    mln_box_runstart_piter(I) s(ima.domain());
    const unsigned n = s.run_length();
    for_all(s)
    {
      mln_value(I)* ptr = & ima(s);
      P p = s;
      for (unsigned i = 0; i < n; ++i)
	{
	  *ptr++ = 0;
	  ++p[dim_1];
	}
    }
  }

  template <typename I>
  void bench_6(I& ima1, const I& ima2)
  {
    typedef mln_site(I) P;
    const unsigned dim_1 = P::dim - 1;
    mln_box_runstart_piter(I) s(ima1.domain());
    const unsigned n = s.run_length();
    for_all(s)
    {
      mln_value(I)* ptr1 = & ima1(s);
      const mln_value(I)* ptr2 = & ima2(s);
      P p = s;
      for (unsigned i = 0; i < n; ++i)
	{
	  *ptr1++ = *ptr2++;
	  ++p[dim_1];
	}
    }
  }


  // box_runend_piter -> point

  template <typename I>
  void bench_7(I& ima)
  {
    typedef mln_site(I) P;
    const unsigned dim_1 = P::dim - 1;
    mln_box_runend_piter(I) s(ima.domain());
    const unsigned n = s.run_length();
    for_all(s)
    {
      mln_value(I)* ptr = & ima(s);
      P p = s;
      for (unsigned i = 0; i < n; ++i)
      {
	*ptr-- = 0;
	--p[dim_1];
      }
    }
  }




//   template <typename I>
//   void bench_6_alt(I& ima1, const I& ima2)
//   {
//     typedef mln_site(I) P;
//     const unsigned dim_1 = P::dim - 1;
//     mln_fwd_by_run_piter(I) p(ima1.domain());
//     for_all(p)
//     {
//       if (p.at_runstart())
// 	{
// 	  p.update(ptr1, ima1);
// 	  p.update(ptr2, ima2);
// 	}
//       *ptr1++ = *ptr2++;
//       p; // converts to point2d
//     }
//   }


} // end of namespace mln





int main()
{
  using namespace mln;

  const unsigned n = 100, size = 1000;
  image2d<int> ima(size, size), ima2(size, size);
  util::timer t;


//   // ptr

//   {
//     t.start();
//     loop(n) bench_0(ima);
//     float ts = t.stop();
//     std::cout << "ptr " << ts << std::endl;
//   }

//   {
//     t.start();
//     loop(n) bench_0(ima, ima2);
//     float ts = t.stop();
//     std::cout << "2 ptrs " << ts << std::endl;
//   }


//   // piter

//   {
//     t.start();
//     loop(n) bench_1(ima);
//     std::cout << "piter " << t.stop() << std::endl;
//   }


//   // pixter

//   {
//     t.start();
//     loop(n) bench_2(ima);
//     float ts = t.stop();
//     std::cout << "pixter " << ts << std::endl;
//   }

//   {
//     t.start();
//     loop(n) bench_2(ima, ima2);
//     float ts = t.stop();
//     std::cout << "2 pixters " << ts << std::endl;
//   }


//   // pixter -> offset

//   {
//     t.start();
//     loop(n) bench_3(ima);
//     std::cout << t.stop() << std::endl;
//   }

//   {
//     t.start();
//     loop(n) bench_3(ima, ima2);
//     std::cout << t.stop() << std::endl;
//   }


//   // pixter -> point

//   {
//     t.start();
//     loop(n) bench_4(ima);
//     std::cout << t.stop() << std::endl;
//   }


//   // box_runstart_piter -> ptr

//   {
//     t.start();
//     loop(n) bench_5(ima);
//     std::cout << t.stop() << std::endl;
//   }


  // box_runstart_piter -> (ptr, point)

//   {
//     t.start();
//     loop(n) bench_6(ima);
//     float ts = t.stop();
//     std::cout << "runstart -> (ptr, point) " << ts << std::endl;
//   }

//   {
//     t.start();
//     loop(n) bench_6(ima, ima2);
//     float ts = t.stop();
//     std::cout << "runstart -> (2 ptrs, point) " << ts << std::endl;
//   }

  {
    t.start();
    loop(n) bench_7(ima);
    float ts = t.stop();
    std::cout << "runend -> (2 ptrs, point) " << ts << std::endl;
  }

}
