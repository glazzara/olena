# include <mln/core/io/pgm/save.hh>
int main()
{
  mln::image2d<mln::value::int_u8> ima;

  // ...

  io::pgm::save(ima, "my_image.pgm");
}
