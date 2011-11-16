// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_WINDOW_HH
# define MLN_CORE_CONCEPT_WINDOW_HH

/// \file
/// Definition of the concept of mln::Window.
///
/// \todo Operator== should test if the cmp is possible.
///
/// \todo The is_centered() method could also exist when the window is
/// not regular...

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>
# include <mln/trait/windows.hh>

# include <mln/core/site_set/p_array.hh>
# include <mln/core/internal/geom_bbox.hh> // For use in convert::from_to.
# include <mln/convert/from_to.hxx>
# include <mln/util/array.hh>



# define mln_is_simple_window(W)							\
											\
mln::metal::and_< mlc_is(mln_trait_window_size(W),					\
			 mln::trait::window::size::fixed),				\
		  mln::metal::and_< mlc_is(mln_trait_window_support(W),			\
					   mln::trait::window::support::regular),	\
				    mlc_is(mln_trait_window_definition(W),		\
					   mln::trait::window::definition::unique) > >


# define mln_is_fastest_IW(I, W)		\
						\
mlc_and(mlc_is(mln_trait_image_speed(I),	\
	       trait::image::speed::fastest),	\
	mln_is_simple_window(W))



namespace mln
{

  // Forward declarations.
  template <typename E> struct Window;
  template <typename E> struct Image;


  // Window category flag type.
  template <>
  struct Window<void>
  {
    typedef Object<void> super;
  };


  /// Base class for implementation classes that are windows.
  ///
  /// \see mln::doc::Window for a complete documentation of this class
  /// contents.
  template <typename E>
  struct Window : public Object<E>
  {
    typedef Window<void> category;

    /*
      typedef   site;
      typedef  psite;
      typedef dpsite;

      typedef     qiter;
      typedef fwd_qiter;
      typedef bkd_qiter;
    */

  protected:
    Window();
  };


  template <typename Wl, typename Wr>
  bool operator==(const Window<Wl>& lhs, const Window<Wr>& rhs);


  template <typename W>
  std::ostream& operator<<(std::ostream& ostr, const Window<W>& win);


  // FIXME: Move as a method of Image?
  template <typename I, typename W>
  util::array<int>
  offsets_wrt(const Image<I>& ima, const Window<W>& win);
  
  template <typename I, typename W>
  util::array<int>
  positive_offsets_wrt(const Image<I>& ima, const Window<W>& win);
  
  template <typename I, typename W>
  util::array<int>
  negative_offsets_wrt(const Image<I>& ima, const Window<W>& win);
  


  namespace convert
  {

    namespace over_load
    {

      template <typename W, typename I>
      void
      from_to_(const Window<W>& from, Image<I>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // size: fixed or unknown.

    template <typename trait_size, typename E>
    struct window_size_check
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct window_size_check< mln::trait::window::size::fixed, E >
    {
      static void run()
      {
	unsigned (E::*m)() const = & E::size;
	(void) m;
      }
    };

    // support: regular or irregular.

    template <typename trait_support, typename E>
    struct window_support_check
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct window_support_check< mln::trait::window::support::regular, E >
    {
      static void run_extra()
      {
	// Associated type.
	typedef mln_regular(E) regular;

	// Methods.
	bool (E::*m1)() const = &E::is_centered;
	(void) m1;
	bool (E::*m2)() const = &E::is_symmetric;
	(void) m2;
	void (E::*m3)() = &E::sym;
	(void) m3;
	unsigned (E::*m4)() const = &E::delta;
	(void) m4;
	bool (E::*m5)() const = &E::is_valid;
	(void) m5;
      }

      static void run(mln::trait::window::definition::unique)
      {
	typedef mln_dpsite(E) D;
	const D& (E::*m1)(unsigned) const = &E::dp;
	(void) m1;
	bool (E::*m2)(const D&) const = &E::has;
	(void) m2;
	run_extra();
      }

      static void run(mln::trait::window::definition::n_ary)
      {
	run_extra();
      }

      static void run(mln::trait::window::definition::varying)
      {
	/* No requirement. */
      }

      static void run()
      {
	run(mln_trait_window_definition(E)());
      }
    };

    // definition: unique, n_ary, or varying.

    template <typename trait_definition, typename E>
    struct window_definition_check
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct window_definition_check< mln::trait::window::definition::multiple, E >
    {
      static void run()
      {
	typedef mln_element(E) W;
	void (E::*m1)(unsigned, const W&) = &E::set_window;
	(void) m1;
	const W& (E::*m2)(unsigned) const = &E::window;
	(void) m2;
	unsigned (E::*m3)() const = &E::nwindows;
	(void) m3;
      }
    };

  } // end of namespace mln::internal


