#pragma once

#include "Core/.Package.h"
#include "Core/Array.h"
#include "Core/Thread/.Package.h"
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

        InnerData(const Callable &func) : alive(true)
        {
            callable = func;
        }

        template <typename ObjectType>
        InnerData(const MemberFunc<ObjectType> &func, ObjectType *obj) : alive(true)
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
        Handle() = default;
        Handle(const Handle &) = default;
        Handle(Handle &&) = default;
        Handle &operator=(const Handle &) = default;
        Handle &operator=(Handle &&) = default;
        ~Handle() = default;

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
        std::unique_lock<std::recursive_mutex> lock(mutex);

        return chain.IsEmpty();
    }

    int32 Count() const
    {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        return chain.Count();
    }

    void Clear()
    {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto &e : chain)
            e->alive = false;

        chain.Clear();
    }

    Handle On(const Callable &func)
    {
        auto innerData = Memory::MakeShared<InnerData>(func);

        std::unique_lock<std::recursive_mutex> lock(mutex);
        chain.Add(innerData);
        return Handle(innerData);
    }

    template <typename ObjectType>
    Handle On(const MemberFunc<ObjectType> &func, ObjectType *obj)
    {
        auto innerData = Memory::MakeShared<InnerData>(func, obj);

        std::unique_lock<std::recursive_mutex> lock(mutex);

        chain.Add(innerData);
        return Handle(innerData);
    }

    template <typename ObjectType>
    Handle On(const ConstMemberFunc<ObjectType> &func, ObjectType *obj)
    {
        return On((MemberFunc<ObjectType>)(func), obj);
    }

    template <typename... Pargs>
    Delegate &operator+=(Pargs &&... args)
    {
        On(std::forward<Pargs>(args)...);
        return *this;
    }

    Delegate &operator-=(Callable &func)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (auto &e : chain)
        {
            if (e->callable == func)
            {
                e->alive = false;
                break;
            }
        }
        return *this;
    }

    void operator()(Args... args)
    {
        uint32 deadNum = 0;

        std::unique_lock<std::recursive_mutex> lock(mutex);

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
    mutable std::recursive_mutex mutex;
};