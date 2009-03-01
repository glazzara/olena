#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/level/sort_psites.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/morpho/attribute/card.hh>
#include <mln/morpho/attribute/sharpness.hh>

#include "propagate.hh"


namespace mln
{
  template <typename I>
  struct treefilter
  {
    typedef p_array< mln_site(I) > S;
    typedef morpho::tree::data<I,S> tree_t;

    template <typename A>
    treefilter(Image<I>& f_,
	       Accumulator<A> a_,
	       double lambda1 = mln_min(double),
	       double lambda2 = mln_max(double));

    tree_t& tree() { return tree_; };
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
    : sorted_sites_(level::sort_psites_decreasing(exact(f_))),
      tree_(exact(f_), sorted_sites_, c4())
  {
    mln_VAR(a, morpho::tree::compute_attribute_image(a_, tree_));

    img_ = duplicate((pw::cst(lambda1) < pw::value(a) &&
		      pw::value(a) < pw::cst(lambda2)) | a.domain());
  }

} // end of namespace mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda1 lamda2"
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

  if (argc < 3)
    usage(argv);

  io::pgm::load(input, argv[1]);
  lambda1 = atof(argv[2]);
  lambda2 = (argc == 4) ? atof(argv[3]) : mln_max(float);

  treefilter<I> f(input, morpho::attribute::sharpness<I>(), lambda1, lambda2);

  back_propagate_subbranch(f.tree(), f.img() ,true);
  back_propagate_level(f.tree(), f.img());

  io::pbm::save(f.img(), "out.pbm");
}
