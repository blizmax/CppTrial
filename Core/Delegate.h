#include "Core/.Package.h"
#include "Core/Array.h"
#include <functional>

template <typename T>
class Delegate;

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(Args...)>
{
public:
    template <typename ObjectType>
    using MemberFunc = ReturnType (ObjectType::*)(Args...);
    template <typename ObjectType>
    using ConstMemberFunc = ReturnType (ObjectType::*)(Args...) const;

    using Callable = std::function<ReturnType(Args...)>;

public:
    struct InnerData
    {
        Callable callable;
        bool alive;

        InnerData(Callable func) : alive(true)
        {
            callable = func;
        }

        template <typename ObjectType>
        InnerData(MemberFunc<ObjectType> func, ObjectType *obj) : alive(true)
        {
            using namespace std::placeholders;

            constexpr uint32 argNum = sizeof...(Args);
            static_assert(argNum < 6, "too many args!");

            if constexpr (argNum == 0)
            {
                callable = std::bind(func, obj);
            }
            else if constexpr (argNum == 1)
            {
                callable = std::bind(func, obj, _1);
            }
            else if constexpr (argNum == 2)
            {
                callable = std::bind(func, obj, _1, _2);
            }
            else if constexpr (argNum == 3)
            {
                callable = std::bind(func, obj, _1, _2, _3);
            }
            else if constexpr (argNum == 4)
            {
                callable = std::bind(func, obj, _1, _2, _3, _4);
            }
            else if constexpr (argNum == 5)
            {
                callable = std::bind(func, obj, _1, _2, _3, _4, _5);
            }
        }
    };

    class Handle
    {
    public:
        Handle(const SPtr<InnerData> &data) : data(data)
        {
        }

        void Off()
        {
            if (data)
                data->alive = false;
        }

    private:
        SPtr<InnerData> data;
    };

public:
    Delegate() = default;
    Delegate(const Delegate &) = delete;
    Delegate(Delegate &&) = delete;
    Delegate &operator=(const Delegate &) = delete;
    Delegate &operator=(Delegate &&) = delete;

    ~Delegate()
    {
        Clear();
    }

    bool IsEmpty() const
    {
        return chain.IsEmpty();
    }

    void Clear()
    {
        for (const auto &e : chain)
            e->alive = false;

        chain.Clear();
    }

    Handle On(Callable func)
    {
        auto innerData = Memory::MakeShared<InnerData>(func);
        chain.Add(innerData);
        return Handle(innerData);
    }

    template <typename ObjectType>
    Handle On(MemberFunc<ObjectType> func, ObjectType *obj)
    {
        auto innerData = Memory::MakeShared<InnerData>(func, obj);
        chain.Add(innerData);
        return Handle(innerData);
    }

    template <typename ObjectType>
    Handle On(ConstMemberFunc<ObjectType> func, ObjectType *obj)
    {
        return On((MemberFunc<ObjectType>)(func), obj);
    }

    void operator()(Args... args)
    {
        uint32 deadNum = 0;
        for (const auto &e : chain)
        {
            if (e->alive)
                e->callable(std::forward<Args>(args)...);
            else
                ++deadNum;
        }

        if (deadNum >= 5)
        {
            Array<SPtr<InnerData>> temp;
            for (const auto &e : chain)
            {
                if (e->alive)
                    temp.Add(e);
            }
            chain.Swap(temp);
        }
    }

private:
    Array<SPtr<InnerData>> chain;
};