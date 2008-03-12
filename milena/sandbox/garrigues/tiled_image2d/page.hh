// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef page_hh
#define page_hh

#include "block.hh"

namespace mln
{

  template <unsigned Dim, typename Layout>
  struct page_domain {};

  template<typename Layout>
  struct page_domain<1, Layout>
  {
    enum {dim = 1};
    static unsigned len(unsigned) { return Layout::width; }
  };
  template<typename Layout>
  struct page_domain<2, Layout>
  {
    enum {dim = 2};
    static unsigned len(unsigned i) { return i ? Layout::height : Layout::width; }
  };
  template<typename Layout>
  struct page_domain<3, Layout>
  {
    enum {dim = 3};
    static unsigned len(unsigned i) {
      return (i == 2) ? Layout::depth : (i ? Layout::height : Layout::depth);
    }
  };

  template<typename T, typename Layout>
  struct page : public Object< page<T, Layout> >
  {
    typedef Layout layout_type;
    typedef block<T, Layout::w * Layout::h> block_type;
    typedef T value_type;
    typedef page_domain<Layout::dim, Layout> domain_type;

    static domain_type domain()
    { return domain_type(); }

    page() {}
    page(const page& p) : data_(p.data_) {}
    page(block_type& p) : data_(&p) {}

    template<typename Point>
    value_type& operator()(const Point& p) {
      return (*data_)[layout_type::page_layout::pixel_at(p)];
    }

    template<typename Point>
    const value_type& operator()(const Point& p) const {
      return (*data_)[Layout::page_layout::pixel_at(p)];
    }

    block_type *data_;
  };


} // end of namespace mln

#endif
