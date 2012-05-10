// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_CORE_CONCEPT_ACCUMULATOR_HH
# define MLN_CORE_CONCEPT_ACCUMULATOR_HH

/// \file
///
/// Definition of the concept of mln::Accumulator.

# include <mln/core/concept/proxy.hh>
# include <mln/metal/fix_return.hh>
# include <mln/metal/const.hh>
# include <mln/trait/accumulators.hh>


namespace mln
{

  // Accumulator category flag type.
  template <>
  struct Accumulator<void>
  {
    typedef Proxy<void> super;
  };



  /// \brief Base class for implementation of accumulators.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \see mln::doc::Accumulator for a complete documentation of this
  /// class contents.
  ///
  template <typename E>
  struct Accumulator : public Proxy<E>
  {
    typedef Accumulator<void> category;

    /*
      typedef argument;
      typedef result;
      typedef q_result;

      void init();
      void take(const argument& t);
      void take(const E& other);

      q_result to_result() const;
      operator q_result const;

      bool is_valid() const;
     */

    /// Take as initialization the value \p t.
    ///
    /// Dev note: this is a final method; override if needed
    /// by take_as_init_ (ending with '_').
    template <typename T>
    void take_as_init(const T& t); // 't' is either argument or E.

    /// Default implementation of "take as initialization".
    template <typename T>
    void take_as_init_(const T& t);


    /// Take \p n times the value \p t.
    ///
    /// Dev note: this is a final method; override if needed
    /// by take_as_init_ (ending with '_').
    template <typename T>
    void take_n_times(unsigned n, const T& t);

    /// Default implementation of "take n times".
    template <typename T>
    void take_n_times_(unsigned n, const T& t);


  protected:
    Accumulator();
  };


  /// \internal Conversion: Accumulator -> mln_result(A)
  template <typename A>
  void
  from_to_(const Accumulator<A>& from, mln_result(A)& to);


# ifndef MLN_INCLUDE_ONLY

  // Accumulator<E>

  template <typename E>
  inline
  Accumulator<E>::Accumulator()
  {
    typedef mln_argument(E) argument;
    typedef mln_result(E)   result;
    typedef mln_q_result(E) q_result;

    void (E::*m1)() = & E::init;
    (void) m1;
    void (E::*m2)(const argument&) = & E::take;
    (void) m2;
    void (E::*m3)(const E&) = & E::take;
    (void) m3;

    q_result (E::*m4)() const = & E::to_result;
    (void) m4;
    q_result (E::*m5)() const = & E::operator q_result;
    (void) m5;

    bool (E::*m6)() const = & E::is_valid;
    (void) m6;
  }


  // take_as_init

  template <typename E>
  template <typename T>
  void
  Accumulator<E>::take_as_init(const T& t)
  {
    typedef mln_exact(T) T_;
    typedef mlc_converts_to(T_, mln_argument(E)) t_is_argument;
    typedef mlc_converts_to(T_, E)               t_is_accumulator;
    mlc_or(t_is_argument, t_is_accumulator)::check();

    // Dispatch.
    exact(this)->take_as_init_(t);
  }

  template <typename E>
  template <typename T>
  void
  Accumulator<E>::take_as_init_(const T& t)
  {
    // Default impl.
    exact(this)->init();
    exact(this)->take(t);
  }


  // take n times

  template <typename E>
  template <typename T>
  void
  Accumulator<E>::take_n_times(unsigned n, const T& t)
  {
    typedef mln_exact(T) T_;
    typedef mlc_converts_to(T_, mln_argument(E)) t_is_argument;
    typedef mlc_converts_to(T_, E)               t_is_accumulator;
    mlc_or(t_is_argument, t_is_accumulator)::check();

    if (n == 0u)
      return;

    // Dispatch.
    exact(this)->take_n_times_(n, t);
  }

  template <typename E>
  template <typename T>
  void
  Accumulator<E>::take_n_times_(unsigned n, const T& t)
  {
    // Default impl.
    for (unsigned i = 0; i < n; ++i)
      exact(this)->take(t);
  }


  // Conversions

  template <typename A>
  void
  from_to_(const Accumulator<A>& from_, mln_result(A)& to)
  {
    const A& from = exact(from_);
    if (from.is_valid())
      to = from.to_result();
    else
      to = mln_result(A)();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_ACCUMULATOR_HH
