#include <mln/walue/gl8.hh>
#include <mln/walue/gl16.hh>
#include <mln/literal/white.hh>
#include <mln/literal/black.hh>



int main()
{
  using namespace mln;
  using walue::gl8;
  using walue::gl16;
  using walue::glf1;
  using walue::set_encoding;

  {
    gl8 g1 = set_encoding(127);
    mln_assertion(g1.encoding() == 127);
    gl8 g2 = set_encoding(128);
    mln_assertion((g1 + g2) / 2 == 0.5);
  }

  {
    gl8 g8 = set_encoding(51);
    mln_assertion(approx_equal(g8, 0.2));

    gl16 g16 = 0.2;
    mln_assertion(g16.encoding() == 13107);
    mln_assertion(approx_equal(g8, g16));
  }

  {
    gl8 g;

    g = 0.;
    mln_assertion(g == literal::black);

    g = 1.;
    mln_assertion(g == literal::white);
    
    g = 0.2;
    mln_assertion(5 * g == literal::white);
  }

  {
    gl8  g1 = literal::white;
    gl16 g2 = g1;
    mln_assertion(g2 == literal::white);
  }

  {
    gl8  g1 = literal::white;
    gl16 g2 = literal::white;
    mln_assertion((g1 + g2) / 2 == literal::white);
    // std::cout << "g1 = " << g1 << ", " << "g2 = " << g2 << std::endl;
    mln_assertion(g1 <= g2 && g2 >= g1);
    mln_assertion(g1 == g2);
  }


  {
    using walue::gl;
    gl<31> g1 = literal::white;
    gl<32> g2 = literal::white;
    mln_assertion(g2 == g1);
    g2.set_encoding(g2.encoding() - 1);
    mln_assertion(g2 < g1);
  }

//   // KO: Ambiguous code do not compile :-)
//   {
//     walue::int_u8 i = 1;
//     g = i;
//   }

}
