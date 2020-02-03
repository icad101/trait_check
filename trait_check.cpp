
///----------------
static_assert( all<std::true_type, std::true_type, std::true_type>::value );
static_assert( all<std::true_type, std::false_type, std::true_type>::value == false );

static_assert( any<std::true_type, std::false_type, std::true_type>::value );
static_assert( any<std::false_type, std::false_type, std::false_type>::value == false );

static_assert( xany<std::true_type, std::true_type, std::true_type>::value == false );
static_assert( xany<std::true_type, std::false_type, std::true_type>::value );
static_assert( xany<std::false_type, std::false_type, std::false_type>::value == false );

static_assert( none<std::false_type, std::false_type, std::false_type>::value );
static_assert( none<std::false_type, std::true_type, std::false_type>::value == false );

static_assert( nonexany<std::true_type, std::true_type, std::true_type>::value == false );
static_assert( nonexany<std::true_type, std::false_type, std::true_type>::value );
static_assert( nonexany<std::false_type, std::false_type, std::false_type>::value );

static_assert( noneall<std::true_type, std::true_type, std::true_type>::value );
static_assert( noneall<std::true_type, std::false_type, std::true_type>::value == false );
static_assert( noneall<std::false_type, std::false_type, std::false_type>::value );

static_assert( noneall2<std::true_type, std::true_type, std::true_type>::value );
static_assert( noneall2<std::true_type, std::false_type, std::true_type>::value == false );
static_assert( noneall2<std::false_type, std::false_type, std::false_type>::value );


///----------------
template <typename T0, typename T1, typename T2, typename T3 = int, typename T4 = int>
using def_arg = std::true_type;

static_assert( min_arity_v<def_arg> == 3 );

static_assert( min_arity_v<std::is_same> == 2 );


///----------------
static_assert( check_f<_qualified, std::type_identity_t, int const>() );
static_assert( check_f<_qualified, std::type_identity_t, int&>() );
static_assert( check_f<_qualified, std::type_identity_t, int*>() );
static_assert( check_f<_qualified, std::type_identity_t, int>() == false );
//static_assert( check_f<_qualified, unqualify, int>() == false ); //expected error

static_assert( check_f<_unqualified, std::type_identity_t, int>() );
static_assert( check_f<_unqualified, std::type_identity_t, int const>() == false );
static_assert( check_f<_unqualified, std::type_identity_t, int&>() == false );
static_assert( check_f<_unqualified, std::type_identity_t, int*>() == false );
//static_assert( check_f<_unqualified, unqualify, int*>() ); //expected error

//static_assert( check_f<std::is_same, std::type_identity_t>() ); //expected constraints error
//static_assert( check_f<std::is_same, std::type_identity_t, int>() ); //expected error
//static_assert( check_f<std::is_same, std::type_identity_t, int, int, int>() ); //expected error
static_assert( check_f<std::is_same, std::type_identity_t, int, int>() );
static_assert( check_f<std::is_same, std::type_identity_t, int, int&>() == false );
static_assert( check_f<std::is_same, unqualify, int, int&>() );
static_assert( check_f<std::is_same, unqualify, int, float&>() == false );

//----------------
static_assert( check_idt_f<_qualified, int const>() );
//static_assert( check_unq_f<_qualified, int const>() ); //expected error

static_assert( check_idt_f<std::is_same, int, int&>() == false );
static_assert( check_unq_f<std::is_same, int, int&>() );

//static_assert( check_idt_f<std::negation, std::is_same, int, float>() );

//----------------
static_assert( check_idt_f<all, _qualified, int const, float&, bool*>() );
static_assert( check_idt_f<all, _qualified, int const, float, bool*>() == false );

static_assert( check_idt_f<any, _qualified, int, float, bool*>() );
static_assert( check_idt_f<any, _qualified, int, float, bool>() == false );

static_assert( check_idt_f<xany, _qualified, int, float, bool*>() );
static_assert( check_idt_f<xany, _qualified, int, float, bool>() == false );
static_assert( check_idt_f<xany, _qualified, int const, float&, bool*>() == false );

static_assert( check_idt_f<none, _qualified, int, float, bool*>() == false );
static_assert( check_idt_f<none, _qualified, int, float, bool>() );
//none and std::negation are usable interchangeably
static_assert( check_idt_f<std::negation, _qualified, int>() );
static_assert( check_idt_f<std::negation, _qualified, int const>() == false );
//static_assert( check_idt_f<std::negation, _qualified, int, int>() ); //expected error

static_assert( check_idt_f<nonexany, _qualified, int, float, bool*>() );
static_assert( check_idt_f<nonexany, _qualified, int, float, bool>() );
static_assert( check_idt_f<nonexany, _qualified, int const, float&, bool*>() == false );

static_assert( check_idt_f<noneall, _qualified, int, float, bool*>() == false );
static_assert( check_idt_f<noneall, _qualified, int, float, bool>() );
static_assert( check_idt_f<noneall, _qualified, int const, float&, bool*>() );

static_assert( check_idt_f<noneall2, _qualified, int, float, bool*>() == false );
static_assert( check_idt_f<noneall2, _qualified, int, float, bool>() );
static_assert( check_idt_f<noneall2, _qualified, int const, float&, bool*>() );

//static_assert( check_idt_f<all, std::is_same, int, int&, int>() == false ); //expected error (this version takes only traits of arity 1 )


//static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, int>() ); //expected error (too few args)
//static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, int, int>() == false ); //expected error
//static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, int, int, int, int>() == false ); //expected error
static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, int, int, int>() );
static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, float, int, int>() );
static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, int, float, float, int>() == false );
static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, float, int, int, int, int, float>() );
static_assert( check_idt_f<any, std::is_same, PackRead::contiguous, float, int, int, float, int, float>() == false );

static_assert( check_idt_f<any, std::is_same, PackRead::linear, int, int, int>() );
static_assert( check_idt_f<any, std::is_same, PackRead::linear, int, float, int>() == false );
static_assert( check_idt_f<any, std::is_same, PackRead::linear, int, float, float, int>() );

static_assert( check_idt_f<any, std::is_same, PackRead::pivot_first, int, float, float>() == false );
static_assert( check_idt_f<any, std::is_same, PackRead::pivot_first, int, float, float, int, float>() );

static_assert( check_idt_f<none, std::is_same, PackRead::pivot_first, int, float, float, int, float>() == false );
//static_assert( check_idt_f<std::negation, std::is_same, PackRead::pivot_first, int, float, float, int, float>() ); //expected constraints error
