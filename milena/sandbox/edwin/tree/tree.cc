#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/data/sort_psites.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/morpho/attribute/card.hh>
#include <mln/morpho/attribute/sharpness.hh>

// to check:
#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/count.hh>
#include <mln/debug/println.hh>
#include <../../theo/color/change_attributes.hh>
//-----------------


#include <string>

#include "propagate.hh"
#include "routines.hh"


namespace mln
{
  /// Structure to simplify filtering using min tree.
  template <typename I>
  struct treefilter : Object< treefilter<I> >
  {
    typedef p_array< mln_site(I) > S;
    typedef morpho::tree::data<I,S> tree_t;

    /// Constructor: Make the min tree based on the image \p f_, then
    /// performs calculus using \p a_ attribute. To finish, it filters
    /// tree's nodes which values are in [lambda1, lambda2] set.
    template <typename A>
    treefilter(Image<I>& f_,
	       Accumulator<A> a_,
	       double lambda1 = mln_min(double),
	       double lambda2 = mln_max(double));

    /// Get the min tree performed.
    tree_t& tree() { return tree_; };

    /// Get the boolean image got after filtering.
    mln_ch_value(I, bool)& img() {return img_; };

  private:
    S sorted_sites_;
    tree_t tree_;
    mln_ch_value(I, bool) img_;
  };


  template <typename I>
  template <typename A>
  inline
  treefilter<I>::treefilter(Image<I>& f_,
			    Accumulator<A> a_,
			    double lambda1,
			    double lambda2)
    : sorted_sites_(data::sort_psites_decreasing(exact(f_))),
      tree_(exact(f_), sorted_sites_, c4())
  {
    mln_VAR(a, morpho::tree::compute_attribute_image(a_, tree_));

    //display_tree_attributes(tree_, a);
    //find_treshold(a, tree_);

    mln_fwd_piter(tree_t::nodes_t) n(tree_.nodes());
    for_all(n)
      assert(tree_.is_a_leaf(tree_.parent(n)) == false);

    img_ = duplicate((pw::cst(lambda1) < pw::value(a) &&
		      pw::value(a) < pw::cst(lambda2))
		     | a.domain());

    //debug::println("attribut", a);
  }

  template <typename I, typename T>
  inline
  float
  find_treshold(const Image<I>& img_, const T& tree)
  {
    typedef p_set< mln_psite(I) > PS;
    typedef p_array<mln_psite(I)> N;

    I img = exact(img_);
    PS pset, ps_rm; // component container.
    mln_value(I) val, old;
    std::vector< mln_value(I)> f_domain;
    std::vector< unsigned> f;


    //debug::println(img | tree.nodes());


    N nodes = data::sort_psites_increasing(img);
    mln_fwd_piter(N) n(nodes);
    mln_fwd_piter(PS) p(pset), p_rm(ps_rm);
    old = 0;

    // FIXME: hack because we want iterate on nodes and we would
    // have wanted to write:
    //     N nodes = data::sort_psites_increasing(img | tree.nodes)
    //
    // but it doesn't work, so we iterate on domain regarding if n is
    // a node.

    n.start();
    while (n.is_valid() && !tree.is_a_node(n))
      n.next();

    while (n.is_valid() && tree.is_a_node(n))
      {
	val = img(n);
	do {
	  // Remove p's children from set.
	  // FIXME: improve deletion pass.
	  ps_rm.clear();
	  for_all(p)
	    if (tree.parent(p) == n)
	      ps_rm.insert(p);

	  for_all(p_rm)
	    pset.remove(p_rm);

	  // Add the new representant to component set.
	  pset.insert(n);
	  do {
	    n.next();
	  } while (n.is_valid() && !tree.is_a_node(n));
	} while (img(n) == val && n.is_valid());

	if (pset.nsites() != old)
	  {
	    old = pset.nsites();
	    f_domain.push_back(val);
	    f.push_back(pset.nsites());
	  }


// Debug.
//  {
// 	    std::cout << " - " << val << ":" << pset.nsites() << " {";
// 	    for_all(p)
// 	      std::cout << p << ",";
// 	    std::cout << "}" << std::endl;
// 	  }
      }

    for (unsigned i = 0; i < f_domain.size() - 1; i++)
      {
	std::cout << "[" << f_domain[i] << ","
		  << f_domain[i + 1] << "[ -> "
		  << f[i] << std::endl;
      }
    std::cout <<  ">=" << f_domain[f_domain.size() - 1] << " -> "
	      << f[f_domain.size() - 1] << std::endl;

    return 0;
  }



