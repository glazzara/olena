# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/convert/to_window.hh>

# include <mln/debug/println.hh>
# include <mln/fun/p2v/iota.hh>
# include <mln/level/paste.hh>
# include <mln/level/fill.hh>
# include <mln/morpho/dilation.hh>



  struct is_cell_t :  mln::Function_p2b<is_cell_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }
  }
  is_cell;

  struct is_edge_t :  mln::Function_p2b<is_edge_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 + p.col() % 2 == 1;
    }
  }
  is_edge;

  struct is_point_t :  mln::Function_p2b<is_point_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 && p.col() % 2;
    }
  }
  is_point;



int main()
{
  using namespace mln;

  window2d c4 = convert::to_window(mln::c4());

  {
    image2d<char> ima(3, 5);
 
    level::fill(ima, 'o');

    debug::println(ima);
    // o o o o o 
    // o o o o o 
    // o o o o o 

    debug::println(ima | is_cell);
    // o   o   o 
    //        
    // o   o   o 
  
    level::fill((ima | is_cell).rw(),  'c');
    level::fill((ima | is_edge).rw(),  'e');
    level::fill((ima | is_point).rw(), 'p');

    debug::println(ima);
    // c e c e c 
    // e p e p e 
    // c e c e c 
  }

  {
    image2d<int> ima(3, 5);

    level::fill(ima, 0);
    debug::println(ima);
    // 0 0 0 0 0 
    // 0 0 0 0 0 
    // 0 0 0 0 0 

    level::fill((ima | is_cell).rw(), fun::p2v::iota);
    debug::println(ima | is_cell);
    // 1   2   3 
    //      
    // 4   5   6 

    //  --- for the record:
    // debug::println(morpho::dilation(ima, win::square(3)));

    debug::println(ima);
    // 1 0 2 0 3 
    // 0 0 0 0 0 
    // 4 0 5 0 6 

    debug::println(ima | is_edge);
    //   0   0   
    // 0   0   0 
    //   0   0   

    debug::println(morpho::dilation(ima | is_edge, c4));
    //   2   3   
    // 4   5   6 
    //   5   6   

    // Attention : les traitements ne travaillent généralement pas en
    // place ; l'entrée n'est pas modifiée, une nouvelle image est
    // retournée.  Dans le code ci-dessus, l'image dilatée est envoyée
    // à la routine d'affichage puis, n'étant plus utilisée, elle est
    // automatiquement désallouée.

    // On peut vérifier que 'ima' n'a pas été modifié :

    debug::println(ima);
    // 1 0 2 0 3 
    // 0 0 0 0 0 
    // 4 0 5 0 6 

    // En revanche, rien n'empêche de "coller" le résultat d'un
    // traitement dans l'image d'entrée :

    level::paste(morpho::dilation(ima | is_edge, c4), ima);

    // Maintenant, le dilaté de "ima restreint à edge" est collé dans
    // 'ima' :

    debug::println(ima);
    // 1 2 2 3 3 
    // 4 0 5 0 6 
    // 4 5 5 6 6 

    level::paste(morpho::dilation(ima | is_point, c4), ima);
    debug::println(ima);
    // 1 2 2 3 3 
    // 4 5 5 6 6 
    // 4 5 5 6 6 
  }

}
