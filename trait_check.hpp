#include <type_traits>
#include <utility> //for index_sequence
#include <tuple> //for tuple_element

///------------------
template <typename T> using unqualify = std::remove_const_t< std::remove_pointer_t<std::remove_reference_t<T>> >;

template <typename T> using _qualified = std::disjunction<
    std::is_reference<T>,
    std::is_pointer<T>,
    std::is_const<T> >;
template <typename T> constexpr bool const is_qualified_v = _qualified<T>::value;

template <typename T> using _unqualified = std::negation<_qualified<T>>;

//------------------
#define TmplTsT template <typename...> typename
#define TmplTT template <typename> typename

template <TmplTsT, TmplTsT>
struct is_tmpl_same_tmpl : std::false_type {};
template <TmplTsT TmplT>
struct is_tmpl_same_tmpl<TmplT, TmplT> : std::true_type {};

//----------------
template <typename... Ts> requires sizeof...(Ts) >= 2 using all  = std::conjunction<Ts...>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using any  = std::disjunction<Ts...>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using xany = std::conjunction<any<Ts...>, std::negation<all<Ts...>>>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using none = std::negation<any<Ts...>>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using nonexany = std::disjunction<none<Ts...>, xany<Ts...>>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using noneall = std::disjunction<none<Ts...>, all<Ts...>>;
template <typename... Ts> requires sizeof...(Ts) >= 2 using noneall2 = std::negation<xany<Ts...>>;

//----------------
template <TmplTsT Trait, typename... Ts>
constexpr auto const min_arity_v = min_arity_v<Trait, Ts..., int>;
template <TmplTsT Trait, typename... Ts> requires requires { Trait<Ts...>{}; }
constexpr auto const min_arity_v<Trait, Ts...> = sizeof...(Ts);


///-----------------
template <TmplTsT Trait, TmplTT QualifOp, typename... Args>
    requires sizeof...(Args) >= 1
constexpr auto check_f() noexcept
{
    static_assert( not ((is_tmpl_same_tmpl<Trait, _qualified>::value or is_tmpl_same_tmpl<Trait, _unqualified>::value)
                        and is_tmpl_same_tmpl<QualifOp, unqualify>::value) );
    constexpr auto const tma = min_arity_v<Trait>;
    static_assert( tma > 0 );
    static_assert( sizeof...(Args) == tma, "mismatching trait arity and arg count" );
    return Trait<QualifOp<Args>...>::value;
}
//Convenience shortcut functions
template <TmplTsT Trait, typename... Args>
constexpr auto check_idt_f() noexcept { return check_f<Trait, std::type_identity_t, Args...>(); }
template <TmplTsT Trait, typename... Args> requires (is_qualified_v<Args> or ...)
constexpr auto check_unq_f() noexcept { return check_f<Trait, unqualify, Args...>(); }

//-----------------
template <TmplTsT Junction, TmplTsT Trait, TmplTT QualifOp, typename... Args>
constexpr auto check_f() noexcept
{
    if constexpr (is_tmpl_same_tmpl<Junction, std::negation>::value) {
        static_assert( sizeof...(Args) == 1, "use variadic logical metafunction for multiple args" );
        return not check_f<Trait, QualifOp, Args...>();
    }
    else {
        static_assert( sizeof...(Args) >= 2 );

        static_assert( not ((is_tmpl_same_tmpl<Trait, _qualified>::value or is_tmpl_same_tmpl<Trait, _unqualified>::value)
                            and is_tmpl_same_tmpl<QualifOp, unqualify>::value) );
        static_assert( min_arity_v<Trait> == 1 );

        using Tup = std::tuple<QualifOp<Args>...>;
        return [] <std::size_t... Is> (std::index_sequence<Is...>&&) constexpr noexcept {
            return Junction< Trait<std::tuple_element_t<Is, Tup>>... >::value;
        }(std::index_sequence_for<Args...>{});
    }
}
template <TmplTsT Junction, TmplTsT Trait, typename... Args>
constexpr auto check_idt_f() noexcept { return check_f<Junction, Trait, std::type_identity_t, Args...>(); }
template <TmplTsT Junction, TmplTsT Trait, typename... Args> requires (is_qualified_v<Args> or ...)
constexpr auto check_unq_f() noexcept { return check_f<Junction, Trait, unqualify, Args...>(); }

//-----------------
enum class PackRead { contiguous, linear, pivot_first };

template <TmplTsT, typename, PackRead, std::size_t, typename>
struct TraitRes;

template <TmplTsT Trait, typename Tup, std::size_t IsOut, std::size_t... IsIn>
struct TraitRes<Trait, Tup, PackRead::contiguous, IsOut, std::index_sequence<IsIn...>> final {
    using type = Trait<std::tuple_element_t<sizeof...(IsIn) * IsOut + IsIn, Tup>...>; };

template <TmplTsT Trait, typename Tup, std::size_t IsOut, std::size_t... IsIn>
struct TraitRes<Trait, Tup, PackRead::linear, IsOut, std::index_sequence<IsIn...>> final {
    using type = Trait<std::tuple_element_t<IsOut + IsIn, Tup>...>; };

template <TmplTsT Trait, typename Tup, std::size_t IsOut, std::size_t... IsIn>
struct TraitRes<Trait, Tup, PackRead::pivot_first, IsOut, std::index_sequence<IsIn...>> final {
    using type = Trait<std::tuple_element_t<0, Tup>, std::tuple_element_t<IsOut + 1 + IsIn, Tup>...>; };


template <TmplTsT Junction, TmplTsT Trait, PackRead pack_read, TmplTT QualifOp, typename... Args>
    requires sizeof...(Args) >= 3 and not is_tmpl_same_tmpl<Junction, std::negation>::value
constexpr auto check_f() noexcept
{
    constexpr auto const count_trait_ts_v = min_arity_v<Trait>;
    static_assert( count_trait_ts_v > 1 );
    static_assert( count_trait_ts_v < sizeof...(Args) );

    using Tup = std::tuple<QualifOp<Args>...>;
    using InnerISeq = std::make_index_sequence< count_trait_ts_v - (pack_read == PackRead::pivot_first) >;

    return [] <std::size_t... IsOut> (std::index_sequence<IsOut...>&&) constexpr noexcept {
        return Junction< typename TraitRes<Trait, Tup, pack_read, IsOut, InnerISeq>::type... >::value;
    }( std::make_index_sequence< [count_trait_ts_v] () constexpr noexcept {
            if constexpr (pack_read == PackRead::contiguous) {
                static_assert( sizeof...(Args) % count_trait_ts_v == 0 ); //arg count must be a multiple of traits arity
                return sizeof...(Args) / count_trait_ts_v;
            } else
                return sizeof...(Args) - count_trait_ts_v + 1;
            }()
        >{}
    );
}
template <TmplTsT Junction, TmplTsT Trait, PackRead pack_read, typename... Args>
constexpr auto check_idt_f() noexcept { return check_f<Junction, Trait, pack_read, std::type_identity_t, Args...>(); }
template <TmplTsT Junction, TmplTsT Trait, PackRead pack_read, typename... Args> requires (is_qualified_v<Args> or ...)
constexpr auto check_unq_f() noexcept { return check_f<Junction, Trait, pack_read, unqualify, Args...>(); }
