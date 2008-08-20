# include <mln/core/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/value/int_u8.hh>

# include <mln/level/fill.hh>
# include <mln/debug/println.hh>
# include <mln/util/timer.hh>
# include <mln/labeling/blobs.hh>

# include <mln/core/p_vaccess.hh>
# include <mln/convert/to_p_run_set.hh>


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
    typedef p_vaccess< value::set<bool>, p_array<point2d> > Arr;
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
    typedef p_vaccess< value::set<int_u8>, p_run_set<point2d> > Arr;
    mlc_equal(Arr::element, p_run<point2d>)::check();
    Arr arr;

    convert::impl::fill_p_run_set_(arr, lab);
    mln_assertion(arr.nsites() == n);

    for (unsigned l = 1; l <= nlabels; ++l)
      {
	mln_assertion(! arr(l).is_empty());
	std::cout << l << ": #"
		  << arr(l).nsites() << " in "
		  << arr(l).bbox() << std::endl;
      }
  }

//   p_run_set<point2d> rs = convert::to_p_run_set(ima);
//   std::cout << rs.zratio() << std::endl;
//   std::cout << rs.bbox() << std::endl;


//   image2d<value::int_u8> ima_(ima.domain());
//   level::fill(ima_, 0);
//   for (unsigned r = 0; r < rs.nruns(); ++r)
//     {
//       mln_piter_(p_run<point2d>) p(rs.run(r));
//       for_all(p)
// 	ima_(p) = r + 1;
//     }
//   debug::println(ima_);
//   io::pgm::save(ima_, "tmp.pgm");
}
