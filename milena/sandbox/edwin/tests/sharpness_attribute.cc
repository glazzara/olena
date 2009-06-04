/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>

/* Site set */
#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>

/* Attributes */
#include <mln/morpho/attribute/mysharpness.hh>
#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

/* io */
#include <mln/io/pgm/load.hh>

#include <iostream>
#include <iomanip>
#include <stack>



void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0] << " input.pgm" << std::endl;
  abort();
}

namespace mln{
  template <typename T, typename H, typename V>
  inline
  mln_ch_value(typename T::function, double)
    normal_sharp(T& tree,
		 Image<H>& h_img,
		 Image<V>& v_img)
  {
    mln_ch_value(typename T::function, double) s;
    initialize(s, tree.f());

    mln_node_piter(T) n(tree);
    for_all(n)
      s(n) = (double)(v_img(n).to_result()) / (double)(h_img(n).to_result() * v_img(n).area());
    return s;
  }
}

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
    usage(argv);


  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> T;

  S sorted_sites = data::sort_psites_decreasing(input);
  T tree(input, sorted_sites, c4());

  typedef mln_ch_value_(I, double) A;
  A mysharp, sharp;

  typedef morpho::attribute::volume<I> v_attr;
  typedef morpho::attribute::height<I> h_attr;
  typedef mln_ch_value_(I, h_attr) H;
  typedef mln_ch_value_(I, v_attr) V;
  H h_img;
  V v_img;

  morpho::tree::compute_attribute_image(h_attr (), tree, &h_img);
  morpho::tree::compute_attribute_image(v_attr (), tree, &v_img);

  mysharp = morpho::attribute::mysharpness(tree, 0);


  {
    initialize(sharp, tree.f());
    mln_node_piter_(T) n(tree);
    for_all(n)
      sharp(n) = (double)(v_img(n).to_result()) / (double)((h_img(n).to_result()+1)  * v_img(n).area());
  }


  //sharp = normal_sharp(tree, h_img, v_img);

  unsigned w = 10;
  std::cout << std::setprecision(5) << std::left;
  std::cout << "| " << std::setw(22) << "node";
  std::cout << "| " << std::setw(w) << "mysharp";
  std::cout << "| " << std::setw(w) << "sharp";
  std::cout << "| " << std::setw(w) << "v(n)";
  std::cout << "| " << std::setw(w) << "h(n)";
  std::cout << "| " << std::setw(w) << "a(n)";
  std::cout << "| " << std::setw(w) << "h(p(n))";
  std::cout << "| " << std::setw(w) << "h'(n)";
  std::cout << "| " << std::setw(w) << "delta(n)";
  std::cout << '|' << std::endl;

  mln_depth1st_piter_(T) n(tree);
  std::stack<mln_psite_(T)> s;
  for_all(n)
  {
    std::cout << "| ";
    if (!s.empty() && tree.parent(n) == s.top())
      s.push(n);
    else {
      while (!s.empty() && s.top() != tree.parent(n))
	s.pop();
      s.push(n);
    }
    for (unsigned i = 0; i < s.size() / 5; i++)
      std::cout << '-';
    for (unsigned i = 0; i < s.size() % 5; i++)
      std::cout << '+';

    unsigned delta = math::diff_abs(tree.f(tree.parent(n)), tree.f(n));

    std::cout << n << "\t";
    std::cout << "| " << std::setw(w) << mysharp(n);
    std::cout << "| " << std::setw(w) << sharp(n);
    std::cout << "| " << std::setw(w) << v_img(n).to_result();
    std::cout << "| " << std::setw(w) << h_img(n).to_result() + 1;
    std::cout << "| " << std::setw(w) << v_img(n).area();
    std::cout << "| " << std::setw(w) << h_img(n).current_level();
    std::cout << "| " << std::setw(w) << h_img(n).base_level();
    std::cout << "| " << std::setw(w) << delta + h_img(n).to_result() + 1;
    std::cout << "| " << std::setw(w) << delta;
    std::cout << '|' << std::endl;
  }

}
