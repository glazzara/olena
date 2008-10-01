// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CONCEPT_WINDOW_HH
# define MLN_CORE_CONCEPT_WINDOW_HH

/*! \file mln/core/concept/window.hh
 * \brief Definition of the concept of mln::Window.
 *
 * \todo Operator== should test if the cmp is possible.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>
# include <mln/trait/windows.hh>
# include <mln/core/site_set/p_array.hh>



# define mln_is_simple_window(W)							\
											\
mln::metal::and_< mlc_is(mln_trait_window_size(W),					\
			 mln::trait::window::size::fixed),				\
		  mln::metal::and_< mlc_is(mln_trait_window_support(W),			\
					   mln::trait::window::support::regular),	\
				    mlc_is(mln_trait_window_definition(W),		\
					   mln::trait::window::definition::unique) > >



namespace mln
{

  // Fwd decl.
  template <typename E> struct Window;

  // Window category flag type.
  template <>
  struct Window<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes that are windows.
   *
   * \see mln::doc::Window for a complete documentation of this class
   * contents.
   */
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



  template <typename W>
  void check_simple(const Window<W>& win);



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
	m = 0;
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
	bool (E::*m1)() const = &E::is_centered;
	m1 = 0;
	bool (E::*m2)() const = &E::is_symmetric;
	m2 = 0;
	void (E::*m3)() = &E::sym;
	m3 = 0;
	unsigned (E::*m4)() const = &E::delta;
	m4 = 0;
      }
      static void run(mln::trait::window::definition::unique)
      {
	typedef mln_dpsite(E) D;
	const D& (E::*m)(unsigned) const = &E::dp;
	m = 0;
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
	m1 = 0;
	const W& (E::*m2)(unsigned) const = &E::window;
	m2 = 0;
	unsigned (E::*m3)() const = &E::nwindows;
	m3 = 0;
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
	  win.window(w).print(ostr);
	}
      ostr << " ]";
    }
    
  } // end of namespace mln

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

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WINDOW_HH
