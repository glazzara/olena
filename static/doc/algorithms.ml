(* Prototyping SCOOP's algorithms on virtual types in Objective Caml.  *)


(*--------.
| Types.  |
`--------*)

module StringType =
  struct
    type t = string
    let compare = String.compare
  end
;;

module StringMap = Map.Make (StringType);;


type cxx_type =
  (* Static stypes. *)
  | Stc_None
  | Stc_Not_found
  | Stc_Abstract
  | Stc_Not_delegated
  | Stc_Not_delegated_Abstract
  | Stc_Final of cxx_type

  (* C++ types. *)
  | Std_Char
  | Std_Short
  | Std_Int
  | Std_Unsigned
  | Std_Float
  | Scoop_Class of scoop_class

and vtypes_dict = cxx_type StringMap.t
and scoop_class = { super: cxx_type; vtypes: vtypes_dict }
;;

(* A helper to create virtual types sets.  *)
let rec create_vtypes = function
  | [] -> StringMap.empty
  | (type_name, type_val)::t ->
      StringMap.add type_name type_val (create_vtypes t)
;;
     

(*-----------------.
| Error handling.  |
`-----------------*)

exception Scoop_exception of string;;

let error =
  function s -> raise (Scoop_exception s)
;;


(*----------------------.
| Virtual type lookup.  |
`----------------------*)

let find_local (source: cxx_type) (target: string) : cxx_type =
  match source with
    | Scoop_Class c ->
	begin
	  try
	    StringMap.find target c.vtypes
	  with Not_found -> Stc_Not_found
	end
    | _ -> raise (Scoop_exception "find_local: source is not SCOOP class.")
;;

let merge2 (local_res : cxx_type) (super_res : cxx_type) : cxx_type =
  match local_res, super_res with

  (* local_res == stc::not_found.  *)
  | Stc_Not_found, Stc_Not_found -> Stc_Not_found
  | Stc_Not_found, Stc_Abstract  -> Stc_Not_found
  | Stc_Not_found, Stc_Final u   -> Stc_Final u
  | Stc_Not_found, u             -> u

  (* local_res == stc::abstract.  *)
  | Stc_Abstract,  Stc_Abstract  -> Stc_Abstract
  | Stc_Abstract,  Stc_Not_found -> Stc_Abstract
  | Stc_Abstract,  Stc_Final _   -> error "Final VT redefined abstract"
  | Stc_Abstract,  _             -> error "VT redefined abstract."

  (* local_res == stc::final<T>.  *)
  | Stc_Final _,   Stc_Final _   -> error "Final VT redefined final."
  | Stc_Final t,   _             -> Stc_Final t

  (* local_res == T.  *)
  | _,             Stc_Final _   -> error "Final VT redefined."
  | t,             _             -> t
;;

(* Another version (rewriting) of merge3.  *)
let merge3 (local_res : cxx_type) (super_res : cxx_type)
    (delegatee_res : cxx_type) : cxx_type =
  match local_res, super_res, delegatee_res with

  (* local_res == stc::not_found.  *)
  | Stc_Not_found, Stc_Not_found, Stc_Not_found  -> Stc_Not_found
  | Stc_Not_found, Stc_Not_found, Stc_Abstract   -> Stc_Not_found
  | Stc_Not_found, Stc_Not_found, Stc_Final v    -> Stc_Final v
  | Stc_Not_found, Stc_Not_found, v              -> v

  | Stc_Not_found, Stc_Abstract,  Stc_Not_found  -> Stc_Not_found
  | Stc_Not_found, Stc_Abstract,  Stc_Abstract   -> Stc_Abstract
  | Stc_Not_found, Stc_Abstract,  Stc_Final v    -> Stc_Final v
  | Stc_Not_found, Stc_Abstract,  v              -> v

  | Stc_Not_found, Stc_Final u,   _              -> Stc_Final u
  | Stc_Not_found, u,             _              -> u

  (* local_res == stc::abstract.  *)
  | Stc_Abstract,  Stc_Not_found, Stc_Not_found -> Stc_Abstract
  | Stc_Abstract,  Stc_Not_found, Stc_Abstract  -> Stc_Abstract
  | Stc_Abstract,  Stc_Not_found, Stc_Final v   -> Stc_Final v
  | Stc_Abstract,  Stc_Not_found, v             -> v

  | Stc_Abstract,  Stc_Abstract,  Stc_Not_found -> Stc_Abstract
  | Stc_Abstract,  Stc_Abstract,  Stc_Abstract  -> Stc_Abstract
  | Stc_Abstract,  Stc_Abstract,  Stc_Final v   -> Stc_Final v
  | Stc_Abstract,  Stc_Abstract,  v             -> v

  | Stc_Abstract,  Stc_Not_delegated_Abstract, _ -> Stc_Not_delegated_Abstract
  | Stc_Abstract,  Stc_Not_delegated, _         -> Stc_Not_delegated_Abstract

  | Stc_Abstract,  Stc_Final _,   _             -> error ("Final VT " ^
                                                          "redefined abstract")
  | Stc_Abstract,  _,             _             -> error ("VT redefined " ^
                                                          "abstract.")

  (* local_res == stc::not_delegated.  *)
  | Stc_Not_delegated,  Stc_Not_found, _        -> Stc_Not_delegated
  | Stc_Not_delegated,  Stc_Abstract,  _        -> Stc_Not_delegated_Abstract
  | Stc_Not_delegated,  Stc_Not_delegated_Abstract, _ -> Stc_Not_delegated_Abstract

  (* local_res == stc::not_delegated_abstract.  *)
  (* FIXME: Shouldn't we introduce a means to tag a vtype both
     as abstract *and* not delegated?  (Currently, the rule below
     prevents this).  *)
  | Stc_Not_delegated_Abstract, _, _            -> error ("Local declaration" ^
							  " of not delegated" ^
							  " and abstract")

  (* local_res == stc::final<T>.  *)
  | Stc_Final _,    Stc_Final _,  _             -> error ("Final VT " ^
                                                          "redefined final.")
  | Stc_Final t,    _,            _             -> Stc_Final t

  (* local_res == T.  *)
  | _,              Stc_Final _,  _             -> error ("Final VT " ^
                                                          "redefined.")
  | t,              _,            _             -> t
;;


let rec find_rec (source : cxx_type) (target : string) : cxx_type =
  match source with
    | Stc_None -> Stc_Not_found
    | Scoop_Class c ->
	begin
	  let local_res = find_local source target in

	  let super_res = find_rec c.super target in
	    (* FIXME: This might not be efficient, since
	       find_rec_in_super can be called several times on the
	       nodes of the delegation branch when going down the
	       inheritance branch.  Of course, the declarative nature
	       of C++ templates will avoid this cost, but it remains
	       inelegant, IMHO.  *)
	  let delegatee = find_rec_in_supers source "delegatee_type" in
	  let delegatee_res =
	    match delegatee with
	    | Stc_Not_found -> Stc_Not_found
	    | Stc_Abstract  -> Stc_Not_found
	    | _             -> find_rec delegatee target in

	    merge3 local_res super_res delegatee_res
	end
    | _ -> error "find_rec: source is not a SCOOP class."

(* Like find_rec, but only search in the inheritance branch.  *)
and find_rec_in_supers (source : cxx_type) (target : string) : cxx_type =
  match source with
    | Stc_None -> Stc_Not_found
    | Scoop_Class c ->
	begin
	  let local_res = find_local source target in
	  let super_res = find_rec c.super target in
	    merge2 local_res super_res
	end
    | _ -> error "find_rec_in_supers: source is not a SCOOP class."
;;


let find (source : cxx_type) (target : string) : cxx_type =
  match find_rec source target with
    | Stc_Abstract               -> error "find: VT is abstract."
    | Stc_Not_delegated_Abstract -> error "find: VT is abstract."
    | Stc_Not_delegated          -> Stc_Not_found
    | Stc_Final t                -> t
    | t                          -> t
;;



(*-----------.
| Examples.  |
`-----------*)

(* ** Virtual type definition

     Ex.

       class A < stc::none =
       {
	 vtype my_type = int;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Std_Int] } in
assert (find a "my_type" = Std_Int);;


(* ** Virtual type re-definition.

     Ex.

       class A < stc::none =
       {
	 vtype my_type = int;
       }
       // Valid.
       class B < A =
       {
	 vtype my_type = float;
       }
       // Valid.
       class C < A =
       {
	 vtype my_type = int;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Std_Int] } in
let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Std_Float] } in
let c =
  Scoop_Class { super = b;
		vtypes = create_vtypes ["my_type", Std_Int] } in
