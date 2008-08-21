# include <mln/core/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/value/int_u8.hh>

# include <mln/level/fill.hh>
# include <mln/debug/println.hh>
# include <mln/util/timer.hh>
# include <mln/labeling/blobs.hh>

# include <mln/core/alias/p_runs2d.hh>
# include <mln/core/p_vaccess.hh>
# include <mln/core/image_if.hh>
# include <mln/pw/all.hh>
# include <mln/convert/from_to.hh>



template <typename I, typename S>
void fill(S& s, const I& ima)
{
  mln_piter(I) p(ima.domain());
  for_all(p)
    s.insert(ima(p), p);
  mln_postcondition(s.nsites() == ima.nsites());
}



int main()
{
  using namespace mln;
  using value::int_u8;

  bool vals[4][4] = {
    {1, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 1}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);

  unsigned n;

  {
    typedef p_vaccess<bool, p_array<point2d> > Arr;
    mlc_equal(Arr::element, point2d)::check();
    Arr arr;
    fill(arr, ima);
    n = arr(true).nsites();
    mln_assertion(arr(true).nsites() + arr(false).nsites() == ima.nsites());
    
    std::cout << "{-} = " << arr(false) << std::endl
	      << "{|} = " << arr(true) << std::endl
	      << std::endl;
  }

  int_u8 nlabels;
  image2d<int_u8> lab = labeling::blobs(ima, c4(), nlabels);
  debug::println(lab);

  {
    typedef p_vaccess<int_u8, p_runs2d> Arr;
    mlc_equal(Arr::element, p_run2d)::check();
    Arr arr;

    convert::from_to(lab | (pw::value(lab) != pw::cst(0u)), arr);
    mln_assertion(arr.nsites() == n);

    for (unsigned l = 0; l <= nlabels; ++l)
      {
	std::cout << l << ": #" << arr(l).nsites();
	if (! arr(l).is_empty())
	  std::cout << " in " << arr(l).bbox();
	std::cout << std::endl;
      }
    std::cout << std::endl;
  }

  {
    p_runs2d rs;
    convert::from_to(ima, rs);
    mln_assertion(rs.bbox() == ima.bbox());

    image2d<int_u8> ima_(ima.domain());
    level::fill(ima_, 0);
    for (unsigned r = 0; r < rs.nelements(); ++r)
      {
	mln_piter_(p_run2d) p(rs[r]);
	for_all(p)
	  ima_(p) = r + 1;
      }
    debug::println(ima_);
  }

}
