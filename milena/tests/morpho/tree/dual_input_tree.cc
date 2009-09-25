// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/value/int_u8.hh>

#include <mln/make/image.hh>
#include <mln/morpho/tree/dual_input_tree.hh>

template <typename V, unsigned dim, unsigned dim2>
void test(V (&f_)[dim], V (&m_)[dim], V (&points_)[dim2], V (&p_)[dim2])
{
  using namespace mln;
  typedef image1d<V> I;
  typedef p_array< mln_psite(I) > S;
  typedef morpho::tree::data<I,S> tree_t;


  I f = make::image(f_);
  I m = make::image(m_);


  tree_t tree = morpho::tree::dual_input_max_tree(f, m, c2());

  mln_dn_site_piter(tree_t) p(tree);
  p.start();
  for (unsigned i = 0; i < dim2; ++i, p.next())
  {
    mln_assertion(p.is_valid());
    mln_assertion(p == point1d(points_[i]));
    mln_assertion(tree.parent(p) == point1d(p_[i]));
  }
  mln_assertion(! p.is_valid());
}



int main()
{

  using namespace mln;

  typedef value::int_u8 V;

  //V f_[] = {10, 10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 30, 30, 20, 20, 10, 20, 20, 10};
  //V m_[] = {10, 10, 20, 20, 30, 30, 40, 40, 30, 30, 30, 30, 30, 30, 20, 20, 15, 15, 15, 15};


  //V f_[] = {15, 15, 10, 10, 05, 05, 8, 8, 5, 5, 10, 10, 15, 15};
  //V m_[] = {20, 20, 18, 18, 25, 25, 8, 8, 5, 5, 10, 10, 15, 15};

  {
    V f[] = {15, 15, 15};
    V m[] = {20, 20, 20};
    V s[] = {0, 1, 2};
    V p[] = {0, 0, 0};
    test(f, m, s, p);
  }

  {
    V f[] = {15, 17, 17, 19, 16};
    V m[] = {15, 17, 17, 19, 16};
    V s[] = {0, 4, 1, 2, 3};
    V p[] = {0, 0, 4, 1, 1};


    test(f, m, s, p);
  }

  {
    V f[] = {15, 17, 17, 15, 18, 19};
    V m[] = {15, 17, 17, 15, 18, 19};
    V s[] = {0, 3, 1, 2, 4, 5};
    V p[] = {0, 0, 0, 1, 0, 4};
    test(f, m, s, p);
  }

  // Clustering: simple connection
  {
    V f[] = {15, 17, 17, 15, 18, 19};
    V m[] = {15, 18, 18, 18, 18, 19};
    V s[] = {0, 3, 1, 2, 4, 5};
    V p[] = {0, 0, 0, 1, 1, 4};
    test(f, m, s, p);
  }

  // Clustering: merge two nodes
  {
    V f[] = {15, 18, 18, 15, 18, 19};
    V m[] = {15, 18, 18, 18, 18, 19};
    V s[] = {0, 3, 1, 2, 4, 5};
    V p[] = {0, 0, 0, 1, 1, 1};
    test(f, m, s, p);
  }

  // Clustering: create new node
  {
    V f[] = {18, 15, 18};
    V m[] = {18, 17, 18};
    V s[] = {1, 4, 0, 2};
    V p[] = {1, 1, 4, 4};
    test(f, m, s, p);
  }

  // Clustering: create new node
  {
    V f[] = {10, 10, 12, 12};
    V m[] = {11, 11, 13, 13};
    V s[] = {0, 1, 4, 2, 3};
    V p[] = {0, 0, 0, 4, 2};
    test(f, m, s, p);
  }

  // Segmentation: simple (no node creation)
  {
    V f[] = {8, 10, 10, 12, 12};
    V m[] = {8, 8, 8, 12, 12};
    V s[] = {0, 1, 2, 3, 4};
    V p[] = {0, 0, 0, 0, 3};
    test(f, m, s, p);
  }

  // Segmentation: simple (no node creation)
  {
    V f[] = {8, 10, 10, 8};
    V m[] = {8, 8, 8, 8};
    V s[] = {0, 3, 1, 2};
    V p[] = {0, 0, 0, 0};
    test(f, m, s, p);
  }

   // Segmentation: create node
  {
    V f[] = {8, 10, 10, 8};
    V m[] = {8, 9, 9, 8};
    V s[] = {0, 3, 5, 1, 2};
    V p[] = {0, 0, 0, 5, 5};
    test(f, m, s, p);
  }

   // Segmentation: create a root node
  {
    V f[] = {8, 10, 10, 8};
    V m[] = {8, 5, 9, 8};
    V s[] = {5, 0, 3, 1, 6, 2}; // Or {5, 0, 3, 6, 1, 2} for a strict sort
    V p[] = {5, 5, 5, 5, 3, 6};
    test(f, m, s, p);
  }

}
