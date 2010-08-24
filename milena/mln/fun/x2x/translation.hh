// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_X2X_TRANSLATION_HH
# define MLN_FUN_X2X_TRANSLATION_HH

/// \file
///
/// \brief Define a translation function.

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_linear_impl.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/h_mat.hh>
# include <mln/fun/i2v/all.hh>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      /// Translation function-object.
      //
      template <unsigned n, typename C>
      struct translation
        :
	fun::internal::x2x_linear_impl_< algebra::vec<n,C>, C,
					 translation<n,C> >,
        public Function_v2v< translation<n,C> >
      {
	/// Type of the underlying data stored in vectors and matrices.
	typedef C data_t;

        /// Type of the inverse function.
        typedef translation<n,C> invert;
        /// Return the inverse function.
        invert inv() const;

        /// Constructor without argument.
        translation();
        /// Constructor with the translation vector.
        translation(const algebra::vec<n,C>& t);

        /// Perform the translation of the given vector
        algebra::vec<n,C> operator()(const algebra::vec<n,C>& v) const;
        algebra::vec<n,C> inverse(const algebra::vec<n,C>& v) const;


        /// Set a net translation vector.
        void set_t(const algebra::vec<n,C>& t);

	/// Return the translation vector.
	const algebra::vec<n,C>& t() const;

      protected:
        void update();

        algebra::vec<n,C> t_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      inline
      translation<n,C>::translation()
      {
      }

      template <unsigned n, typename C>
      inline
      translation<n,C>::translation(const algebra::vec<n,C>& t)
        :t_(t)
      {
        this->update();
      }

      template <unsigned n, typename C>
      inline
      algebra::vec<n,C>
      translation<n,C>::operator()(const algebra::vec<n,C>& v) const
      {
        return v + t_;
      }

      template <unsigned n, typename C>
      inline
      algebra::vec<n,C>
      translation<n,C>::inverse(const algebra::vec<n,C>& v) const
      {
        return v - t_;
      }

      template <unsigned n, typename C>
      inline
      translation<n,C>
      translation<n,C>::inv() const
      {
        typename translation::invert res(-t_);

        return res;
      }

      template <unsigned n, typename C>
      inline
      void
      translation<n,C>::set_t(const algebra::vec<n,C>& t)
      {
        this->t_ = t;
        this->update();
      }

      template <unsigned n, typename C>
      inline
      const algebra::vec<n,C>&
      translation<n,C>::t() const
      {
        return this->t_;
      }

      template <unsigned n, typename C>
      inline
      void
      translation<n,C>::update()
      {
        this->m_ = algebra::h_mat<n,C>::Id;
        for (unsigned i = 0; i < n; ++i)
          this->m_(i,n) = this->t_[i];
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_TRANSLATION_HH
