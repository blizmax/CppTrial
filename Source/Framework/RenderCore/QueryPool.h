#pragma once

#include "Core/Queue.h"
#include "RenderCore/.Package.h"

struct QueryPoolDesc
{
    QueryType queryType;
    int32 count;
};

class QueryPool
{
public:
    QueryPool(const QueryPoolDesc &desc)
        : desc(desc)
    {
    }

    virtual ~QueryPool() = default;

    const QueryPoolDesc &GetDesc() const
    {
        return desc;
    }

    QueryType GetQueryType() const
    {
        return desc.queryType;
    }

    int32 Allocate()
    {
        if (freeQueries.IsEmpty())
        {
            CT_CHECK(nextQueryID < desc.count);
            return nextQueryID++;
        }

        int32 queryID = freeQueries.Peek();
        freeQueries.Pop();
        return queryID;
    }

    void Release(int32 queryID)
    {
        CT_CHECK(queryID >= 0 && queryID < desc.count);
        freeQueries.Push(queryID);
    }

    static SPtr<QueryPool> Create(const QueryPoolDesc &desc);

protected:
    QueryPoolDesc desc;
    int32 nextQueryID = 0;
    Queue<int32> freeQueries;
};