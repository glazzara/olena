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

#ifndef MLN_MORPHO_ATTRIBUTE_CARD_HH
# define MLN_MORPHO_ATTRIBUTE_CARD_HH

/// \file
///
/// \brief Define an accumulator that computes the cardinality of a
/// component.
///
/// \todo Add a static check in the accumulator concept so that
/// when_pix::use_none => take_as_init() is provided.

# include <mln/accu/internal/base.hh>
# include <mln/util/pix.hh>


namespace mln
{


  // Forward declaration.

  namespace morpho {
    namespace attribute {
      template <typename I> class card;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::card<I> >
    {
      typedef accumulator::has_untake::no     has_untake;
      typedef accumulator::has_set_value::no  has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::use_none when_pix;
    };

  } // end of namespace mln::trait


  namespace morpho
  {

    namespace attribute
    {

      /// Cardinality accumulator class.

      template <typename I>
      class card : public mln::accu::internal::base< unsigned, card<I> >
      {
	typedef mln::accu::internal::base< unsigned, card<I> > super_;
      public:

	typedef mln_psite(I) argument;

	card();

	/// Manipulators.
	/// \{
	void init();

	void take();
	void take(const argument& s);
	void take(const util::pix<I>& px);
	void take(const card<I>& other);

	void take_as_init(); // Extra version.
	void take_as_init_(const util::pix<I>& px); // Overload.
	/// \}

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:
	/// The value.
	unsigned c_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      card<I>::card()
      {
	init();
      }

      template <typename I>
      inline
      void
      card<I>::init()
      {
	c_ = 0;
      }

      // take.

      template <typename I>
      inline
      void
      card<I>::take()
      {
	++c_;
      }

      template <typename I>
      inline
      void
      card<I>::take(const argument&)
      {
	take();
      }

      template <typename I>
      inline
      void
      card<I>::take(const util::pix<I>&)
      {
	take();
      }

      template <typename I>
      inline
      void
      card<I>::take(const card<I>& other)
      {
	c_ += other.c_;
      }

      // take_as_init.

      template <typename I>
      inline
      void
      card<I>::take_as_init()
      {
	init();
	take();
      }

      template <typename I>
      inline
      void
      card<I>::take_as_init_(const util::pix<I>&)
      {
	this->take_as_init();
      }

      template <typename I>
      inline
      unsigned
      card<I>::to_result() const
      {
	return c_;
      }

      template <typename I>
      inline
      bool
      card<I>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_CARD_HH
