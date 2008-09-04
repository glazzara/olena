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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_FUN_X2X_COMPOSED_HH
# define MLN_FUN_X2X_COMPOSED_HH

/*! \file mln/fun/x2x/composed.hh
 *
 * \brief Definition of a composed transformation.
 */

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

      // Fwd decl.
      template <typename F, typename G>
      struct composed;

      namespace internal
      {
	/// \internal
	template <typename F, typename G, typename E, bool is_bij>
	struct helper_composed_;


	/// \internal Helper for describing a bijective composition.
	template <typename F, typename G, typename E>
	struct helper_composed_< F, G, E, true>
	  : public fun::internal::x2x_linear_impl_<mln_result(F), E >,
	    public Bijection_x2x<E>
	{
	  typedef fun::internal::x2x_linear_impl_<typename F::result, E > super_;

	  using super_::dim;

	  /// Constructor without argument.
	  helper_composed_();
	  /// Constructor with the two transformation to be composed.
	  helper_composed_(const F& f, const G& g);

	  using super_::operator();

	  /// Set the new first transformation.
	  void set_first(const F& f);
	  /// Set the new second transformation.
	  void set_second(const G& g);

	  /// Type of the inverse function.
	  typedef composed<mln_invert(G),mln_invert(F)> invert;
	  /// Return the inverse function.
	  invert inv() const;

	  protected:

	  F f_;
	  G g_;
	};

	/// \internal Helper for describing a non bijective composition.
	template <typename F, typename G, typename E>
	struct helper_composed_< F, G, E, false>
	  : public fun::internal::x2x_linear_impl_<mln_result(F), E >,
	    public Function_x2x<E>
	{
	  typedef fun::internal::x2x_linear_impl_<typename F::result, E > super_;

	  using super_::dim;

	  /// Constructor without argument.
	  helper_composed_();
	  /// Constructor with the two transformation to be composed.
	  helper_composed_(const F& f, const G& g);

	  using super_::operator();

	  /// Set the new first transformation.
	  void set_first(const F& f);
	  /// Set the new second transformation.
	  void set_second(const G& g);

	  protected:

	  F f_;
	  G g_;
	};

      } // end of namespace mln::fun::x2x::internal

      /*! \brief Represent a composition of two transformations.
       *
       */
      template <typename F, typename G>
      struct composed
	: public internal::helper_composed_<F, G, composed<F,G>,
					    mlc_is(F, Bijection_x2x<F>)::value &&
					    mlc_is(G, Bijection_x2x<G>)::value>,


          private metal::and_< metal::bool_<(F::dim == G::dim)>,
			       metal::is<mln_argument(F), mln_result(G)>
                             >::check_t
      {
	/// Constructor without argument.
	composed() {}

	/// Constructor with the two transformation to be composed.
	composed(const F& f, const G& g)
	  : internal::helper_composed_<F, G, composed<F,G>,
	  mlc_is(F, Bijection_x2x<F>)::value &&
	  mlc_is(G, Bijection_x2x<G>)::value>(f, g)
	  {
	  }
      };

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

  /*! \brief Do a composition of two transformations
   *
   * \param[in] f The first transformation.
   * \param[in] g The second transformation.
   *
   * \return The composed transformation fog.
   */
  template <typename F, typename G>
  fun::x2x::composed<F,G> compose(F f, G g);

# ifndef MLN_INCLUDE_ONLY

  namespace fun
  {

    namespace x2x
    {

      namespace internal
      {

	// \internal Implementation of the bijective version.

	template <typename F, typename G, typename E>
	inline
	helper_composed_<F,G,E,true>::helper_composed_()
	{
	}

	template <typename F, typename G, typename E>
	inline
	helper_composed_<F,G,E,true>::helper_composed_(const F& f, const G& g)
	  :f_(f),
	   g_(g)
	{
	  this->m_ = f_.mat() * g_.mat();
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	typename helper_composed_<F,G,E,true>::invert
	helper_composed_<F,G,E,true>::inv() const
	{
	  return compose(g_.inv(), f_.inv());
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	void
	helper_composed_<F,G,E,true>::set_first(const F& f)
	{
	  this->f_ = f;
	  this->m_ = this->f_.mat() * this->g_.mat();
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	void
	helper_composed_<F,G,E,true>::set_second(const G& g)
	{
	  this->g_ = g;
	  this->m_ = this->f_.mat() * this->g_.mat();
	}

	// \internal Implementation of the non bijective version.
	template <typename F, typename G, typename E>
	inline
	helper_composed_<F,G,E,false>::helper_composed_()
	{
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	helper_composed_<F,G,E,false>::helper_composed_(const F& f, const G& g)
	  :f_(f),
	   g_(g)
	{
	  this->m_ = f_.mat() * g_.mat();
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	void
	helper_composed_<F,G,E,false>::set_first(const F& f)
	{
	  this->f_ = f;
	  this->m_ = this->f_.mat() * this->g_.mat();
	}

	/// \internal
	template <typename F, typename G, typename E>
	inline
	void
	helper_composed_<F,G,E,false>::set_second(const G& g)
	{
	  this->g_ = g;
	  this->m_ = this->f_.mat() * this->g_.mat();
	}

      } // end of namespace mln::fun::x2x::internal

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

  template <typename F, typename G>
  inline
  fun::x2x::composed<F,G> compose(F f, G g)
  {
    trace::entering("fun::x2x::compose");
    fun::x2x::composed<F,G> comp(f, g);
    trace::exiting("fun::x2x::compose");
    return comp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_X2X_COMPOSED_HH
