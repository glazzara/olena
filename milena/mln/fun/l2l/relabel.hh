// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_L2L_RELABEL_HH
# define MLN_FUN_L2L_RELABEL_HH

/// \file mln/fun/l2l/relabel.hh
///
/// Function mapping a label to a new one.
/// \sa mln::labeling::relabel

# include <vector>
# include <algorithm>
# include <mln/fun/internal/array_base.hh>
# include <mln/util/array.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  /// Forward declaration.
  namespace fun
  {

    namespace l2l
    {

      template <typename L>
      class relabel;

    } // end of namespace mln::fun::l2l

  } // end of namespace mln::fun



  namespace convert
  {

    namespace over_load
    {

      template <typename L>
      inline
      void
      from_to_(const util::array<L>& from,
	       fun::l2l::relabel<L>& to);

      template <typename L>
      inline
      void
      from_to_(const std::vector<L>& from,
	       fun::l2l::relabel<L>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  namespace fun
  {

    namespace l2l
    {

      template <typename L>
      class relabel : public internal::array_base< L, relabel<L> >
      {
	typedef internal::array_base< L, relabel<L> > super_base_;

      public:

	/// Constructors
	/// \{

	/// Default.
	relabel();
	/// Constructs a function with \p nvalues.
	relabel(unsigned n);
	/// Constructs a function with \p nvalues and \p label as default label.
	relabel(unsigned n, const L& label);

	/// Used in from_to(). Constructs that function from an util::array.
	/// Always prefer using from_to instead of this constructor.
	relabel(const util::array<L>& from);
	/// Used in from_to(). Constructs that function from an std::vector.
	/// Always prefer using from_to instead of this constructor.
	relabel(const std::vector<L>& from);
	/// \}

      };

    } // end of namespace mln::fun::l2l

  } // end of namespace mln::fun



# ifndef MLN_INCLUDE_ONLY

  // Init.

  template <typename T1, typename T2>
  void init_(tag::function_t,
	     fun::l2l::relabel<T1>&	  f,
	     const fun::l2l::relabel<T2>& model)
  {
    f.init_(model.size());
  }


  // convert::from_to

  namespace convert
  {

    namespace over_load
    {

      template <typename L>
      inline
      void
      from_to_(const util::array<L>& from, fun::l2l::relabel<L>& to)
      {
        to = fun::l2l::relabel<L>(from);
      }

      template <typename L>
      inline
      void
      from_to_(const std::vector<L>& from, fun::l2l::relabel<L>& to)
      {
        to = fun::l2l::relabel<L>(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  /// fun::l2l::relabel

  namespace fun
  {

    namespace l2l
    {

      template <typename L>
      inline
      relabel<L>::relabel()
      {
	// FIXME: Too restrictive?
	mlc_converts_to(L, unsigned)::check();
      }

      template <typename L>
      inline
      relabel<L>::relabel(unsigned n)
	: super_base_(n)
      {
	// FIXME: Too restrictive?
	mlc_converts_to(L, unsigned)::check();
      }

      template <typename L>
      inline
      relabel<L>::relabel(unsigned n, const L& label)
	: super_base_(n, label)
      {
	// FIXME: Too restrictive?
	mlc_converts_to(L, unsigned)::check();
      }

      template <typename L>
      inline
      relabel<L>::relabel(const util::array<L>& from)
	: super_base_(from)
      {
	// FIXME: Too restrictive?
	mlc_converts_to(L, unsigned)::check();
      }

      template <typename L>
      inline
      relabel<L>::relabel(const std::vector<L>& from)
	: super_base_(from)
      {
	// FIXME: Too restrictive?
	mlc_converts_to(L, unsigned)::check();
      }

    } // end of namespace mln::fun::l2l

  } // end of namespace mln::fun

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_L2L_RELABEL_HH