assert (find c "my_type" = Std_Int);;


(* ** Virtual type abstract declaration (i.e., deferred definition).
 
     Ex.:

       class A < stc::none =
       {
	 // my_type is abstract.
	 vtype my_type = 0;
       }
       class B < A =
       {
	 vtype my_type = int;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Stc_Abstract] } in
let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Std_Int] } in
assert (find b "my_type" = Std_Int);;

(* - A concrete virtual type cannot be redefined as abstract.

     Ex.:

       class A < stc::none =
       {
	 vtype my_type = int;
       }
       // Invalid.
       class B < A =
       {
	 vtype my_type = 0;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Std_Int] } in
let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Stc_Abstract] } in
try
  ignore (find b "my_type")
with Scoop_exception "VT redefined abstract." -> ();;


(* ** Final virtual type

   - A virtual type can be tagged as final.

     Ex.:

       class A < stc::none =
       {
	 final vtype my_type = int;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Stc_Final Std_Int] } in
assert (find a "my_type" = Std_Int);;

(* - A virtual type tagged as final in a class cannot be redefined in its
     subclasses, either tagged final or not, even if its value is
     unchanged.

     Ex.:

       class A < stc::none =
       {
	 final vtype my_type = int;
       }
       // Invalid.
       class B < A =
       {
	 vtype my_type = float;
       }
       // Invalid.
       class C < A =
       {
	 vtype my_type = int;
       }
       // Invalid.
       class D < A =
       {
	 final vtype my_type = int;
       }
       // Invalid.
       class E < A =
       {
	 final vtype my_type = float;
       }
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Stc_Final Std_Int] } in
let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Std_Float] } in
let c =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Std_Int] } in
let d =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Stc_Final Std_Int] } in
let e =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["my_type", Stc_Final Std_Float] } in

  assert (find a "my_type" = Std_Int);
  try ignore (find b "my_type")
  with Scoop_exception "Final VT redefined." -> ();
  try ignore (find c "my_type")
  with Scoop_exception "Final VT redefined." -> ();
  try ignore (find d "my_type")
  with Scoop_exception "Final VT redefined final." -> ();
  try ignore (find e "my_type")
  with Scoop_exception "Final VT redefined final." -> ();
