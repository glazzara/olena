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

#ifndef MLN_MAKE_EDGE_IMAGE_HH
# define MLN_MAKE_EDGE_IMAGE_HH

/// \file
///
/// Routine to create a edge image.

# include <mln/core/image/edge_image.hh>
# include <mln/core/image/vertex_image.hh>
# include <mln/core/concept/function.hh>
# include <mln/util/internal/id2element.hh>
# include <mln/data/fill.hh>


namespace mln
{

  namespace make
  {


    /// Construct an edge image.
    ///
    /// \param[in] g  A graph.
    /// \param[in] fv A function mapping edge ids to values.
    ///
    /// \return an edge image.
    //
    template <typename V, typename G>
    mln::edge_image<void,V,G>
    edge_image(const Graph<G>& g, const fun::i2v::array<V>& fv);

    /// Construct an edge image.
    ///
    /// \param[in] g  A graph.
    /// \param[in] fv A function mapping edge ids to values.
    ///
    /// \return an edge image.
    //
    template <typename FV, typename G>
    mln::edge_image<void, mln_result(FV), G>
    edge_image(const Graph<G>& g, const Function_v2v<FV>& fv);


    /// Construct an edge image.
    ///
    /// \param[in] g_ A graph.
    /// \param[in] fp A function mapping edge ids to sites.
    /// \param[in] fv A function mapping edge ids to values.
    ///
    /// \return an edge image.
    //
    template <typename FP, typename FV, typename G>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const Graph<G>& g_,
	       const Function_v2v<FP>& fp,
	       const Function_v2v<FV>& fv);



    /// Construct an edge image.
    ///
    /// \param[in] v_ima_ A vertex image.
    /// \param[in] pe	  A p_edges mapping graph elements to sites.
    /// \param[in] fv_	  A function mapping two vertex ids to a value.
    ///			  The result is associated to the corresponding edge.
    ///
    /// \return an edge image.
    //
    template <typename P, typename V, typename G, typename FP, typename FV>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const p_edges<G,FP> pe,
	       const Function_vv2v<FV>& fv_);



    /// Construct an edge image.
    ///
    /// \param[in] v_ima_ A vertex image.
    /// \param[in] fv_	  A function mapping two vertices' values to a value.
    ///			  The result is associated to the corresponding edge.
    ///
    /// \return an edge image without localization information mapped to
    /// graph elements.
    //
    template <typename P, typename V, typename G, typename FV>
    mln::edge_image<void,mln_result(FV),G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const Function_vv2v<FV>& fv_);


    /// Construct an edge image.
    ///
    /// \param[in] v_ima_ A vertex image.
    /// \param[in] fv_	  A predicate on a vertex's value.
    ///			  The (Boolean) result is associated to the
    ///			  edges adjacent to the vertex.
    ///
    /// \return an edge image without localization information mapped to
    /// graph elements.
    //
    template <typename P, typename V, typename G, typename F>
    mln::edge_image<void,bool,G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const Function_v2b<F>& fv_);


# ifndef MLN_INCLUDE_ONLY



    template <typename V, typename G>
    inline
    mln::edge_image<void,V,G>
    edge_image(const Graph<G>& g, const fun::i2v::array<V>& fv)
    {
      mln_trace("make::edge_image");
      mln_precondition(exact(g).is_valid());

      p_edges<G> pe(g);
      mln::edge_image<void,V,G> ima(pe, fv);

      return ima;
    }

    template <typename FV, typename G>
    mln::edge_image<void,mln_result(FV),G>
    edge_image(const Graph<G>& g_,
	       const Function_v2v<FV>& fv)
    {
      mln_trace("make::edge_image");
      const G& g = exact(g_);
      const FV& f = exact(fv);
      mln_precondition(g.is_valid());

      p_edges<G> pe(g);
      typedef mln::edge_image<void,mln_result(FV),G> ima_t;
      ima_t ima(pe);

      mln_piter(ima_t) e(ima.domain());
      for_all(e)
	ima(e) = f(e.id());

      return ima;
    }


    template <typename FP, typename FV, typename G>
    inline
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const Graph<G>& g_,
	       const Function_v2v<FP>& fp,
	       const Function_v2v<FV>& fv)
    {
      mln_trace("make::edge_image");
      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      p_edges<G,FP> pe(g,fp);
      mln::edge_image<mln_result(FP),mln_result(FV),G> ima(pe, fv);

      return ima;
    }



    template <typename P, typename V, typename G, typename FP, typename FV>
    inline
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const p_edges<G,FP> pe,
	       const Function_vv2v<FV>& fv_)
    {
      mln_trace("make::edge_image");

      const FV& fv = exact(fv_);
      const mln::vertex_image<P,V,G>& v_ima = exact(v_ima_);
      mln_precondition(v_ima.is_valid());

      typedef mln::edge_image<mln_result(FP),mln_result(FV),G> edge_ima_t;
      edge_ima_t ima_e(pe);

      mln_piter(edge_ima_t) e(ima_e.domain());
      for_all(e)
	ima_e(e) = fv(e.element().v1(), e.element().v2());

      return ima_e;
    }



    template <typename P, typename V, typename G, typename FV>
    inline
    mln::edge_image<void,mln_result(FV),G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const Function_vv2v<FV>& fv_)
    {
      mln_trace("make::edge_image");

      const FV& fv = exact(fv_);
      const mln::vertex_image<P,V,G>& v_ima = exact(v_ima_);
      mln_precondition(v_ima.is_valid());

      p_edges<G> pe(v_ima.domain().graph());
      typedef mln::edge_image<void,mln_result(FV),G> edge_ima_t;
      edge_ima_t ima_e(pe);

      mln_piter(edge_ima_t) e(ima_e.domain());
      for_all(e)
	ima_e(e) = fv(v_ima(e.element().v1()), v_ima(e.element().v2()));

      return ima_e;
    }


    template <typename P, typename V, typename G, typename F>
    inline
    mln::edge_image<void,bool,G>
    edge_image(const mln::vertex_image<P,V,G>& v_ima_,
	       const Function_v2b<F>& fv_)
    {
      mln_trace("make::edge_image");

      const F& fv = exact(fv_);
      typedef mln::vertex_image<P,V,G> v_ima_t;
      const v_ima_t& v_ima = exact(v_ima_);
      mln_precondition(v_ima.is_valid());

      p_edges<G> pe(v_ima.domain().graph());
      typedef mln::edge_image<void,bool,G> edge_ima_t;
      edge_ima_t ima_e(pe);
      data::fill(ima_e, true);

      mln_piter(v_ima_t) p(v_ima.domain());
      for_all(p)
	if (!fv(v_ima(p)))
	{
	  typename v_ima_t::edge_win_t win;
	  mln_qiter(v_ima_t::edge_win_t) q(win, p);
	  for_all(q)
	    ima_e(q) = false;
	}

      return ima_e;
    }



# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_EDGE_IMAGE_HH
