# include <mln/core/image2d.hh>
# include <mln/core/site_set/p_key.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>



int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;
  I lena;
  io::pgm::load(lena, "../../../img/lena.pgm");
  
  typedef p_key<unsigned, point2d> S;
  S s;
  mln_piter_(I) p(lena.domain());
  for_all(p)
    s.insert(lena(p), p);

  mln_assertion(lena.nsites() == s.nsites());

  {
    I out(lena.domain());
    mln_piter_(I) p(lena.domain());
    mln_piter_(S) q(s);
    for_all_2(p, q)
      out(p) = lena(q);
    io::pgm::save(out, "out.pgm");
  }
}
