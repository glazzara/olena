using namespace mln;


/* En faire une interface? pour pouvoir changer le type de graphe généré à partir de lbl? 
*/

namespace mln
{
  template <typename N, typename E, typename I>
  class graph_link : public graph<N, E>
  {
    I& lbl;
    public:
      void graph_link(const I& lbl);
      void ~graph_link();
      void rem_edge(E& e);
      void rem_vertex(node_id v);
  }

  void graph_link::graph_link(const I& lbl)
  {
    // Créer le graphe à partir de l'image d'étiquettes. (Utiliser segm_to_pregraph? bah non, on le fera dans le cas particulier)
  }
  
  void graph_link::~graph_link()
  {
  }
  
  void rem_edge(E& e) // Faisable?
  {
  }
  
  void rem_vertex(node_id v)
  {
    
  }
}