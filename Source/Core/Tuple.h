#pragma once

#include "Core/Template.h"

namespace TupleInternal
{
template <SizeType Index, typename... Types>
class TupleImpl;

template <SizeType Index, typename T>
class TupleImpl<Index, T>
{
public:
    using HeadType = T;

    HeadType head;

    constexpr TupleImpl() = default;

    template <typename _T>
    constexpr explicit TupleImpl(_T &&head)
        : head(std::forward<_T>(head))
    {
    }

    constexpr HeadType &Head()
    {
        return head;
    }

    constexpr const HeadType &Head() const
    {
        return head;
    }

    void Swap(TupleImpl &other) noexcept
    {
        std::swap(head, other.head);
    }
};

#ifdef CT_TUPLE_INHERIT
template <SizeType Index, typename T, typename... Others>
class TupleImpl<Index, T, Others...> : public TupleImpl<Index + 1, Others...>
{
public:
    using HeadType = T;
    using TailType = TupleImpl<Index + 1, Others...>;

    HeadType head;

    constexpr TupleImpl() = default;

    template <typename _T, typename... _Others>
        requires sizeof...(Others) == sizeof...(_Others) constexpr TupleImpl(_T &&head, _Others &&... tail)
        : head(std::forward<_T>(head)),
    TailType(std::forward<_Others>(tail)...)
    {
    }

    constexpr HeadType &Head()
    {
        return head;
    }

    constexpr const HeadType &Head() const
    {
        return head;
    }

    constexpr TailType &Tail()
    {
        return *this;
    }

    constexpr const TailType &Tail() const
    {
        return *this;
    }

    void Swap(Tuple &other) noexcept
    {
        std::swap(head, other.head);
        Tail().Swap(other.Tail());
    }
};

#else

template <SizeType Index, typename T, typename... Others>
class TupleImpl<Index, T, Others...>
{
public:
    using HeadType = T;
    using TailType = TupleImpl<Index + 1, Others...>;

    HeadType head;
    TailType tail;

    constexpr TupleImpl() = default;

    template <typename _T, typename... _Others>
        requires sizeof...(Others) == sizeof...(_Others) constexpr TupleImpl(_T &&head, _Others &&... tail)
        : head(std::forward<_T>(head)),
    tail(std::forward<_Others>(tail)...)
    {
    }

    constexpr HeadType &Head()
    {
        return head;
    }

    constexpr const HeadType &Head() const
    {
        return head;
    }

    constexpr TailType &Tail()
    {
        return tail;
    }

    constexpr const TailType &Tail() const
    {
        return tail;
    }

    void Swap(TupleImpl &other) noexcept
    {
        std::swap(head, other.head);
        Tail().Swap(other.Tail());
    }
};
#endif

template <SizeType Index, SizeType _Index, typename _T, typename... _Others>
constexpr decltype(auto) Get(const TupleInternal::TupleImpl<_Index, _T, _Others...> &tuple)
{
    if constexpr (Index == _Index)
        return tuple.Head();
    else
        return Get<Index, _Index + 1, _Others...>(tuple.Tail());
}

template <SizeType Index, SizeType _Index, typename _T, typename... _Others>
constexpr decltype(auto) Get(TupleInternal::TupleImpl<_Index, _T, _Others...> &tuple)
{
    if constexpr (Index == _Index)
        return tuple.Head();
    else
        return Get<Index, _Index + 1, _Others...>(tuple.Tail());
}
}

template <typename... Types>
class Tuple : public TupleInternal::TupleImpl<0, Types...>
{
public:
    constexpr Tuple() = default;

    template <typename... Args>
    constexpr Tuple(Args &&... args)
        : TupleInternal::TupleImpl<0, Types...>(std::forward<Args>(args)...)
    {
    }

    template <SizeType Index>
    constexpr typename std::tuple_element<Index, Tuple<Types...>>::type &get()
    {
        return TupleInternal::Get<Index, 0, Types...>(*this);
    }

    template <SizeType Index>
    constexpr const typename std::tuple_element<Index, Tuple<Types...>>::type &get() const
    {
        return TupleInternal::Get<Index, 0, Types...>(*this);
    }
};

template <typename... Args>
constexpr Tuple<std::decay_t<Args>...> MakeTuple(Args &&... args)
{
    return Tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
}

namespace std
{
template <typename... Types>
struct tuple_size<Tuple<Types...>> : public integral_constant<SizeType, sizeof...(Types)>
{
};

template <SizeType Index, typename T, typename... Others>
struct tuple_element<Index, Tuple<T, Others...>>
    : tuple_element<Index - 1, Tuple<Others...>>
{
};

template <typename T, typename... Others>
struct tuple_element<0, Tuple<T, Others...>>
{
    using type = T;
};

}