;;

(* ** General virtual type lookup (i.e., with delegation)

   *** Abstract

     class A < stc::none
     {
       // my_vtype not defined.
     }

     class D < stc::none
     {
       vtype my_type = 0;
     }

     // A
     // ^
     // |
     // C<>--D
     class C < stc::none
     {
       vtype delegatee_type = D;
     }

     type t = C#my_type;
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes [] } in
let d =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["my_type", Stc_Abstract] } in
let c =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["delegatee_type", d] } in
assert (find c "my_type" = Stc_Not_found);;


(* More examples.

   ** A complex example, with stc::not_delegated.

                                 A
                                 ^
                                 |    X
                                 B    ^
                                 ^    |
                                 |    Y
                                 C<>-´
                                 ^
                                 |
                                 D
                                 ^
                                 |
                                 E


     class A < stc::none         |     class C < B
     {                           |     {
     }                           |       vtype delegatee_type = Y;
     class B < A                 |       vtype foo = int;
     {                           |       vtype baz = not_delegated;
       vtype foo = 0;            |       vtype hop = not_delegated;
     }                           |     }
                                 |     class D < C
     class X < stc::none         |     {
     {                           |       vtype quux = unsigned;
       vtype bar = 0;            |     }
       vtype hop = int;          |     class E < D
     }                           |     {
     class Y < X                 |       vtype baz = float;     
     {                           |     }
       vtype baz = short;        |
       vtype bar = char;         |
     }                           |
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes [] } in
let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes ["foo", Stc_Abstract] } in

let x =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes [("bar", Stc_Abstract);
					("hop", Std_Int)] } in