  template <typename E>
  inline
  Window<E>::Window()
  {
    // Check properties.
    mlc_not_equal( mln_trait_window_size(E),       mln::trait::undef )::check();
    mlc_not_equal( mln_trait_window_support(E),    mln::trait::undef )::check();
    mlc_not_equal( mln_trait_window_definition(E), mln::trait::undef )::check();

    // Check associated types.
    typedef   mln_site(E)   site;
    typedef  mln_psite(E)  psite;
    typedef mln_dpsite(E) dpsite;
    typedef     mln_qiter(E)     qiter;
    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;

    // Check methods depending upon properties.
    internal::window_size_check      < mln_trait_window_size(E),       E >::run();
    internal::window_support_check   < mln_trait_window_support(E),    E >::run();
    internal::window_definition_check< mln_trait_window_definition(E), E >::run();
  }

  template <typename Wl, typename Wr>
  inline
  bool operator==(const Window<Wl>& lhs, const Window<Wr>& rhs)
  {
    return exact(lhs).std_vector() == exact(rhs).std_vector();
  }


  // Operator <<.

  namespace internal
  {

    template <typename W>
    inline
    void print(trait::window::definition::unique,
	       std::ostream& ostr, const W& win) // FIXME: Add Window<W> to win?
    {
      win.print(ostr);
    }

    template <typename W>
    inline
    void print(trait::window::definition::multiple,
	       std::ostream& ostr, const W& win) // FIXME: Add Window<W> to win?
    {
      ostr << "[";
      const unsigned nw = win.nwindows();
      for (unsigned w = 0; w < nw; ++w)
	{
	  ostr << " #" << w << ':';
	  win.window_(w).print(ostr);
	}
      ostr << " ]";
    }
    
  } // end of namespace mln::internal

  template <typename W>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Window<W>& win)
  {
    mlc_is(mln_trait_window_support(W),
	   trait::window::support::regular)::check();
    mlc_is_not(mln_trait_window_definition(W),
	       trait::window::definition::varying)::check();
    // FIXME: test on is_empty?
    internal::print(mln_trait_window_definition(W)(),
		    ostr, exact(win));
    return ostr;
  }


  template <typename I, typename W>
  inline
  util::array<int>
  offsets_wrt(const Image<I>& ima_, const Window<W>& win_)
  {
    mln_is_simple_window(W)::check();

    const I& ima = exact(ima_);
    const W& win = exact(win_);
    mln_precondition(ima.is_valid());
    mln_precondition(win.is_valid());

    util::array<int> arr;
    unsigned n = win.size();
    
    for (unsigned i = 0; i < n; ++i)
      arr.append(ima.delta_index(win.dp(i)));

    return arr;
  }


  template <typename I, typename W>
  inline
  util::array<int>
  positive_offsets_wrt(const Image<I>& ima_, const Window<W>& win_)
  {
    mln_is_simple_window(W)::check();

    const I& ima = exact(ima_);
    const W& win = exact(win_);
    mln_precondition(ima.is_valid());
    mln_precondition(win.is_valid());

    util::array<int> arr;
    unsigned n = win.size();
    
    for (unsigned i = 0; i < n; ++i)
      {
	int offset = ima.delta_index(win.dp(i));
	if (offset > 0)
	  arr.append(offset);
      }

    return arr;
  }


  template <typename I, typename W>
  inline
  util::array<int>
  negative_offsets_wrt(const Image<I>& ima_, const Window<W>& win_)
  {
    mln_is_simple_window(W)::check();

    const I& ima = exact(ima_);
    const W& win = exact(win_);
    mln_precondition(ima.is_valid());
    mln_precondition(win.is_valid());

    util::array<int> arr;
    unsigned n = win.size();
    
    for (unsigned i = 0; i < n; ++i)
      {
	int offset = ima.delta_index(win.dp(i));
	if (offset < 0)
	  arr.append(offset);
      }

    return arr;
  }


  namespace convert
  {

    namespace over_load
    {

      template <typename W, typename I>
      void
      from_to_(const Window<W>& win_, Image<I>& ima_)
      {
	mln_is_simple_window(W)::check();
	typedef mln_psite(I) P;
	mlc_converts_to(mln_dpsite(W), mln_delta(P))::check();
	mlc_equal(mln_value(I), bool)::check();

	const W& win = exact(win_);
	I& ima = exact(ima_);

	mln_precondition(win.is_valid());
	mln_precondition(! ima.is_valid());

	// Hack (below) to avoid circular dependency.
	ima.init_(mln::internal::geom_bbox(win));
	{
	  // data::fill(ima, false) is:
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = false;
	}
	unsigned n = win.size();
	for (unsigned i = 0; i < n; ++i)
	  ima(convert::to<P>(win.dp(i))) = true;
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WINDOW_HH
