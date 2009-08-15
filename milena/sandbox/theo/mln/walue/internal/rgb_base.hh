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

#ifndef MLN_WALUE_INTERNAL_RGB_BASE_HH
# define MLN_WALUE_INTERNAL_RGB_BASE_HH

/// \file
/// \brief FIXME

# include <mln/walue/rgb.hh>
# include <mln/algebra/vec.hh>


namespace mln
{

  namespace walue
  {

    namespace internal
    {

      /// \brief FIXME
      //
      template <typename R, typename G, typename B,
		typename E>
      class rgb_base : public Rgb< E >
      {
      public:

	typedef R red_t;
	R  red() const;
	R& red();

	typedef G green_t;
	G  green() const;
	G& green();

	typedef B blue_t;
	B  blue() const;
	B& blue();

      protected:

	rgb_base();
	rgb_base(const R& r, const G& g, const B& b);

	R r_;
	G g_;
	B b_;
      };


      /// \brief FIXME
      //
      template <typename T, typename E>
      class rgb_base<T,T,T, E> : public Rgb< E >
      {
      public:

	typedef T red_t;
	T  red() const;
	T& red();

	typedef T green_t;
	T  green() const;
	T& green();

	typedef T blue_t;
	T  blue() const;
	T& blue();

	const algebra::vec<3,T>& to_vec() const;

      protected:

	rgb_base();

	// FIXME: Allow input with a type U different from T...
	rgb_base(const T& r, const T& g, const T& b);

	T v_[3];
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename R, typename G, typename B, typename E>
      inline
      rgb_base<R,G,B,E>::rgb_base()
      {
      }

      template <typename R, typename G, typename B, typename E>
      inline
      rgb_base<R,G,B,E>::rgb_base(const R& r, const G& g, const B& b)
	: r_(r), g_(g), b_(b)
      {
      }

      template <typename R, typename G, typename B, typename E>
      inline
      R rgb_base<R,G,B,E>::red() const
      {
	return r_;
      }

      template <typename R, typename G, typename B, typename E>
      inline
      R& rgb_base<R,G,B,E>::red()
      {
	return r_;
      }

      template <typename R, typename G, typename B, typename E>
      inline
      G rgb_base<R,G,B,E>::green() const
      {
	return g_;
      }

      template <typename R, typename G, typename B, typename E>
      inline
      G& rgb_base<R,G,B,E>::green()
      {
	return g_;
      }

      template <typename R, typename G, typename B, typename E>
      inline
      B rgb_base<R,G,B,E>::blue() const
      {
	return b_;
      }

      template <typename R, typename G, typename B, typename E>
      inline
      B& rgb_base<R,G,B,E>::blue()
      {
	return b_;
      }

      // Specialization.

      template <typename T, typename E>
      inline rgb_base<T,T,T,E>::rgb_base()
      {
      }

      template <typename T, typename E>
      inline
      rgb_base<T,T,T,E>::rgb_base(const T& r, const T& g, const T& b)
      {
	v_[0] = r;
	v_[1] = g;
	v_[2] = b;
      }

      template <typename T, typename E>
      inline
      T rgb_base<T,T,T,E>::red() const
      {
	return v_[0];
      }

      template <typename T, typename E>
      inline
      T& rgb_base<T,T,T,E>::red()
      {
	return v_[0];
      }

      template <typename T, typename E>
      inline
      T rgb_base<T,T,T,E>::green() const
      {
	return v_[1];
      }

      template <typename T, typename E>
      inline
      T& rgb_base<T,T,T,E>::green()
      {
	return v_[1];
      }

      template <typename T, typename E>
      inline
      T rgb_base<T,T,T,E>::blue() const
      {
	return v_[2];
      }

      template <typename T, typename E>
      inline
      T& rgb_base<T,T,T,E>::blue()
      {
	return v_[2];
      }

      template <typename T, typename E>
      inline
      const algebra::vec<3,T>&
      rgb_base<T,T,T,E>::to_vec() const
      {
	return *(const algebra::vec<3,T>*)(v_);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::walue::internal

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_RGB_BASE_HH
