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
