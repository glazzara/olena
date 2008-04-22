#ifndef TIKZ_H
# define TIKZ_H

#include <fstream>

namespace mln {
  namespace io {
    namespace tikz {

      template <typename I>
	void save (const Image<I>& input, std::string filename)
	{
	  const I& ima = exact(input);

	  std::ofstream file(filename.c_str());
	  file << "\\begin{tikzpicture}" << std::endl;
	  file << "\\tikzstyle{every node}=[draw,shape=rectangle, minimum width=1cm, minimum height=1cm]" << std::endl;

	  typename I::fwd_piter p(ima.domain());

	  int i = 0;

	  for_all(p)
	    file << "\\path " << p.to_point() << "node[fill=black!" << ((255 - ima(p)) * 100) / 255 << "] (p_" << i++ << ") { \\scriptsize "
		 << ((ima(p) < 127)?("\\color{white}"):("")) << ima(p) << "};" << std::endl;

	  file << "\\end{tikzpicture}" << std::endl;
	}

    } // namespace tikz
  } // namespace io
} // namespace mln




#endif // !TIKZ_H