  template <typename I, typename A>
  void filtercheck(treefilter<I>& f, const  Meta_Accumulator<A>& a)
  {

    using value::label_8;
    label_8 n;
    util::array<unsigned int> counts;
    util::array< mln_psite(I) > fnodes;

    mln_VAR(lbl, labeling::blobs(f.img(), c4(), n));
    counts = labeling::compute(a, lbl, n);

    mln_VAR(acc, morpho::tree::compute_attribute_image(morpho::attribute::card<I>(), f.tree()));
    fnodes = morpho::tree::get_first_nodes(f.img(), f.tree());

    mln_assertion(counts.nelements() == fnodes.nelements() + 1);

    std::vector<unsigned> counts2;
    counts2.resize(fnodes.nelements());
    std::transform(fnodes.hook_std_vector_().begin(), fnodes.hook_std_vector_().end(),
		   counts2.begin(), acc);

    std::sort(counts.hook_std_vector_().begin(), counts.hook_std_vector_().end());
    std::sort(counts2.begin(), counts2.end());

    for (unsigned i = 0; i < counts2.size(); i++)
      {
	mln_assertion(acc.domain().has(fnodes(p)));
	std::cout << "counts[" << i << "]: (ref " << counts[i] << ") " << counts2[i]
		  << std::endl;
      }


  }
} // end of namespace mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm accumulator propagation lambda1 [lambda2]"
	    << "\taccu:\tcard | sharpness" << std::endl
	    << "\tpropa:\tto_children | to_childhood | to_parent | to_ancestors | leaf_to_ancestors" << std::endl;
  abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  float lambda1;
  float lambda2;


  if (argc < 4)
    usage(argv);

  I input;
  io::pgm::load(input, argv[1]);
//   int_u8 vals[] = { 0, 1, 0, 2, 0, 1, 0,
// 		    1, 1, 1, 2, 1, 1, 1,
// 		    0, 1, 0, 2, 0, 1, 0,
// 		    2, 2, 2, 2, 2, 2, 2,
// 		    1, 1, 1, 2, 0, 1, 0,
// 		    1, 1, 1, 2, 1, 1, 1,
// 		    1, 1, 1, 2, 0, 1, 0};
//    I input = make::image2d(vals);



  lambda1 = atof(argv[4]);
  lambda2 = (argc == 6) ? atof(argv[5]) : mln_max(float);

  std::string s(argv[2]);
  std::string propa(argv[3]);
  treefilter<I>* f = 0;
  if (s == "card")
    f = new treefilter<I>(input, morpho::attribute::card<I>(), lambda1, lambda2);
  else if (s == "sharpness")
    f = new treefilter<I>(input, morpho::attribute::sharpness<I>(), lambda1, lambda2);
  else
    usage(argv);


  std::cout << "s1" << std::endl;
  if (propa == "to_childhood")
    morpho::tree::propagate_to_childhood(f->tree(), f->img());
  else if (propa == "to_children")
    morpho::tree::propagate_to_children(f->tree(), f->img());
  else if (propa == "to_ancestors")
    morpho::tree::propagate_to_ancestors(f->tree(), f->img());
  else if (propa == "to_parent")
    morpho::tree::propagate_to_parent(f->tree(), f->img());
  else if (propa == "leaf_to_ancestors")
    morpho::tree::propagate_leaf_to_ancestors(f->tree(), f->img());
  else
    usage(argv);

  std::cout << "s2" << std::endl;
  morpho::tree::propagate_to_node(f->tree(), f->img());
  std::cout << "s3" << std::endl;



  //filtercheck(*f, accu::meta::count());


  io::pbm::save(f->img(), "out.pbm");
  delete f;
}
