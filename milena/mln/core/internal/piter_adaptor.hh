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

#ifndef MLN_CORE_INTERNAL_PITER_ADAPTOR_HH
# define MLN_CORE_INTERNAL_PITER_ADAPTOR_HH

/*! \file mln/core/internal/piter_adaptor.hh
 *
 * \brief Definition of iterators on points of boxes.
 */

# include <mln/core/concept/piter.hh>
# include <mln/core/concept/box.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A base class for point iterator adaptors.
     *
     * Parameter \c Pi is the type of the point iterator adaptee;
     * parameter E is the exact type.
     */
    template <typename Pi, typename E>
    class piter_adaptor_ : public Piter<E>
    {
    public:

      /// Space dimension.
      enum { dim = Pi::dim };

      /// Psite associated type.
      typedef mln_psite(Pi) psite;

      /// Point associated type.
      typedef mln_point(Pi) point;

      /// Dpoint associated type.
      typedef mln_dpoint(Pi) dpoint;

      /// Coordinate associated type.
      typedef mln_coord(Pi) coord;


      /// Constructor from a point iterator \p piter.
      piter_adaptor_(const Pi& piter);


      /// Convertion to point.
      operator mln_point(Pi) () const;

      /// Address of the point.
      const point* pointer() const;

      /// Give the i-th coordinate.
      coord operator[](unsigned i) const;

      /// Test the iterator validity.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next point.
      void next_();
    
    protected:

      Pi piter_; // own copy
    };




# ifndef MLN_INCLUDE_ONLY


    template <typename Pi, typename E>
    piter_adaptor_<Pi,E>::piter_adaptor_(const Pi& piter)
      : piter_(piter)
    {
      invalidate();
    }

    template <typename Pi, typename E>
    piter_adaptor_<Pi,E>::operator mln_point(Pi) () const
    {
      return piter_;
    }

    template <typename Pi, typename E>
    const mln_point(Pi)*
    piter_adaptor_<Pi,E>::pointer() const
    {
      return piter_.pointer();
    }

    template <typename Pi, typename E>
    mln_coord(Pi)
    piter_adaptor_<Pi,E>::operator[](unsigned i) const
    {
      assert(i < dim);
      return piter_[i];
    }

    template <typename Pi, typename E>
    bool
    piter_adaptor_<Pi,E>::is_valid() const
    {
      return piter_.is_valid();
    }

    template <typename Pi, typename E>
    void
    piter_adaptor_<Pi,E>::invalidate()
    {
      piter_.invalidate();
    }

    template <typename Pi, typename E>
    void
    piter_adaptor_<Pi,E>::start()
    {
      piter_.start();
    }

    template <typename Pi, typename E>
    void
    piter_adaptor_<Pi,E>::next_()
    {
      piter_.next();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PITER_ADAPTOR_HH
