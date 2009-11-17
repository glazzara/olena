#include <mln/core/image/image2d.hh>
#include <mln/util/array.hh>
#include <mln/util/timer.hh>
#include <mln/data/fill.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>


namespace mln
{

//   template <typename I>
//   void
//   test1(const I& input, I& out)
//   {
//     unsigned s = 2;
//     util::timer timer_;
//     timer_.start();

//     mln_box_runstart_piter(I) sp(out.domain());

//     const unsigned n = sp.run_length();

//     unsigned row_offset = input.domain().ncols()
//       + 2 * input.border();

//     util::array<      mln_value(I)*> ptr(s);
//     util::array<const mln_value(I)*> ptr_input(s);

//     sp.start();
//     while (sp.is_valid())
//     {
//       mln_site(I)
// 	site = sp; // Site at scale 1.

//       unsigned nptr = 0;
//       for (; sp.is_valid() && nptr < ptr.size(); ++nptr)
//       {
// 	ptr(nptr) = & out(sp);
// 	ptr_input(nptr) = & input(sp);
// 	sp.next();
//       }
//       ptr.resize(nptr);

//       for (unsigned col = 0; col < n; col += s, site[1] += s)
//       {
// 	for (unsigned i = 0; i < ptr.size() ; ++i)
// 	  for (unsigned j = 0; j < s; ++j)
// 	    *ptr(i)++ = *ptr_input(i)++;
//       }
//     }

//     float t_ = timer_;
//     std::cout << "Test 1 - " << t_ << std::endl;
//   }


  template <typename I>
  void
  test2(const I& input, I& out, unsigned s)
  {
    const unsigned s_2 = s * s, round_it = s_2 / 2;
    util::timer timer_;
    timer_.start();

    typedef mln_value(I) V;
    typedef mln_sum(V) S;

    mln_box_runstart_piter(I) sp(input.domain());

    const unsigned n = sp.run_length();

    int row_offset = input.domain().ncols() + 2 * input.border();
    int row_offset_s = row_offset - s;
    int row_offset_next = - (s - 1) * row_offset;

    mln_value(I)* ptr = out.buffer();
    const mln_value(I)* ptr_input;

    sp.start();
    while (sp.is_valid())
    {
      mln_site(I)
	site = sp; // Site at scale 1.

      ptr_input = & input(sp);

      for (unsigned col = 0; col < n; col += s, site[1] += s)
      {

	// tile
	S sum = 0;
	for (unsigned i = 0; i < s;)
	{
	  for (unsigned j = 0; j < s; ++j)
 	    sum += *ptr_input++;

	  ++i;
	  if (i < s)
	    ptr_input += row_offset_s;
	}

	ptr_input += row_offset_next;
	*ptr++ = sum / s_2;
      }

      for (unsigned i = 0; i < s; ++i)
	sp.next();
    }

    float t_ = timer_;
    std::cout << t_ << std::endl;
  }



} // end of namespace mln

int main()
{
  using namespace mln;
  using value::int_u8;

  def::coord
    r = 3144,
    c = 2252;

  image2d<int_u8> input;
  io::pgm::load(input, "in.pgm");

  unsigned s = 3;
  box2d b = make::box2d((input.nrows() + s - 1) / s,
			(input.ncols() + s - 1) / s);
  image2d<int_u8> output(b, 0);
  test2(input, output, s);

  io::pgm::save(output, "out.pgm");

}
