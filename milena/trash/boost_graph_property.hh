// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/// \file mln/util/internal/boost_graph_property.hh
/// \brief boost_graph properties access operations.

#ifndef MLN_UTIL_INTERNAL_BOOST_GRAPH_PROPERTY_HH_
# define MLN_UTIL_INTERNAL_BOOST_GRAPH_PROPERTY_HH_

# include <utility>
# include <boost/graph/adjacency_list.hpp>

/// fwd declaration
namespace mln
{
  namespace util
  {
    namespace internal
    {
      template <typename VertexProperty, typename EdgeProperty>
      class boost_graph;
    }
  }
}

namespace boost
{
  namespace mlnu = mln::util::internal;


  /// \brief Returns the property map object for the vertex property
  ///        specified by PropertyTag.
  template <typename VProp, typename EProp, typename PropertyTag>
  typename property_map<typename mlnu::boost_graph<EProp, VProp>::decorated,
			PropertyTag>::type
  get(PropertyTag, typename mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns the property map object for the vertex property
  ///        specified by PropertyTag (const version).
  template <typename VProp, typename EProp, typename PropertyTag>
  typename property_map<typename mlnu::boost_graph<EProp, VProp>::decorated,
			PropertyTag>::const_type
  get(PropertyTag, const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief This returns the property value for x.
  ///        x is either a vertex or edge descriptor.
  template <typename VProp, typename EProp, typename PropertyTag, typename X>
  typename property_traits<
    typename property_map<typename mlnu::boost_graph<EProp, VProp>::decorated,
			  PropertyTag>::const_type >::value_type
  get(PropertyTag, const mlnu::boost_graph<VProp, EProp>& g, X x);


  /// \brief This sets the property value for x to value.
  ///        x is either a vertex or edge descriptor.
  template <typename VProp, typename EProp,
	    typename PropertyTag, typename X, typename Value>
  void
  put(PropertyTag, const mlnu::boost_graph<VProp, EProp>& g,
      X x, const Value& value);


  /// \brief Return the property specified by GraphPropertyTag.
  template <typename EProp, typename VProp, typename GraphProperties,
	    typename GraphPropertyTag>
  typename graph_property<typename mlnu::boost_graph<EProp, VProp>::decorated,
			  GraphPropertyTag>::type&
  get_property(mlnu::boost_graph<VProp, EProp>& g, GraphPropertyTag);


  /// \brief Return the property specified by GraphPropertyTag.
  template <typename VProp, typename EProp,
	    typename GraphProperties, typename GraphPropertyTag>
  const typename graph_property<
    typename mlnu::boost_graph<EProp, VProp>::decorated,
    GraphPropertyTag>::type&
    get_property(const mlnu::boost_graph<VProp, EProp>& g,
		 GraphPropertyTag);

# ifndef MLN_INCLUDE_ONLY

  template <typename VProp, typename EProp, typename PropertyTag>
  typename property_map<typename mlnu::boost_graph<EProp, VProp>::decorated,
			PropertyTag>::type
  get(PropertyTag property_tag,
      typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return get(property_tag, g.graph());
  }

  template <typename VProp, typename EProp, typename PropertyTag>
  typename property_map<typename mlnu::boost_graph<EProp, VProp>::decorated,
			PropertyTag>::const_type
  get(PropertyTag property_tag,
      const mlnu::boost_graph<VProp, EProp>& g)
  {
    return get(property_tag, g.graph());
  }

  template <typename VProp, typename EProp,
	    typename PropertyTag, typename X>
  typename property_traits<
    typename property_map<
      typename mlnu::boost_graph<EProp, VProp>::decorated,
      PropertyTag>::const_type>::value_type
  get(PropertyTag property_tag, const mlnu::boost_graph<VProp, EProp>& g, X x)
  {
    return get(property_tag, g.graph(), x);
  }

  template <typename VProp, typename EProp,
	    typename PropertyTag, typename X, typename Value>
  void
  put(PropertyTag property_tag, const mlnu::boost_graph<VProp, EProp>& g,
      X x, const Value& value)
  {
    put(property_tag, g.graph(), x, value);
  }

  template <typename EProp, typename VProp,
	    typename GraphProperties, typename GraphPropertyTag>
  typename graph_property<typename mlnu::boost_graph<EProp, VProp>::decorated,
			  GraphPropertyTag>::type&
  get_property(mlnu::boost_graph<VProp, EProp>& g,
	       GraphPropertyTag property_tag)
  {
    return get_property(g.graph(), property_tag);
  }

  template <typename VProp, typename EProp,
	    typename GraphProperties, typename GraphPropertyTag>
  const typename graph_property<
    typename mlnu::boost_graph<EProp, VProp>::decorated,
    GraphPropertyTag>::type&
    get_property(const mlnu::boost_graph<VProp, EProp>& g,
		 GraphPropertyTag property_tag)
  {
    return get_property(g.graph(), property_tag);
  }

# endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_UTIL_INTERNAL_BOOST_GRAPH_PROPERTY_HH_
