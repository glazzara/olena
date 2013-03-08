// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_EDGE_IMAGE_HH
# define MLN_CORE_IMAGE_EDGE_IMAGE_HH

/// \file
///
/// \brief Image based on graph edges.

# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_edges.hh>
# include <mln/core/image/graph_elt_window.hh>
# include <mln/core/image/graph_elt_neighborhood.hh>
# include <mln/pw/internal/image_base.hh>
# include <mln/fun/i2v/array.hh>



namespace mln
{
  // Forward declaration.
  template <typename P, typename V, typename G> class edge_image;
  namespace util { class graph; }


  // Properties

  namespace trait
  {

    template <typename P, typename V, typename G>
    struct image_< edge_image<P,V,G> >
      : pw_image_<fun::i2v::array<V>,
		  p_edges<util::graph, fun::i2v::array<P> >,
		  edge_image<P,V,G> >
    {
    };

  } // end of namespace mln::traits.



  namespace internal
  {

    /*! \internal
      \brief Data structure for mln::pw::internal::image
    */
    template <typename P, typename V, typename G>
    struct data< mln::edge_image<P,V,G> >
    {
      typedef typename edge_image<P,V,G>::site_function_t site_function_t;

      data(const fun::i2v::array<V>& edge_values,
	   const p_edges<G,site_function_t>& pe);

      fun::i2v::array<V> f_;
      p_edges<G,site_function_t> domain_;
    };

  } // end of namespace mln::internal




  /// Construct a edge image from a fun::i2v::array and a p_edges.
  /// image = fun::i2v::array | p_edges.
  template <typename V, typename G, typename P>
  edge_image<P,V,G>
  operator | (const fun::i2v::array<V>& edge_values,
	      const p_edges<G,fun::i2v::array<P> >& pe);


  // Vertex_image_fsite_selector

  namespace internal
  {

    template <typename P, typename G>
    struct efsite_selector
    {
      typedef fun::i2v::array<P> site_function_t;
    };



    template <typename G>
    struct efsite_selector<void,G>
    {
      typedef util::internal::id2element< G,util::edge<G> > site_function_t;
    };


  } // end of namespace mln::internal


  /// \brief Image based on graph edges.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename P, typename V, typename G = util::graph>
  class edge_image
    : public pw::internal::image_base<fun::i2v::array<V>,
				      p_edges<G, typename internal::efsite_selector<P,G>::site_function_t >,
				      edge_image<P,V,G> >
  {
    typedef pw::internal::image_base<fun::i2v::array<V>,
				     p_edges<G, typename internal::efsite_selector<P,G>::site_function_t >,
				     edge_image<P,V,G> > super_;

  public:
    typedef typename super_::psite  psite;
    typedef typename super_::rvalue rvalue;
    typedef typename super_::lvalue lvalue;

    /// The type of the underlying graph.
    typedef G graph_t;

    /// Skeleton type.
    typedef edge_image< tag::psite_<P>,
			tag::value_<V>,
			tag::graph_<G> > skeleton;

    /// Function mapping graph elements to sites.
    typedef typename internal::efsite_selector<P,G>::site_function_t
	    site_function_t;
    typedef mln_result(site_function_t) function_result_t;

    /// Edge Window type
    typedef graph_elt_window<G,p_edges<G,site_function_t> > edge_win_t;
    /// Neighborhood type.
    typedef graph_elt_neighborhood<G,p_edges<G,site_function_t> > edge_nbh_t;

    /// Default Window type
    typedef edge_win_t win_t;

    /// Default Neighborhood type
    typedef edge_nbh_t nbh_t;


    /// Constructors.
    /// \{
    edge_image();
    edge_image(const p_edges<G, site_function_t>& pe);
    edge_image(const Graph<G>& g,
	       const Function_v2v< site_function_t >& edge_sites,
	       const Function_v2v< fun::i2v::array<V> >& edge_values);

    edge_image(const p_edges<G, site_function_t >& pe,
	       const Function_v2v< fun::i2v::array<V> >& edge_values);

    template <typename FP, typename FV>
    edge_image(const Graph<G>& g,
	       const Function_v2v<FP>& edge_sites,
	       const Function_v2v<FV>& edge_values);

    template <typename FV>
    edge_image(const p_edges<G,site_function_t>& pe,
	       const Function_v2v<FV>& edge_values);
    /// \}

    /// Value accessors/operators overloads.
    /// \{
    rvalue operator()(unsigned e_id) const;
    lvalue operator()(unsigned e_id);
    /// \}

    // Just to help g++-2.95...
    rvalue operator()(const psite& p) const;
    lvalue operator()(const psite& p);
    // ...because "using super_::operator()" does not properly work.

  };

