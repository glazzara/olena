// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_INTERNAL_RUN_IMAGE_HH
# define MLN_CORE_INTERNAL_RUN_IMAGE_HH

/*! \file
 *
 * \brief Definition of mln::internal::run_image_ class for internal use only
 */

# include <mln/core/internal/image_primary.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/core/site_set/p_set_of.hh>

namespace mln
{

  namespace internal
  {

    /*!
      \brief Factorization class for run_image.

      Parameter \c T is the type of the image value.
      Parameter \c P is the type of the image point.
      Parameter \c E is the Exact type of the image.
    */
    template <typename T, typename P, typename E>
    class run_image_ : public internal::image_primary< P, p_set_of< p_run<P> >, E >
    {
    protected:
      run_image_();

    public:
      /*! \brief Give the rate of compression.
       *
       * Give the rate of space gained by coding an image in this
       * format.  A rate of 1 means there is no compression. Less than
       * 1 means we have gained space.
       *
       * \return The rate of compression.
       */
      float compression() const;

      /*! \brief Finalize the construction.
       *
       * For internal use, this method has to be called to have
       * actually an lighter image. So it improves compression.
       */
      void finalize();

      /*! \brief Give the point represented by the point-site.
       *
       * Use the coordinate of the psite to see what point it corresponds
       * with the domain of the image.
       *
       * \return The corresponding point.
       */
      P point_at(const mln_psite(p_set_of< p_run<P> >)& ps) const;

      /*! \brief Tell if the image has the given point site.
       *
       * \return True if the image has the point site, else false.
       */
      bool has(const mln_psite(p_set_of< p_run<P> >)& ps) const;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename P, typename E>
    inline
    run_image_<T, P, E>::run_image_()
    {
    }

    template <typename T, typename P, typename E>
    inline
    float
    run_image_<T, P, E>::compression() const
    {
      return float(exact(this)->data_->memory_size()) /
	float (sizeof(T) * exact(this)->data_->domain_.bbox().nsites());
    }

    template <typename T, typename P, typename E>
    inline
    void
    run_image_<T, P, E>::finalize()
    {
      exact(this)->data_->finalize();
    }

    template <typename T, typename P, typename E>
    inline
    P
    run_image_<T, P, E>::point_at(const mln_psite(p_set_of< p_run<P> >)& ps) const
    {
      const E* ima = exact(this);
      mln_precondition(ps.index() < ima->data_->domain_.nsites());
      mln_precondition(ps.p().index() < ima->data_->domain_[ps.index()].length());
      return ima->data_->domain_[ps.index()][ps.p().index()];
    }

    template <typename T, typename P, typename E>
    inline
    bool
    run_image_<T, P, E>::has(const mln_psite(p_set_of< p_run<P> >)& ps) const
    {
      const E* ima = exact(this);
      if (!ima->is_valid())
	return false;
      else
	return (ps.index() < ima->data_->domain_.nsites()
		&& ps.p().index() < ima->data_->domain_[ps.index()].length()
		&& ima->data_->domain_[ps.index()][ps.p().index()] == ps);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_RUN_IMAGE_HH
