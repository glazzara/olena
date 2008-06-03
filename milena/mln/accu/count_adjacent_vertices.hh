// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_COUNT_ADJACENT_VERTICES_HH
# define MLN_ACCU_COUNT_ADJACENT_VERTICES_HH

/// \file mln/accu/count_adjacent_vertices.hh
/// \brief Define an accumulator that counts the vertices adjacent to a
/// set of line graph psite.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/line_graph_image.hh>
# include <mln/util/pix.hh>

namespace mln
{

  namespace accu
  {

    /// \brief Accumulator class counting the number of vertices
    /// adjacent to a set of mln::line_graph_psite (i.e., a set of
    /// edges).
    ///
    /// The type to be count is mln::util::pix< mln::line_graph_image<P, V> >
    /// where \p P and \p V are the parameters of this class.
    ///
    /// This accumulator is used by mln::closing_area_on_vertices and
    /// mln::opening_area_on_vertices.
    template <typename P, typename V>
    struct count_adjacent_vertices_ 
      : public mln::accu::internal::base_< std::size_t,
					   count_adjacent_vertices_<P, V> >
    {
      typedef mln::util::pix< mln::line_graph_image<P, V> > argument;

      count_adjacent_vertices_();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& arg);
      void take(const count_adjacent_vertices_<P, V>& other);

      /// Force the value of the counter to \a c. 
      void set_value(std::size_t c);
      /// \}

      /// Get the value of the accumulator.
      std::size_t to_result() const;

    protected:
      /// Update the value of the counter.
      void update_ ();

    protected:
      /// The value of the counter.
      std::size_t count__;
      /// The set of adjacent vertices.
      std::set<util::node_id> vertices_;
    };


    /// \brief Meta accumulator for count_adjacent_vertices.
    struct count_adjacent_vertices
      : public Meta_Accumulator< count_adjacent_vertices >
    {
      template <typename P, typename V>
      struct with
      {
	typedef count_adjacent_vertices_<P, V> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename V>
    inline
    count_adjacent_vertices_<P, V>::count_adjacent_vertices_()
    {
      init();
    }

    template <typename P, typename V>
    inline
    void
    count_adjacent_vertices_<P, V>::init()
    {
      vertices_.clear();
      update_();
    }

    template <typename P, typename V>
    inline
    void
    count_adjacent_vertices_<P, V>::take(const argument& arg)
    {
      vertices_.insert(arg.p().first_id());
      vertices_.insert(arg.p().second_id());
      update_();
    }

    template <typename P, typename V>
    inline
    void
    count_adjacent_vertices_<P, V>::take(const count_adjacent_vertices_<P, V>& other)
    {
      vertices_.insert (other.vertices_.begin(), other.vertices_.end());
      update_();
    }

    template <typename P, typename V>
    inline
    std::size_t
    count_adjacent_vertices_<P, V>::to_result() const
    {
      return count__;
    }

    template <typename P, typename V>
    inline
    void
    count_adjacent_vertices_<P, V>::set_value(std::size_t c)
    {
      count__ = c;
      /// Reset the other member.
      vertices_.clear();
    }

    template <typename P, typename V>
    inline
    void
    count_adjacent_vertices_<P, V>::update_()
    {
      count__ = vertices_.size();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNT_ADJACENT_VERTICES_HH
