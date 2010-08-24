#include <mln/core/image/image2d.hh>
#include <mln/util/array.hh>
#include <mln/util/timer.hh>
#include <mln/data/fill.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


namespace mln
{

  template <typename I>
  void
  test1(const I& input, I& out)
  {
    unsigned s = 2;
    util::timer timer_;
    timer_.start();

    mln_box_runstart_piter(I) sp(out.domain());

    const unsigned n = sp.run_length();

    util::array<      mln_value(I)*> ptr(s);
    util::array<const mln_value(I)*> ptr_input(s);

    sp.start();
    while (sp.is_valid())
    {
      mln_site(I)
	site = sp; // Site at scale 1.

      unsigned nptr = 0;
      for (; sp.is_valid() && nptr < ptr.size(); ++nptr)
      {
	ptr(nptr) = & out(sp);
	ptr_input(nptr) = & input(sp);
	sp.next();
      }
      ptr.resize(nptr);

      for (unsigned col = 0; col < n; col += s, site[1] += s)
	for (unsigned i = 0; i < ptr.size() ; ++i)
	  for (unsigned j = 0; j < s ; ++j)
	    *ptr(i)++ = *ptr_input(i)++;
    }

    float t_ = timer_;
    std::cout << "Test 1 - " << t_ << std::endl;
  }


  template <typename I>
  void
  test2(const I& input, I& out)
  {
    unsigned s = 2;
    util::timer timer_;
    timer_.start();

    mln_box_runstart_piter(I) sp(out.domain());

    const unsigned n = sp.run_length();

    util::array<      mln_value(I)*> ptr(s);
    util::array<const mln_value(I)*> ptr_input(s);

    sp.start();
    while (sp.is_valid())
    {
      mln_site(I)
	site = sp; // Site at scale 1.

      unsigned nptr = 0;
      for (; sp.is_valid() && nptr < ptr.size(); ++nptr)
      {
	ptr(nptr) = & out(sp);
	ptr_input(nptr) = & input(sp);
	sp.next();
      }
      ptr.resize(nptr);

      for (unsigned col = 0; col < n; col += s, site[1] += s)
      {
	for (unsigned i = 0; i < ptr.size() ; ++i)
	{
	  memcpy(ptr(i), ptr_input(i), s * sizeof(mln_value(I)));
	  ptr(i) += s;
	  ptr_input(i) += s;
	}
      }
    }

    float t_ = timer_;
    std::cout << "Test 2 - " << t_ << std::endl;
  }

  template <typename I>
  void
  test3(const I& input, I& out)
  {
    unsigned s = 2;
    util::timer timer_;
    timer_.start();

    mln_box_runstart_piter(I) sp(out.domain());

    const unsigned n = sp.run_length();

    int row_offset = input.domain().ncols() + 2 * input.border();
    int row_offset_s = row_offset - s;
    int row_offset_next = - (s - 1) * row_offset;

          mln_value(I) *ptr;
    const mln_value(I) *ptr_input;

    sp.start();
    while (sp.is_valid())
    {
      mln_site(I)
	site = sp; // Site at scale 1.

      ptr = & out(sp);
      ptr_input = & input(sp);

      for (unsigned col = 0; col < n; col += s, site[1] += s)
      {
	for (unsigned i = 0; i < s;)
	{
	  for (unsigned j = 0; j < s; ++j)
	    *ptr++ = *ptr_input++;

	  ++i;
	  if (i < s)
	  {
	    ptr += row_offset_s;
	    ptr_input += row_offset_s;
	  }
	}

	ptr += row_offset_next;
	ptr_input += row_offset_next;
      }


      for (unsigned j = 0; j < s; ++j)
	sp.next();
    }

    float t_ = timer_;
    std::cout << "Test 3 - " << t_ << std::endl;
  }


  template <typename I>
  void
  test4(const I& input, I& out)
  {
    unsigned s = 2;
    util::timer timer_;
    timer_.start();

    mln_box_runstart_piter(I) sp(out.domain());

    const unsigned n = sp.run_length();

    int row_offset = input.domain().ncols() + 2 * input.border();
    int row_offset_s = row_offset - s;
    int row_offset_next = - s * row_offset + s;

          mln_value(I) *ptr;
    const mln_value(I) *ptr_input;

    sp.start();
    while (sp.is_valid())
    {
      mln_site(I)
	site = sp; // Site at scale 1.

      ptr = & out(sp);
      ptr_input = & input(sp);

      for (unsigned col = 0; col < n; col += s, site[1] += s)
      {
	for (unsigned i = 0; i < s; ++i)
	{
	  memcpy(ptr, ptr_input, s * sizeof(mln_value(I)));
	  ptr += s;
	  ptr_input += s;

	  ptr += row_offset_s;
	  ptr_input += row_offset_s;
	}

	ptr += row_offset_next;
	ptr_input += row_offset_next;
      }


      for (unsigned j = 0; j < s; ++j)
	sp.next();
    }

    float t_ = timer_;
    std::cout << "Test 4 - " << t_ << std::endl;
  }




} // end of namespace mln

int main()
{
  using namespace mln;

  def::coord
    r = 6144,
    c = 6252;

  image2d<unsigned> input(r, c), output(r, c);
  debug::iota(input);

  {
    data::fill(output, 0);
    test1(input, output);
//     debug::println(output);
  }

  {
    data::fill(output, 0);
    test2(input, output);
//     debug::println(output);
  }

  {
    data::fill(output, 0);
    test3(input, output);
//     debug::println(output);
  }

  {
    data::fill(output, 0);
    test4(input, output);
//     debug::println(output);
  }
}