  template <typename P, typename V, typename G, typename J>
  void init_(tag::image_t, edge_image<P,V,G>& target, const Image<J>& model);


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename V, typename G, typename J>
  void init_(tag::image_t, edge_image<P,V,G>& target, const Image<J>& model)
  {
    fun::i2v::array<V> f;
    init_(tag::function, f, exact(model));
    p_edges<G, typename edge_image<P,V,G>::site_function_t> s;
    init_(tag::domain, s, exact(model));
    target.init_(f, s);
  }

  // Operator.

  template <typename V, typename G, typename P>
  inline
  edge_image<P,V,G>
  operator | (const fun::i2v::array<V>& edge_values,
	      const p_edges<G,fun::i2v::array<P> >& pe)
  {
    edge_image<P,V,G> tmp(edge_values, pe);
    return tmp;
  }



  // data< pw::image >

  namespace internal
  {

    template <typename P, typename V, typename G>
    inline
    data< mln::edge_image<P,V,G> >::data(const fun::i2v::array<V>& f,
					 const p_edges<G,site_function_t>& ps)
      : f_(exact(f)),
	domain_(ps)
    {
    }

  } // end of namespace mln::internal



  // edge_image<P,V,G>

  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image()
  {
  }

  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image(const p_edges<G,site_function_t>& pe)
    : super_(fun::i2v::array<V>(pe.nsites()), pe)
  {
  }

  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image(const Graph<G>& g,
				const Function_v2v< site_function_t >& edge_sites,
				const Function_v2v< fun::i2v::array<V> >& edge_values)
    : super_(exact(edge_values),
	     p_edges<G,site_function_t>(g, exact(edge_sites)))
  {
  }

  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image(const p_edges<G,site_function_t>& pe,
				const Function_v2v< fun::i2v::array<V> >& edge_values)
    : super_(exact(edge_values), pe)
  {
  }

  template <typename P, typename V, typename G>
  template <typename FP, typename FV>
  inline
  edge_image<P,V,G>::edge_image(const Graph<G>& g,
				const Function_v2v<FP>& edge_sites,
				const Function_v2v<FV>& edge_values)
    : super_(convert::to<fun::i2v::array<V> >(exact(edge_values)),
	     p_edges<G,site_function_t>(g, exact(edge_sites)))
  {
    mlc_equal(mln_result(FP),P)::check();
    mlc_equal(mln_result(FV),V)::check();
  }

  template <typename P, typename V, typename G>
  template <typename FV>
  inline
  edge_image<P,V,G>::edge_image(const p_edges<G,site_function_t>& pe,
				const Function_v2v<FV>& edge_values)
    : super_(convert::to<fun::i2v::array<V> >(exact(edge_values)), pe)
  {
    mlc_equal(mln_result(FV),V)::check();
  }

  template <typename P, typename V, typename G>
  typename edge_image<P,V,G>::rvalue
  edge_image<P,V,G>::operator()(unsigned e_id) const
  {
    return this->data_->f_(e_id);
  }

  template <typename P, typename V, typename G>
  typename edge_image<P,V,G>::lvalue
  edge_image<P,V,G>::operator()(unsigned e_id)
  {
    return this->data_->f_(e_id);
  }

  template <typename P, typename V, typename G>
  typename edge_image<P,V,G>::rvalue
  edge_image<P,V,G>::operator()(const typename edge_image<P,V,G>::psite& p) const
  {
    return this->super_::operator()(p);
  }

  template <typename P, typename V, typename G>
  typename edge_image<P,V,G>::lvalue
  edge_image<P,V,G>::operator()(const typename edge_image<P,V,G>::psite& p)
  {
    return this->super_::operator()(p);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_EDGE_IMAGE_HH
