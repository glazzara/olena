//
// thinvox, a binary voxel thinning program
// Copyright (c) 2004-2007 by Patrick Min, patrick.n.min "at" gmail "dot" com
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// 
//
// $Id: Templates.cc,v 1.5 2001/01/20 02:38:09 min Exp $
//

#include "binvox/voxel/Templates.h"

using namespace std;

using namespace binvox;



Templates::Templates()
{
  type = 0;
  init_usw();

}  // constructor



void
Templates::convert_type(int new_type)
{
//    cout << "Templates::convert_type(" << new_type << ")" << endl;

//    cout << "type: " << type << " type & UD_MASK: " << (type & UD_MASK)
//         << ", type & NS_MASK: " << (type & NS_MASK)
//         << ", type & WE_MASK: " << (type & WE_MASK) << endl;
//    cout << "new_type: " << new_type << " new_type & UD_MASK: " << (new_type & UD_MASK)
//         << ", new_type & NS_MASK: " << (new_type & NS_MASK)
//         << ", new_type & WE_MASK: " << (new_type & WE_MASK) << endl;
  
  if ((type & UD_MASK) != (new_type & UD_MASK)) {
    //    cout << "  calling flip_ud" << endl;
    for(int i=0; i < nr_templates; i++)
      T[i].flip_ud();
  }
  if ((type & NS_MASK) != (new_type & NS_MASK)) {
    //    cout << "  calling flip_ns" << endl;
    for(int i=0; i < nr_templates; i++)
      T[i].flip_ns();
  }
  if ((type & WE_MASK) != (new_type & WE_MASK)) {
    //    cout << "  calling flip_we" << endl;
    for(int i=0; i < nr_templates; i++)
      T[i].flip_we();
  }

  type = new_type;

}  // Templates::convert_type

    

int
Templates::already_present(Template& t)
{
  for(int i=0; i < nr_templates; i++) {
    if (T[i] == t) {
      //      cout << "matches existing template " << i + 1 << endl;
      return 1;
    }
  }
  return 0;

}  // Templates::already_present



void
Templates::add(Template& t)
{
  T.push_back(t);
  nr_templates++;
  //  cout << "added template, new nr_templates: " << nr_templates << endl;
  
}  // Templates::add



//
// init USW as in palagyi99 paper
//
void
Templates::init_usw()
{
  // first init 7 base templates
  init_usw_base_templates();
  
  // then reflect each through three planes
  // and check if already present
  for(int i=0; i < nr_templates; i++) {  // !!! nr_templates !!!
    //    cout << "Base template " << i + 1 << ": " << endl << T[i] << endl;
    
    Template refl;

    refl = T[i];
    refl.reflect_sw_ne();
    //    cout << "reflecting through sw_ne" << endl;
    if (!already_present(refl)) add(refl);

    refl = T[i];
    refl.reflect_us_dn();
    //    cout << "reflecting through us_dn" << endl;
    if (!already_present(refl)) add(refl);
    
    refl = T[i];
    refl.reflect_uw_de();
    //    cout << "reflecting through uw_de" << endl;
    if (!already_present(refl)) add(refl);

  }  // for each base template

  //  cout << "Templates::init_usw done, " << nr_templates << " templates created." << endl;

}  // init_usw



static char B[7][28] = {
  "wpbwppwwwwppwbpwwwwwwwwwwww",
  "wppwpbwppwppwbpwppwwwwwwwww",
  "ppppbppppppppbppppwwwwwwwww",
  "dbdwwbwwddddwbdwwdwwwwwwwww",
  "ddddbdddddbddbdddddddwwwwww",
  "ddddbdddddbddbbddddddwwdwwd",
  "ddddbddddddbdbddddwwbwwwwww"
};
   
void
Templates::init_usw_base_templates()
{
  for(int i=0; i < 7; i++) {
    Template templ;
    add(templ);
    for(int j=0; j < 27; j++) {
      switch (B[i][j]) {
	case 'w': T[i][j] = WHITE; break;
	case 'b': T[i][j] = BLACK; break;
	case 'd': T[i][j] = DONT_CARE; break;
	case 'p': T[i][j] = POTENTIAL_BLACK; break;
	default:
	  cout << "Templates::init_usw_base_templates error" << endl;
      }
    }
  }
  nr_templates = 7;
  
}  // Templates::init_usw_base_templates


