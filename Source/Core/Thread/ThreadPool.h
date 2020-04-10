#pragma once

#include "Core/Thread/.Package.h"
#include "Core/String.h"
#include "Core/Exception.h"

namespace ThreadPoolInternal
{
class ThreadProxy
{
public:
    using WorkFunc = std::function<void()>;

    ThreadProxy()
    {
        thread = Memory::MakeUnique<std::thread>([this]() {
            this->Run();
        });
    }

    ~ThreadProxy()
    {
        Wait();

        startedCond.notify_one();

        thread->join();
    }

    bool IsRunning() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        return running;
    }

    String GetName() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        return name;
    }

    int32 GetID() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        return workID;
    }

    void Start(const String &newName, int32 id, WorkFunc func)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);

            name = newName;
            workID = id;
            worker = func;
            running = true;
        }
        startedCond.notify_one();
    }

    void Wait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (running)
            finishedCond.wait(lock);
    }

    void WaitByID(int32 id)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (running && id == workID)
            finishedCond.wait(lock);
    }

private:
    void Run()
    {
        while (true)
        {
            WorkFunc func = nullptr;

            {
                std::unique_lock<std::mutex> lock(mutex);
                if (!running)
                    startedCond.wait(lock);

                func = worker;
            }

            if (!func)
                return;
            func();

            {
                std::unique_lock<std::mutex> lock(mutex);
                running = false;
                worker = nullptr;
            }
            finishedCond.notify_all();
        }
    }

private:
    String name;
    class ThreadPool *pool;
    WorkFunc worker = nullptr;
    UPtr<std::thread> thread;
    mutable std::mutex mutex;
    std::condition_variable startedCond;
    std::condition_variable finishedCond;
    int32 workID = -1;
    bool running = false;
};
}

class ThreadPool
{
public:
    static constexpr int32 MAX_CAPACITY = 16;
    using ThreadProxy = ThreadPoolInternal::ThreadProxy;
    using WorkFunc = ThreadProxy::WorkFunc;

    class Handle
    {
    public:
        Handle(const SPtr<ThreadProxy> &proxy, int32 id) : proxy(proxy), workID(id)
        {
        }

        void Wait()
        {
            auto ptr = proxy.lock();
            if (ptr)
                ptr->WaitByID(workID);
        }

    private:
        WPtr<ThreadProxy> proxy;
        int32 workID;
    };

    ThreadPool(int32 initCapacity = MAX_CAPACITY)
    {
        if (initCapacity > MAX_CAPACITY)
        {
            CT_LOG(Warning, CT_TEXT("Init capacity cannot be greater than {0}."), MAX_CAPACITY);
        }

        capacity = initCapacity > MAX_CAPACITY ? MAX_CAPACITY : initCapacity;
    }

    ~ThreadPool()
    {
        StopAll();
    }

    const int32 Capacity() const
    {
        return capacity;
    }

    const int32 GetWorkingCount() const
    {
        int32 num = 0;

        std::unique_lock<std::mutex> lock(mutex);
        for (auto &e : proxies)
        {
            if (e->IsRunning())
                ++num;
        }
        return num;
    }

    const int32 GetIdleCount() const
    {
        int32 num = 0;

        std::unique_lock<std::mutex> lock(mutex);
        for (auto &e : proxies)
        {
            if (!e->IsRunning())
                ++num;
        }
        return num;
    }

    const int32 GetAllocatedCount() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        return proxies.Count();
    }

    Handle Execute(const String &name, WorkFunc func)
    {
        CT_CHECK(func != nullptr);

        std::unique_lock<std::mutex> lock(mutex);

        SPtr<ThreadProxy> proxy = GetAvailableProxyUnlocked();
        int32 id = nextWorkID++;
        proxy->Start(name, id, func);

        return Handle(proxy, id);
    }

    void StopAll()
    {
        std::unique_lock<std::mutex> lock(mutex);
        proxies.Clear();
    }

private:
    SPtr<ThreadProxy> GetAvailableProxyUnlocked()
    {
        for (auto &e : proxies)
        {
            if (!e->IsRunning())
                return e;
        }

        if (proxies.Count() < capacity)
        {
            proxies.Add(Memory::MakeShared<ThreadProxy>());
            return proxies.Last();
        }

        CT_EXCEPTION(Core, "Create thread failed, check max capacity of the pool.");
        return nullptr;
    }

private:
    int32 capacity;
    int32 nextWorkID = 0;
    Array<SPtr<ThreadProxy>> proxies;
    mutable std::mutex mutex;
};