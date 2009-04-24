#include <iostream>
#include <fstream>

#include <zlib.h>

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " input.mat output.raw" << std::endl;
    return 1;
  }

  std::ifstream input(argv[1]);

  // Fetch data.
  unsigned int size;
  input.seekg(132);
  input.read((char*) &size, sizeof(unsigned int));
  std::cout << "Size: " << size << std::endl;
  unsigned char* data_buffer = new unsigned char[size];
  input.read((char*) data_buffer, size);
  input.close();

  // Decompress.
  unsigned long dest_size = 2 * size;
  unsigned char* dest_buffer = new unsigned char[dest_size];
  int dezip_result = uncompress(dest_buffer, &dest_size, data_buffer, size);

  switch (dezip_result)
  {
    case Z_OK:
      std::cout << "Z_OK" << std::endl;
      std::cout << "Dest size: " << dest_size << std::endl;
      break;
    case Z_MEM_ERROR:
      std::cout << "Z_MEM_ERROR" << std::endl;
      break;
    case Z_BUF_ERROR:
      std::cout << "Z_BUF_ERROR" << std::endl;
      break;
    case Z_DATA_ERROR:
      std::cout << "Z_DATA_ERROR" << std::endl;
      break;
  }

  // Write buffer to disk.
  std::ofstream save_file(argv[2]);
  save_file.write((char*) dest_buffer, dest_size);

  return 0;
}
