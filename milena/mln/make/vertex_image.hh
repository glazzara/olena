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

#ifndef MLN_MAKE_VERTEX_IMAGE_HH
# define MLN_MAKE_VERTEX_IMAGE_HH

/// \file
///
/// Routine to create a vertex image.

# include <mln/core/image/vertex_image.hh>
# include <mln/core/concept/function.hh>
# include <mln/util/internal/id2element.hh>


namespace mln
{

  namespace make
  {


    /// Construct a vertex image
    ///
    /// \param[in] g A graph.
    /// \param[in] fv A function mapping vertex ids to values.
    ///
    /// \return A vertex image.
    //
    template <typename G, typename FV>
    mln::vertex_image<void,mln_result(FV),G>
    vertex_image(const Graph<G>& g, const Function_v2v<FV>& fv);


    /// Construct a vertex image
    ///
    /// \param[in] g_ A graph.
    /// \param[in] fp A function mapping vertex ids to sites.
    /// \param[in] fv A function mapping vertex ids to values.
    ///
    /// \return A vertex image.
    //
    template <typename FP, typename FV, typename G>
    mln::vertex_image<mln_result(FP),mln_result(FV),G>
    vertex_image(const Graph<G>& g_,
		 const Function_v2v<FP>& fp,
		 const Function_v2v<FV>& fv);



# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename FV>
    mln::vertex_image<void,mln_result(FV),G>
    vertex_image(const Graph<G>& g, const Function_v2v<FV>& fv)
    {
      mln_trace("make::vertex_image");
      mln_precondition(exact(g).is_valid());

      p_vertices<G> pv(g);
      mln::vertex_image<void,mln_result(FV),G> ima(pv, fv);

      return ima;
    }


    template <typename FP, typename FV, typename G>
    mln::vertex_image<mln_result(FP),mln_result(FV),G>
    vertex_image(const Graph<G>& g_,
		 const Function_v2v<FP>& fp,
		 const Function_v2v<FV>& fv)
    {
      mln_trace("make::vertex_image");
      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      p_vertices<G,FP> pv(g,fp);
      mln::vertex_image<mln_result(FP),mln_result(FV),G> ima(pv, fv);

      return ima;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_VERTEX_IMAGE_HH
