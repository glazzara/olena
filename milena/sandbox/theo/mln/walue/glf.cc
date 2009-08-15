#include <mln/walue/glf.hh>
#include <mln/walue/gl8.hh>
#include <mln/literal/white.hh>


int main()
{
  using namespace mln;
  using walue::glf1;
  using walue::glf2;

  glf1 g = walue::set_encoding(0.5);
  g = g * 2;
  mln_assertion(g == literal::white);


  {
    using walue::gl8;
    gl8 g = literal::white;
    glf1 f = g;
    mln_assertion(f == g);
  }

  // Tests on approx_equal.
  {
    glf1 g = 0.49999, g_ = 0.50001;
    mln_assertion(approx_equal(g, g_) == false);
  }
  {
    glf1 g = 0.499999, g_ = 0.500001;
    mln_assertion(approx_equal(g, g_));
    mln_assertion(approx_equal(g, 0.5));
  }
  {
    glf1 g1 = 0.499999;
    glf2 g2 = 0.500001;
    mln_assertion(approx_equal(g1, g2));
  }
}
