// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_POINT_SET_HH
# define MLN_TRAIT_POINT_SET_HH

/*! \file mln/trait/point_set.hh
 *
 * \brief Properties of point_set classes.
 */

# include <string>
# include <mln/trait/undef.hh>

# define mln_trait_point_set_arity(P) typename mln::trait::point_set< P >::arity
# define mln_trait_point_set_has_speed(P) typename mln::trait::point_set< P >::has_speed

namespace mln
{

  struct line2d;
  template <typename P> struct p_array;
  template <typename P> struct p_bgraph;
  template <typename P> struct p_graph;
  template <typename P> struct p_line_graph;
  template <typename P, typename T> struct p_priority;
  template <typename P, typename T> struct p_priority_fast;
  template <typename P, typename T, unsigned S> struct p_priority_fast_with_array;
  template <typename P> struct p_queue;
  template <typename P> struct p_queue_fast;
  template <typename P> struct p_run;
  template <typename P> struct p_runs_;
  template <typename P> struct p_set;
  template <typename S, typename F> struct p_if;

  namespace trait
  {

    namespace point_set
    {

      struct arity
      {
	struct any {};
	struct unique : any
	{ std::string name() const { return "arity::unique"; } };
	struct multiple : any
	{ std::string name() const { return "arity::multiple"; } };
      };

      struct has_speed
      {
	struct any {};
	struct slow : any
	{ std::string name() const { return "has_speed::slow"; } };
	struct fast : any
	{ std::string name() const { return "has_speed::fast"; } };
      };

    } // end of namespace mln::trait::point_set

    template <typename P>
    struct undefined_point_set_
    {
      typedef undef arity;      // unique or multiple
      typedef undef has_speed;  // slow or fast
    };

    template <typename P>
    struct point_set_ : public undefined_point_set_<P>
    {
    };

    template <typename P>
    struct default_point_set_ : public undefined_point_set_<P>
    {
      typedef trait::point_set::arity::unique   arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_POINT_SET_HH
