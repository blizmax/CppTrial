#pragma once

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

    static SPtr<QueryPool> Create(const QueryPoolDesc &desc);

private:
    QueryPoolDesc desc;
};