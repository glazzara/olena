// Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_FUN_X2X_COMPOSED_HH
# define MLN_FUN_X2X_COMPOSED_HH

/// \file
///
/// \brief Definition of a composed transformation.

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_linear_impl.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/is.hh>
# include <mln/metal/bexpr.hh>
# include <mln/algebra/h_mat.hh>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      // Forward declaration.
      template <typename T2, typename T1>
      struct composed;

      namespace internal
      {
        template <typename T2, typename T1, typename E, bool is_bij>
        struct helper_composed_;


        /*!
	  \internal
	  \brief Helper for describing a bijective composition.
	*/
        template <typename T2, typename T1, typename E>
        struct helper_composed_<T2,T1,E,true>
          : public fun::internal::x2x_linear_impl_<mln_result(T2), typename T2::data_t, E >,
            public Function_v2v<E>
        {
          typedef fun::internal::x2x_linear_impl_<typename T2::result, typename T2::data_t, E > super_;

	  /// Type of the underlying data stored in vectors and matrices.
	  typedef typename T2::data_t data_t;

          using super_::dim;

          /// Constructor without argument.
          helper_composed_();
          /// Constructor with the two transformation to be composed.
          helper_composed_(const T2& f, const T1& g);

          using super_::operator();

	  /// Set the new second transformation.
          const T2& second() const;
          /// Set the new first transformation.
	  const T1& first() const;

          /// Set the new second transformation.
          void set_second(const T2& f);
          /// Set the new first transformation.
          void set_first(const T1& g);

          /// Type of the inverse function.
          typedef composed<mln_invert(T1),mln_invert(T2)> invert;
          /// Return the inverse function.
          invert inv() const;

          //protected:

          T2 f_;
          T1 g_;
        };

        /*!
	  \internal
	  \brief Helper for describing a non bijective composition.
	*/
        template <typename T2, typename T1, typename E>
        struct helper_composed_<T2,T1,E,false>
          : public fun::internal::x2x_linear_impl_<mln_result(T2), typename T2::data_t, E >,
            public Function_v2v<E>
        {
          typedef fun::internal::x2x_linear_impl_<typename T2::result, typename T2::data_t, E > super_;


	  /// Type of the underlying data stored in vectors and matrices.
	  typedef typename T2::data_t data_t;

          using super_::dim;

          /// Constructor without argument.
          helper_composed_();
          /// Constructor with the two transformation to be composed.
          helper_composed_(const T2& f, const T1& g);

          using super_::operator();

          /// Set the new second transformation.
          void set_second(const T2& f);
          /// Set the new first transformation.
          void set_first(const T1& g);

	  /// Set the new second transformation.
	  const T2& second() const;
          /// Set the new first transformation.
          const T1& first() const;

          //protected:

          T2 f_;
          T1 g_;
        };

      } // end of namespace mln::fun::x2x::internal


      /// Represent a composition of two transformations.
      template <typename T2, typename T1>
      struct composed
        : public internal::helper_composed_< T2, T1, composed<T2,T1>,
                                             (mlc_is(T2, Function_v2v<T2>)::value &&
					      mlc_is(T1, Function_v2v<T1>)::value) >,
          private metal::and_< metal::bool_<(T2::dim == T1::dim)>,
                               metal::is<mln_argument(T2), mln_result(T1)>
                               >::check_t
      {
        /// Constructor without argument.
        composed() {}

        /// Constructor with the two transformation to be composed.
        composed(const T2& f, const T1& g)
          : internal::helper_composed_< T2, T1, composed<T2,T1>,
                                        (mlc_is(T2, Function_v2v<T2>)::value &&
					 mlc_is(T1, Function_v2v<T1>)::value) >(f, g)
        {}
      };

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun


  /// Do a composition of two transformations
  ///
  /// \param[in] f The second transformation.
  /// \param[in] g The first transformation.
  ///
  /// \return The composed transformation fog.
  template <typename T2, typename T1>
  fun::x2x::composed<T2,T1> compose(T2 f, T1 g);

# ifndef MLN_INCLUDE_ONLY

  namespace fun
  {

    namespace x2x
    {

      namespace internal
      {

        // Implementation of the bijective version.

        template <typename T2, typename T1, typename E>
        inline
        helper_composed_<T2,T1,E,true>::helper_composed_()
        {
        }

        template <typename T2, typename T1, typename E>
        inline
        helper_composed_<T2,T1,E,true>::helper_composed_(const T2& f, const T1& g)
          :f_(f),
           g_(g)
        {
          this->m_ = f_.mat() * g_.mat();
        }

        template <typename T2, typename T1, typename E>
        inline
        typename helper_composed_<T2,T1,E,true>::invert
        helper_composed_<T2,T1,E,true>::inv() const
        {
          return compose(g_.inv(), f_.inv());
        }

        template <typename T2, typename T1, typename E>
        inline
        void
        helper_composed_<T2,T1,E,true>::set_second(const T2& f)
        {
          this->f_ = f;
          this->m_ = this->f_.mat() * this->g_.mat();
        }

        template <typename T2, typename T1, typename E>
        inline
        void
        helper_composed_<T2,T1,E,true>::set_first(const T1& g)
        {
          this->g_ = g;
          this->m_ = this->f_.mat() * this->g_.mat();
        }

	template <typename T2, typename T1, typename E>
        inline
        const T2&
        helper_composed_<T2,T1,E,true>::second() const
        {
	  return this->f_;
        }

        template <typename T2, typename T1, typename E>
        inline
        const T1&
        helper_composed_<T2,T1,E,true>::first() const
        {
	  return this->g_;
        }

        // Implementation of the non bijective version.
        template <typename T2, typename T1, typename E>
        inline
        helper_composed_<T2,T1,E,false>::helper_composed_()
        {
        }

        template <typename T2, typename T1, typename E>
        inline
        helper_composed_<T2,T1,E,false>::helper_composed_(const T2& f, const T1& g)
          :f_(f),
           g_(g)
        {
          this->m_ = f_.mat() * g_.mat();
        }

        template <typename T2, typename T1, typename E>
        inline
        void
        helper_composed_<T2,T1,E,false>::set_second(const T2& f)
        {
          this->f_ = f;
          this->m_ = this->f_.mat() * this->g_.mat();
        }

        template <typename T2, typename T1, typename E>
        inline
        void
        helper_composed_<T2,T1,E,false>::set_first(const T1& g)
        {
          this->g_ = g;
          this->m_ = this->f_.mat() * this->g_.mat();
        }

	template <typename T2, typename T1, typename E>
        inline
        const T2&
        helper_composed_<T2,T1,E,false>::second() const
        {
	  return this->f_;
        }

        template <typename T2, typename T1, typename E>
        inline
        const T1&
        helper_composed_<T2,T1,E,false>::first() const
        {
	  return this->g_;
        }


      } // end of namespace mln::fun::x2x::internal

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

  template <typename T2, typename T1>
  inline
  fun::x2x::composed<T2,T1> compose(T2 f, T1 g)
  {
    mln_trace("fun::x2x::compose");
    fun::x2x::composed<T2,T1> comp(f, g);
    return comp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_X2X_COMPOSED_HH
