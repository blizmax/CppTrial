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
        bool enabled;

        InnerData(Callable func) : enabled(true)
        {
            callable = func;
        }

        template <typename ObjectType>
        InnerData(MemberFunc<ObjectType> func, ObjectType *obj) : enabled(true)
        {
            using namespace std::placeholders;

            constexpr uint32 argNum = sizeof...(Args);
            static_assert(argNum < 6, "too much args!");

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

        void Disable()
        {
            enabled = false;
        }

        bool IsEnabled() const
        {
            return enabled;
        }
    };

    class Handle
    {
    public:
        Handle(const SPtr<InnerData> & data) : data(data)
        {
        }

        void Off()
        {
            if(data)
                data->Disable();
        }

    private:
        SPtr<InnerData> data;
    };

public:
    bool IsEmpty() const
    {
        return chain.Size() == 0;
    }

    void Clear()
    {
        for (const auto &e : chain)
            e->Disable();

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
        //TODO Clean the chain if contains too many holes
        for (const auto &e : chain)
        {
            if (e->IsEnabled())
                e->callable(std::forward<Args>(args)...);
        }
    }

private:
    Array<SPtr<InnerData>> chain;
};