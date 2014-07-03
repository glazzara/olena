// Copyright (C) 2009, 2014 EPITA Research and Development Laboratory (LRDE).
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/edge_image.hh>
#include <mln/core/image/vertex_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/value/label_16.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/colors.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/compute.hh>
#include <mln/make/p_vertices_with_mass_centers.hh>
#include <mln/make/edge_image.hh>
#include <mln/make/vertex_image.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/util/graph.hh>
#include <mln/accu/center.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_rag.hh>
//#include <scribo/primitive/group/from_rag.hh>

#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>


#include <scribo/fun/v2b/objects_small_filter.hh>
#include <scribo/debug/bboxes_image.hh>
#include <scribo/debug/linked_bboxes_image.hh>
#include <scribo/make/debug_filename.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}



namespace scribo
{

  using namespace mln;


  namespace graph
  {

    template <typename A, typename G, typename I>
    inline
    vertex_image<void,mln_result(A),G>
    compute_vertex(const Accumulator<A>& accu,
		   const Graph<G>& g_,
		   const Image<I>& lbl_,
		   const mln_value(I)& nlabels)
    {
      const G& g = exact(g_);
      const I& lbl = exact(lbl_);

      mln_precondition(g.is_valid());
      mln_precondition(lbl.is_valid());

      util::array<mln_result(A)>
	values = labeling::compute(accu, lbl, nlabels);

      vertex_image<void, mln_result(A), G>
	v_ima = mln::make::vertex_image(g, values);

      return v_ima;
    }

  } // end of namespace scribo::graph


  template <typename I>
  struct edge_color : mln::Function_v2b< edge_color<I> >
  {
      typedef value::rgb8 result;

      edge_color(const Image<I>& mask) : mask_(exact(mask)) {}

      value::rgb8 operator()(const unsigned id) const
      {
	if (mask_(id))
	  return literal::green;
	return literal::red;
      }

      I mask_;
  };


  /// Compute a distance between two rgb8 values.
  struct dist : Function_vv2v< dist >
  {

    typedef bool result;

    bool operator()(const point2d& p1, const point2d& p2) const
    {
      return (math::sqrt(math::sqr(static_cast<float>(p1.row() - p2.row()))
			 + math::sqr(static_cast<float>(p1.col() - p2.col())))) < 5;
    }

  };


  namespace filter
  {

    template <typename P, typename V, typename G, typename F, typename FP>
    edge_image<void,bool,G>
    graph_edges(const vertex_image<P,V,G>& v_ima,
		const Function<F>& /* edge_values */,
		const Function<FP>& /* predicate */)
    {
      typedef edge_image<void,bool,util::graph> e_filter_t;
      e_filter_t e_filter = mln::make::edge_image(v_ima, dist());


    }

  } // end of namespace scribo::filter


}



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 2)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "group_with_rag";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  typedef image2d<value::label_16> L;
  value::label_16 nbboxes;
  typedef component_set<L> components_t;
  components_t components = primitive::extract::components(input, c8(), nbboxes);

  /// First filtering.
  components_t filtered_components
    = scribo::filter::components_small(components, 6);

  filtered_components
    = scribo::filter::components_thin(filtered_components, 3);

  filtered_components
    = scribo::filter::objects_thick(filtered_components,
				    math::min(input.ncols(), input.nrows()) / 6);


  /// Getting components links from a Region Adjacency graph.
  typedef util::couple< util::graph, image2d<value::label_16> > rag_data_t;
  rag_data_t rag_data = primitive::link::with_rag(filtered_components, c8());


  typedef vertex_image<void, algebra::vec<2u, float>, util::graph> v_ima_t;
  v_ima_t v_ima =
    scribo::graph::compute_vertex(accu::center<point2d>(),
                                  rag_data.first(),
                                  filtered_components.labeled_image(),
                                  filtered_components.nelements());

  //FOR DEBUGGING PURPOSE
  {
    image2d<value::rgb8>
      before_grouping = data::convert(value::rgb8(), input);

    // FIXME: Disabled, no longer compiles.
#if 0
    scribo::draw::bounding_boxes(before_grouping,
				 filtered_components.bboxes(),
				 literal::blue);
#endif

    typedef p_vertices<util::graph, mln::fun::i2v::array<point2d> > pv_t;
    pv_t pv =
      mln::make::p_vertices_with_mass_centers(filtered_components.labeled_image(),
                                              rag_data.first());
    mln::debug::draw_graph(before_grouping, pv, literal::green, literal::green);

    io::ppm::save(before_grouping,
		  scribo::make::debug_filename("before_grouping.ppm"));
  }



  typedef edge_image<void,bool,util::graph> e_filter_t;
  e_filter_t e_filter = mln::make::edge_image(v_ima, dist());




  //FOR DEBUGGING PURPOSE
//   {
//     image2d<value::rgb8>
//       after_grouping = data::convert(value::rgb8(), input);


//     scribo::draw::bounding_boxes(after_grouping,
// 				 filtered_components.bboxes(),
// 				 literal::blue);

//     mln::debug::draw_graph(after_grouping, v_ima.domain(),
// 			   pw::cst(literal::black), edge_color<e_filter_t>(e_filter));

//     io::ppm::save(after_grouping, "after_grouping.ppm");
//   }
}