let y =
  Scoop_Class { super = x;
		vtypes = create_vtypes [("bar", Std_Char);
					("baz", Std_Short)] } in

let c =
  Scoop_Class { super = b;
		vtypes = create_vtypes [("delegatee_type", y);
					("foo", Std_Int);
					("baz", Stc_Not_delegated);
					("hop", Stc_Not_delegated)] } in
let d =
  Scoop_Class { super = c;
		vtypes = create_vtypes [("quux", Std_Unsigned)] } in
let e =
  Scoop_Class { super = d;
		vtypes = create_vtypes [("baz", Std_Float)] } in

assert (find e "foo" = Std_Int);
assert (find e "bar" = Std_Char);
assert (find e "baz" = Std_Float);
assert (find e "quux" = Std_Unsigned);
assert (find e "hop" = Stc_Not_found);
;;


(* *** abstract redefined with tag stc::not_delegated.

     class A < stc::none                |
     {                                  |
       vtype foo = 0                    |
     }                                  |
     class B < A                        |  class X < stc::none
     {                                  |  {
       vtype delegatee_type = X;        |    vtype foo = int;
       vtype foo = stc::not_delegated   |  }
     }                                  |
     class C < B                        |
     {                                  |
     }                                  |
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["foo", Stc_Abstract] } in

let x =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["foo", Std_Int] } in

let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes [("delegatee_type", x);
					("foo", Stc_Not_delegated)] } in
let c =
  Scoop_Class { super = b;
		vtypes = create_vtypes [] } in

(* foo is abstract and tagged ``not delegated'' for B.  *)
try ignore (find b "foo") with Scoop_exception "find: VT is abstract." -> ();
(* Likewise for C.  *)
try ignore (find c "foo") with Scoop_exception "find: VT is abstract." -> ()
;;


(* *** abstract redefined with tag stc::not_delegated.

   This is the converse situation from the previous example (foo is
   declared abstract in C instead of A).

     class A < stc::none                |
     {                                  |
     }                                  |
     class B < A                        |  class X < stc::none
     {                                  |  {
       vtype delegatee_type = X;        |    vtype foo = int;
       vtype foo = stc::not_delegated   |  }
     }                                  |
     class C < B                        |
     {                                  |
       vtype foo = 0                    |
     }                                  |
*)
let a =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes [] } in

let x =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["foo", Std_Int] } in

let b =
  Scoop_Class { super = a;
		vtypes = create_vtypes [("delegatee_type", x);
					("foo", Stc_Not_delegated)] } in
let c =
  Scoop_Class { super = b;
		vtypes = create_vtypes ["foo", Stc_Abstract] } in

(* foo is abstract and tagged ``not delegated'' for B.  *)
assert (find b "foo" = Stc_Not_found);
(* Likewise for C.  *)
try ignore (find c "foo") with Scoop_exception "find: VT is abstract." -> ()
;;


(* *** Olena-like examples.

     class /image_entry/ < stc::none
     {
       vtype value_type = 0;
     }

     class image2d<int> < image_entry
     {
       vtypes value_type = int;
     }

     class /value_morpher/ < image_entry
     {
       vtype delegatee_type = 0;
       vtype value_type = stc::not_delegated;
     } 

     class value_cast<image2d<int>, float> < value_morpher
     {
       vtype delegatee_type = image2d<int>;
       vtype value_type = float;
     }
*)

let image_entry =
  Scoop_Class { super = Stc_None;
		vtypes = create_vtypes ["value_type", Stc_Abstract] } in
let image2d_int =
  Scoop_Class { super = image_entry;
		vtypes = create_vtypes ["value_type", Std_Int] } in
let value_morpher =
  Scoop_Class { super = image_entry;
		vtypes = create_vtypes [("delegatee_type", Stc_Abstract);
					("value_type", Stc_Not_delegated)]} in
let value_cast__image2d_int__float =
  Scoop_Class { super = value_morpher;
		vtypes = create_vtypes [("delegatee_type", image2d_int);
					("value_type", Std_Float)] } in

assert (find image2d_int "value_type" = Std_Int);
assert (find value_cast__image2d_int__float "value_type" = Std_Float);;
