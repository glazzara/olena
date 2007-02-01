
namespace stc
{

  class abstract;

  template <typename Exact>
  class any
  {
  public:
    Exact& exact() { return *(Exact*)(void*)this; }
    const Exact& exact() const  { return *(const Exact*)(const void*)this; }
  };
  
}

#define stc_typename(Type) typedef typename vtypes<Exact>::Type Type
#define stc_using(Type)    typedef typename Super::Type Type


#define stc_equip_namespace(Namespace)		\
						\
namespace Namespace				\
{						\
  template <typename T> struct vtypes;		\
						\
  /* Any. */					\
						\
  template <typename Exact> class Any;		\
						\
  template <typename Exact>			\
  struct vtypes< Any<Exact> >			\
  {						\
    typedef Exact exact;			\
  };						\
						\
  template <typename Exact>			\
  class Any : public stc::any<Exact>		\
  {						\
  };						\
						\
}						\
						\
struct end_with_semicolumn
