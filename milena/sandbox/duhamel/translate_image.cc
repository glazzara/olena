#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/level/fill.hh>
#include <mln/level/paste.hh>
#include <mln/border/fill.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/debug/println.hh>
#include "translate_image.hh"

int main ()
{
  using namespace mln;

  typedef image2d<value::int_u8> I;

  I ima (4, 2, 1);
  debug::iota (ima);
  translate_image<I> tmp (ima, make::point2d (0,2) - make::point2d (0,0));
  std::cout << "orginal image domain : "
	    << ima.domain ()
	    << std::endl
	    << "translated image domain : "
	    << tmp.domain ()
	    << std::endl;

  std::cout << "original image :"
	    << std::endl;
  debug::println (ima);
  std::cout << std::endl;
  std::cout << "translated image :"
	    << std::endl;
  debug::println (tmp);
//   std::cout << std::endl;
//   I out (4,4);
//   level::paste(ima, out);
//   level::paste(tmp, out);
//   std::cout << "pasted image :"
// 	    << std::endl;
//   debug::println (out);
//   std::cout << std::endl;
}
