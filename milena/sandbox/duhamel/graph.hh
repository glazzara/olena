// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_GRAPH_HH
# define MLN_GRAPH_HH

# include <cstddef>
# include <iostream>
# include <vector>
# include <map>
# include <mln/core/concept/object.hh>

namespace mln
{
  namespace util
  {

    template<typename T>
    struct				s_node
    {
      T					data;
      std::vector<unsigned>		links;
    };

    template<>
    struct				s_node<void>
    {
      std::vector<unsigned>		links;
    };

    template<typename T>
    struct				s_edge
    {
      T					data;
      unsigned				node1;
      unsigned				node2;
    };

    template<>
    struct				s_edge <void>
    {
      unsigned				node1;
      unsigned				node2;
    };

    template<typename N, typename E = void>
    class graph
    {
    public:
      graph ()  :
	nb_node_ (0), nb_link_ (0) {}
      graph (unsigned nb_node, unsigned nb_link) :
	nb_node_ (nb_node), nb_link_ (nb_link) {}
      ~graph () {}

      //      void add_node (N& elt);
      void add_node (void);
      void add_edge (unsigned n1, unsigned n2);
      void coherence () const;
      void print_debug () const;

    public:
      unsigned nb_node_;
      unsigned nb_link_;
      std::vector<struct s_node<N>*> nodes_;
      std::vector<struct s_edge<E>*> links_;
    };

  } // end of util  
} // end of mln


#include "graph.hxx"

#endif // MLN_GRAPH_HH
