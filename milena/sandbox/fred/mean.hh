// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MORPHO_ATTRIBUTE_MEAN_HH
# define MLN_MORPHO_ATTRIBUTE_MEAN_HH

/// \file mln/morpho/attribute/mean.hh
///
/// Define an accumulator that computes the mean of a
/// component.
///

# include <mln/accu/internal/base.hh>
# include <mln/accu/mean.hh>
# include <mln/util/pix.hh>

namespace mln {

  // Forward declaration.

  namespace morpho {
    namespace attribute {
      template <typename I> class mean;
    }
  }

  // Traits

  namespace trait {
    template <typename I>
    struct accumulator_< morpho::attribute::mean<I> >
    {
      typedef accumulator::has_untake::no	has_untake;
      typedef accumulator::has_set_value::no	has_set_value;
      typedef accumulator::has_stop::no		has_stop;
      typedef accumulator::when_pix::use_v	when_pix;
    };
  }


  namespace morpho {

    namespace attribute {

      /// Morphological (i.e. for pixel and pixel values) accumulator calculating mean.
      /// Valid for leveling algorithm.
      /// FIXME: is inclusion polyphormism really appliable ?
      template <typename I>
      class mean: public mln::accu::internal::base<typename mln::accu::mean<mln_value(I)>::result, mean<I> >
      {
      public:
        typedef util::pix<I> argument;
        typedef typename mln::accu::mean<mln_value(I)>::result result;
        typedef typename mln::accu::mean<mln_value(I)>::q_result q_result;

        mean();


	/// Manipulators.
	/// \{
        void init();

        void take(const mln_value(I)& v);
        void take(const argument& t);
        void take(const mean<I>& m);
	/// \}

	/// Get the value of the accumulator.
        result to_result() const;

	/// Check whether this accu is able to return a result.
        bool is_valid() const;

      protected:
	/// The mean
        mln::accu::mean<mln_value(I)> accu_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      mean<I>::mean () :
        accu_ ()
      {
      }

      template <typename I>
      inline
      void
      mean<I>::init ()
      {
        accu_.init();
      }

      template <typename I>
      inline
      void
      mean<I>::take (const mean<I>& accu)
      {
        accu_.take(accu.accu_);
      }

      template <typename I>
      inline
      void
      mean<I>::take (const util::pix<I>& elt)
      {
        take(elt.v());
      };


      template <typename I>
      inline
      void
      mean<I>::take (const mln_value(I)& elt)
      {
        accu_.take(elt);
      };

      template <typename I>
      inline
      typename mean<I>::result
      mean<I>::to_result() const
      {
        return accu_.to_result();
      };

      template <typename I>
      inline
      bool
      mean<I>::is_valid() const
      {
        return accu_.is_valid();
      };

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln

#endif /* ! MLN_MORPHO_ATTRIBUTE_MEAN_HH */
