#include "Core/.Package.h"
#include "Core/Array.h"
#include <functional>

template<typename T>
class Delegate;

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(Args...)>
{
public:
    template <typename OwnerType>
    using MemberFuncPtr = ReturnType (OwnerType::*)(Args...);
    using FuncPtr = ReturnType (*)(Args...);
    using WrappedFunc = std::function<ReturnType(Args...)>;

private:
    struct InnerData
    {
        void *objPtr = nullptr;
        void *funcPtr = nullptr;
        WrappedFunc wrappedFunc;

        InnerData(FuncPtr func)
        {
            funcPtr = reinterpret_cast<void *>(func);
            wrappedFunc = WrappedFunc(func);
        }

        InnerData(WrappedFunc func)
        {
            wrappedFunc = func;
        }

        template <typename OwnerType>
        InnerData(MemberFuncPtr<OwnerType> func, OwnerType *obj)
        {
            //funcPtr = reinterpret_cast<void *>(func);
            objPtr = reinterpret_cast<void *>(obj);

            using namespace std::placeholders;

            constexpr uint32 argNum = sizeof...(Args);
            static_assert(argNum < 6, "too much args!");

            if constexpr (argNum == 0)
            {
                wrappedFunc = std::bind(func, obj);
            }
            else if constexpr (argNum == 1)
            {
                wrappedFunc = std::bind(func, obj, _1);
            }
            else if constexpr (argNum == 2)
            {
                wrappedFunc = std::bind(func, obj, _1, _2);
            }
            else if constexpr (argNum == 3)
            {
                wrappedFunc = std::bind(func, obj, _1, _2, _3);
            }
            else if constexpr (argNum == 4)
            {
                wrappedFunc = std::bind(func, obj, _1, _2, _3, _4);
            }
            else if constexpr (argNum == 5)
            {
                wrappedFunc = std::bind(func, obj, _1, _2, _3, _4, _5);
            }
        }
    };

    SizeType Find(void *func) const
    {
        for (SizeType i = 0; i < data.Size(); ++i)
        {
            if (data[i].funcPtr == func)
            {
                return i;
            }
        }
        return INDEX_NONE;
    }

public:
    bool Exists(FuncPtr func) const
    {
        return Find(reinterpret_cast<void *>(func)) != INDEX_NONE;
    }

    // template<typename OwnerType>
    // bool Exists(MemberFuncPtr<OwnerType> func, OwnerType *obj) const
    // {
    //     auto index = Find(reinterpret_cast<void *>(func));
    //     if(index != INDEX_NONE && data[index].objPtr == obj)
    //     {
    //         return true;
    //     }
    //     return false;
    // }

    void Bind(FuncPtr func)
    {
        if (!Exists(func))
        {
            data.Add(InnerData(func));
        }
    }

    // TODO Try compare two member function ptr
    template <typename OwnerType>
    void Bind(MemberFuncPtr<OwnerType> func, OwnerType *obj)
    {
        // if (!Exists(func, obj))
        // {
        //     data.Add(InnerData(func, obj));
        // }
        data.Add(InnerData(func, obj));
    }

    // TODO Try compare two std::function object
    void Bind(WrappedFunc func)
    {
        data.Add(InnerData(func));
    }

    void Unbind(FuncPtr func)
    {
        auto index = Find(func);
        if(index != INDEX_NONE)
        {
            data.Remove(index);
        }
    }

    template <typename OwnerType>
    void Unbind(OwnerType* obj)
    {
        for(SizeType i = 0; i < data.Size();)
        {
            if(data[i].objPtr == obj)
            {
                data.Remove(i);
            }
            else
            {
                ++i;
            }
        }
    }

    void Clear()
    {
        data.Clear();
    }

    void operator()(Args... args)
    {
        for(const auto& e : data)
        {
            e.wrappedFunc(std::forward<Args>(args)...);
        }
    }

private:
    Array<InnerData> data;
};