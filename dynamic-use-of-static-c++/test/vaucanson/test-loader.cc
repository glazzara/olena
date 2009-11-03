/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#include "dyn.hh"
using namespace dyn::language;

#include <vaucanson/xml/XML.hh>
// #include <dyn_vaucanson_mlc_name.hh>
  // dyn::post_include("dyn_vaucanson_mlc_name.hh");
// #include <vaucanson/xml/xml_converter.hh>
using namespace vcsn;
using namespace vcsn::io;
// using dyn::vcsn::xml::XML;
using namespace vcsn::xml;
using namespace xercesc;

int main(int, char**argv)
{
  std::string s = argv[1];
  std::cout << "load(" << s << ")" << std::endl;
  std::istream* is (s == "-" ? &std::cin : new std::ifstream (s.c_str()));
  if (not is->fail())
  {
    xercesc::XMLPlatformUtils::Initialize();

    xercesc::DOMElement* root_ = xerces_parser::stream_parser(*is);

    typedef Node<var> node_t;
    Factory<node_t, std::string> f;
    register_all_factory(f, var);
    node_t::map_t str2state;

    node_t* node = factory_create(f, xml2str(root_->getNodeName()));
    node->process(root_, var(), str2state, f);
    // xercesc::DOMNodeList* nodelist;

    // nodelist = doc->getElementsByTagName(STR2XML("type"));
    // std::cout << nodelist->getLength() << std::endl;

    xercesc::XMLPlatformUtils::Terminate();
  }
}
