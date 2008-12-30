// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_FROM_TO_HXX
# define MLN_CONVERT_FROM_TO_HXX

/// \internal \file mln/convert/from_to.hxx

// All overloaded "convert::from_to" routines have to be declared in
// the present file.  It allows for *deferred* use of these routines.

# include <mln/core/def/all.hh>
# include <mln/core/grids.hh>

//FIXME: have a forward declaration.
# include <vector>
# include <set>


namespace mln
{


  // Forward declarations.

  template <typename E> struct Accumulator;
  template <typename E> struct Gdpoint;
  template <typename E> struct Gpoint;
  template <typename E> struct Image;
  template <typename E> struct Object;
  template <typename E> struct Proxy;
  template <typename E> struct Site_Set;
  template <typename E> struct Value;
  template <typename E> struct Weighted_Window;
  template <typename E> struct Window;

  template <typename G, typename C> struct dpoint;
  template <typename G, typename C> struct point;

  template <typename D> class neighb;
  template <typename D> class window;
  template <typename D, typename W> class w_window;

  namespace algebra {
    template <unsigned n, typename T> class vec;
    template <unsigned d, typename C> class h_vec;
  }

  namespace fun {
    namespace i2v {
      template <typename T> class array;
    }
  }

  namespace util {
    template <typename T> class array;
  }

  namespace value {
    template <unsigned n> struct rgb;
  }

  // end of Forward declarations.



  // convert::to

  namespace convert
  {

    template <typename T, typename O>
    T
    to(const O& from);

  } // end of namespace mln::convert



  // convert::from_to

  namespace convert
  {

    // Facade

    template <typename F, typename T>
    void
    from_to(const F& from, T& to);


    namespace over_load
    {

      // Guard.
      template <typename F, typename T>
      void
      from_to_(const Object<F>&, Object<T>&);
      // end of Guard.

      // Object -> Object
      template <typename T>
      void
      from_to_(const Object<T>& from, Object<T>& to);

      // Object -> Object
      template <typename T>
      void
      from_to_(const T& from, T& to);


      // algebra::vec -> Gpoint.
      template <unsigned n, typename T, typename P>
      void
      from_to_(const algebra::vec<n,T>& from, Gpoint<P>& to_);

      // algebra::vec -> rgb.
      template <typename T, unsigned m>
      void
      from_to_(const algebra::vec<3,T>& from, value::rgb<m>& to);


      // bool -> rgb.
      template <unsigned m>
      void
      from_to_(bool from, value::rgb<m>& to);


      // C-array -> Image.
      template <typename V, unsigned S, typename I>
      void
      from_to_(const V (&values)[S], Image<I>& to);

      // C-array -> w_window
      template <typename V, unsigned S, typename D, typename W>
      void
      from_to_(const V (&weight)[S], w_window<D,W>& to);


      // C-array -> window2d
      template <unsigned S>
      void
      from_to_(const bool (&values)[S],
	       window< dpoint<grid::square, def::coord> >& win);

      template <unsigned R, unsigned C>
      void
      from_to_(const bool (&values)[R][C],
	       window< dpoint<grid::square, def::coord> >& win);

      // C-array -> neighb2d
      template <unsigned S>
      void
      from_to_(const bool (&values)[S],
	       neighb< window< dpoint<grid::square, def::coord> > >& nbh);

      template <unsigned R, unsigned C>
      void
      from_to_(bool const (&values)[R][C],
	       neighb< window< dpoint<grid::square, def::coord> > >& nbh);

      // Gdpoint -> point
      template <typename D>
      void
      from_to_(const Gdpoint<D>& from, mln_site(D)& to);


      // Value -> Value
      template <typename F, typename T>
      void
      from_to_(const Value<F>& from, Value<T>& to);

      // float -> Value
      template <typename V>
      void
      from_to_(const float& from, Value<V>& to);

      // float -> unsigned
      void
      from_to_(const float& from, unsigned& to);

      // float -> int
      void
      from_to_(const float& from, int& to);

      // int -> Value
      template <typename T>
      void
      from_to_(const int& from, Value<T>& to);

      // Proxy -> T
      template <typename P, typename T>
      void
      from_to_(const Proxy<P>& from, T& to);


      // Gpoint -> algebra::vec.
      template <typename P, unsigned n, typename T>
      void
      from_to_(const Gpoint<P>& from, algebra::vec<n,T>& to);

      // Gpoint -> delta-point
      template <typename P>
      void
      from_to_(const Gpoint<P>& from, mln_delta(P)& to);


      // Image -> Site_Set.
      template <typename I, typename S>
      void
      from_to_(const Image<I>& from, Site_Set<S>& to);

      // Image -> w_window
      template <typename I, typename D, typename W>
      void
      from_to_(const Image<I>& from, w_window<D,W>& to);


      // neighb<W> -> W
      template <typename W>
      void
      from_to_(const mln::neighb<W>& from, W& to);

      // W -> neighb<W>
      template <typename W>
      void
      from_to_(const W& from, mln::neighb<W>& to);


      // Window -> Image
      template <typename W, typename I>
      void
      from_to_(const Window<W>& from, Image<I>& to);


      // w_window -> Image
      template <typename D, typename W, typename I>
      void
      from_to_(const w_window<D,W>& from, Image<I>& to);



      // util::array<T> -> fun::i2v::array<T>
      template <typename T>
      void
      from_to_(const util::array<T>& from, fun::i2v::array<T>& to);

      // std::vector<T> -> fun::i2v::array<T>
      template <typename T>
      void
      from_to_(const std::vector<T>& from, fun::i2v::array<T>& to);

      // util::array<T1> -> util::array<T2>
      template <typename T1, typename T2>
      void
      from_to_(const util::array<T1>& from, util::array<T2>& to);


      // Accumulator<A> -> mln_result(A)
      template <typename A>
      void
      from_to_(const Accumulator<A>& from, mln_result(A)& to);


      // Site_Set -> std::set
      template <typename S, typename P, typename _C>
      void
      from_to_(const Site_Set<S>& from, std::set<P,_C>& to);

      // std::set -> Site_Set
      template <typename P, typename _C, typename S>
      void
      from_to_(const std::set<P,_C>& from, Site_Set<S>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_FROM_TO_HXX
