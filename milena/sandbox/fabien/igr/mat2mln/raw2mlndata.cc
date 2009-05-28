#include <iostream>
#include <fstream>

#include <mln/core/image/image3d.hh>
#include <mln/io/dump/save.hh>

#include <mln/io/pgm/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/stretch.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/image/image2d.hh>

using namespace mln;


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " input.raw output.dump" << std::endl;
    return 1;
  }

  std::ifstream input(argv[1]);

  unsigned int size = 0;
  unsigned int offset = 0;


  // Top level miMATRIX.
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Size: " << size << std::endl;

  // Array flags.
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Flags size: " << size << std::endl;

  // Dimensions array.
  offset += size;
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Dimensions size: " << size << std::endl;

  // Array name.
  offset += size;
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Name size: " << size << std::endl;
  char* name = new char[size + 1];
  input.seekg(offset);
  input.read((char*) name, size);
  name[size + 1] = '\0';
  std::cout << "Array name: " << name << std::endl;
  delete name;
  offset += 8 - size % 8;


  std::cout << "----" << std::endl;
  // FIXME: Decode data.

  offset = 128;


  // Data miMATRIX.
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Data size: " << size << std::endl;

  // Array flags.
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Flags size: " << size << std::endl;

  // Dimensions array.
  offset += size;
  offset += 4;
  input.seekg(offset);
  input.read((char*) &size, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  std::cout << "Dimensions size: " << size << std::endl;
  std::cout << "Number of dimensions: " << size / 4 << std::endl;
  int ncols = 0;
  int nrows = 0;
  int nslis = 0;
  input.seekg(offset);
  input.read((char*) &ncols, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  input.seekg(offset);
  input.read((char*) &nrows, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  input.seekg(offset);
  input.read((char*) &nslis, sizeof (unsigned int));
  offset += sizeof (unsigned int);
  offset += sizeof (unsigned int); // Padding.
  std::cout << "ncols: " << ncols << "; nrows: " << nrows << "; nslis: " << nslis << std::endl;

  // Data buffer.
  typedef double V;
  int data_size = ncols * nrows * nslis;
  V* data_buffer = new V[data_size];
  input.seekg(offset);
  input.read((char*) data_buffer, data_size);

  image3d<V> ima(nslis, nrows, ncols);
  memcpy(ima.buffer(), data_buffer, data_size);
  io::dump::save(ima, argv[2]);

  // Debug.
  image2d<V> ima_debug;
  ima_debug = duplicate(slice(ima, 50));
  //io::pgm::save(level::stretch(value::int_u8(), ima_debug), "debug.pgm");
  io::pgm::save(ima_debug, "debug.pgm");

  return 0;
}
