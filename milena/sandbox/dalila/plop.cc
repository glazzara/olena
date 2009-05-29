#include <mln/essential/2d.hh>

int main()
{
	using namespace mln;

	image2d<bool> ima1(2, 3);
	image2d<bool> ima2(2, 3);

	data::fill(ima1, 2);
	data::fill(ima2, 3);

//	debug::println(ima1);
//	debug::println(ima2);

//	debug::println(ima2 - ima1);
//	debug::println(arith::minus(ima2, ima1));
	ima1 * ima2;
}
