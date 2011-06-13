// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_NO_CONSTRAINT_HH
# define MLN_TOPO_NO_CONSTRAINT_HH

/// \file
/// \brief Definition of a functor expressing a lack of constraint.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace topo
  {

    /// \brief A constraint functor always returning \c true.
    ///
    /// \see mln::fun::p2b::tautology
    struct no_constraint : public Function_v2b<no_constraint>
    {
      typedef bool result;

      /// Dummy setter (does nothing).
      template <typename I>
      void set_image(const Image<I>& ima) const;

      template <typename P>
      bool operator()(const P& p) const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void
    no_constraint::set_image(const Image<I>& /* ima */) const
    {
    }

    template <typename P>
    inline
    bool
    no_constraint::operator()(const P& /* p */) const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln


#endif // ! MLN_TOPO_NO_CONSTRAINT_HH
