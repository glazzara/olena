// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_PSEUDO_SITE_HH
# define MLN_CORE_CONCEPT_PSEUDO_SITE_HH

/// \file
///
/// \brief Definition of the concept of mln::Pseudo_Site.
///
/// \todo Inconsistency: "if_possible::change_target" expects
/// "::target_t" from a pseudo-site, whereas the concept checks for
/// "::target".

# include <mln/core/concept/site_proxy.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  // Fwd decl.
  template <typename E> struct Pseudo_Site;



  /// Pseudo_Site category flag type.
  template <>
  struct Pseudo_Site<void>
  {
    typedef Site_Proxy<void> super;
  };


  /*! \brief Base class for implementation classes of the notion of
   *  "pseudo site".
   *
   * FIXME: Explain...
   */
  template <typename E>
  struct Pseudo_Site : public Site_Proxy<E>
  {
    typedef Pseudo_Site<void> category;

    // typedef target;

    // void change_target(const target& new_target);
    // const target* target_() const; // Hook to the target.

    bool has_target() const;

  protected:
    Pseudo_Site();
  };


  namespace if_possible
  {
    // Nota: This procedure is used in
    // internal::site_set_iterator_base and
    // site_relative_iterator_base.

    template <typename O, typename T>
    void change_target(Object<O>&, const T&);

  } // end of namespace mln::if_possible


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Pseudo_Site<E>::Pseudo_Site()
  {
    typedef mln_target(E) target;

    void (E::*m1)(const target&) = & E::change_target;
    (void) m1;
    const target* (E::*m2)() const = & E::target_;
    (void) m2;
  }

  template <typename E>
  inline
  bool
  Pseudo_Site<E>::has_target() const
  {
    return exact(this)->target_() != 0;
  }



  namespace if_possible
  {

    namespace internal
    {

      template <bool b> struct helper;

      template <>
      struct helper< /* is an Object */ true >
      {
 
	template <typename P>
	void change_target(Pseudo_Site<P>& p,
			   const mln_target(P)& new_target) const
	{
	  exact(p).change_target(new_target);
	}

	template <typename O, typename D>
	void change_target(Object<O>&, const D&) const
	{
	  // No-op.
	}
	
      };

      template <>
      struct helper< /* NOT an Object */ false >
      {
	template <typename O, typename D>
	void change_target(O&, const D&) const
	{
	  // No-op.
	}
      };
      
    } // namespace mln::if_possible::internal


    template <typename O, typename D>
    void change_target(O& o, const D& d)
    {
      enum { is_object = mlc_is_a(O, Object)::value };
      mln::if_possible::internal::helper< is_object >().change_target(exact(o),
								      d);
    }

  } // end of namespace mln::if_possible

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PSEUDO_SITE_HH
