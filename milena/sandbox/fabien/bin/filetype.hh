#include <string>

// 2d
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

// 3d
#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>


// pbm
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

// pgm
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

// ppm
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>


//dump
#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>


namespace mln
{

  namespace filetype
  {
    enum id { pbm, pgm, ppm, dump, unknown };
  }

  filetype::id
  get_filetype(const std::string& filename)
  {
    if (filename.find(".pbm") == filename.length() - 4)
      return filetype::pbm;
    if (filename.find(".pgm") == filename.length() - 4)
      return filetype::pgm;
    if (filename.find(".ppm") == filename.length() - 4)
      return filetype::ppm;
    if (filename.find(".dump") == filename.length() - 5)
      return filetype::dump;
    return filetype::unknown;
  }

} // mln


