#include <Magick++.h>
#include <iostream>


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  std::cout << "Loading image..." << std::endl;
  Magick::Image image(argv[1]);
  std::cout << "Loading done." << std::endl;

  std::cout << "columns: " << image.columns() << std::endl;
  std::cout << "rows: " << image.rows() << std::endl;

  unsigned ncols = image.columns();
  unsigned nrows = image.rows();
  const Magick::PixelPacket *pixel_cache;
  unsigned count = 0;
  for (unsigned row = 0; row < nrows; row += 1024)
  {
    std::cout << "Processing row #" << row << std::endl;
    pixel_cache = image.getConstPixels(0, 0, ncols / 128, row);
    for (unsigned col = 0; col < ncols / 128; col += 256)
    {
      const Magick::PixelPacket *pixel = pixel_cache + col;
      ++count;
    }
  }

  std::cout << "nbr pixels: " << count << std::endl;

  return 0;
}
