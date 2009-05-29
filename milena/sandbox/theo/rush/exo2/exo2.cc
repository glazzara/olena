#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
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

#include <string>

#include "propagate.hh"


namespace mln
{
  /// Structure to simplify filtering using min tree.
  template <typename I>
  struct treefilter
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

    img_ = duplicate((pw::cst(lambda1) < pw::value(a) &&
		      pw::value(a) < pw::cst(lambda2)) | a.domain());
  }

} // end of namespace mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm accumulator lambda1 [lambda2]"
	    << std::endl;
  abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  float lambda1;
  float lambda2;
  I input;

  if (argc < 4)
    usage(argv);

  io::pgm::load(input, argv[1]);

  lambda1 = atof(argv[3]);
  lambda2 = (argc == 5) ? atof(argv[4]) : mln_max(float);

  std::string s(argv[2]);
  treefilter<I>* f = 0;
  if (s == "card")
    f = new treefilter<I>(input, morpho::attribute::card<I>(), lambda1, lambda2);
  else if (s == "sharpness")
    f = new treefilter<I>(input, morpho::attribute::sharpness<I>(), lambda1, lambda2);
  else
    usage(argv);

  back_propagate_subbranch(f->tree(), f->img() ,true);
  back_propagate_level(f->tree(), f->img());

  io::pbm::save(f->img(), "out.pbm");
  delete f;
}
