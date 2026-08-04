#include "stdafx.h"
#include "PdhQuery.h"
#include <cmath>
#include <cstddef>

CPdhQuery::CPdhQuery(LPCTSTR _fullCounterPath)
    : fullCounterPath(_fullCounterPath)
{
    Initialize();
}

CPdhQuery::~CPdhQuery()
{
    //关闭查询
    PdhCloseQuery(query);
}

bool CPdhQuery::Initialize()
{
    if (isInitialized)
        return true;

    PDH_STATUS status;
    //打开查询
    status = PdhOpenQuery(NULL, NULL, &query);
    if (status != ERROR_SUCCESS)
        return false;

    //添加计数器
    status = PdhAddCounter(query, fullCounterPath.GetString(), NULL, &counter);
    //先调用PdhAddCounter，如果失败使用PdhAddEnglishCounter再试一次
    if (status != ERROR_SUCCESS)
    {
        status = PdhAddEnglishCounter(query, fullCounterPath.GetString(), NULL, &counter);
        if (status != ERROR_SUCCESS)
        {
            PdhCloseQuery(query);
            query = nullptr;
            return false;
        }
    }

    //初始化计数器
    PdhCollectQueryData(query);
    isInitialized = true;
    return true;
}

bool CPdhQuery::QueryValue(double& value)
{
    if (!isInitialized)
        return false;

    //更新数据
    PdhCollectQueryData(query);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    PDH_STATUS status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
    if (status != ERROR_SUCCESS)
    {
        return false;
    }
    value = pdhValue.doubleValue;
    return true;
}

bool CPdhQuery::QueryValues(std::vector<CounterValueItem>& values)
{
    values.clear();
    if (!isInitialized)
        return false;

    // Refresh the wildcard counter, then size and fetch its current instance
    // array. Instances can change between the two PDH calls during dock/hotplug,
    // so retry a bounded number of times with RAII-backed storage.
    if (PdhCollectQueryData(query) != ERROR_SUCCESS)
        return false;

    DWORD buffer_size = 0;
    DWORD item_count = 0;
    PDH_STATUS status =
        PdhGetFormattedCounterArray(counter, PDH_FMT_DOUBLE, &buffer_size, &item_count, nullptr);
    if (status != PDH_MORE_DATA || buffer_size == 0)
        return false;

    constexpr int max_resize_attempts = 3;
    constexpr DWORD max_counter_buffer_size = 16u * 1024u * 1024u;
    for (int attempt = 0; attempt < max_resize_attempts; ++attempt)
    {
        if (buffer_size > max_counter_buffer_size)
            return false;

        const size_t aligned_count =
            (buffer_size + sizeof(std::max_align_t) - 1) / sizeof(std::max_align_t);
        std::vector<std::max_align_t> storage(aligned_count);
        auto* items = reinterpret_cast<PDH_FMT_COUNTERVALUE_ITEM*>(storage.data());

        DWORD supplied_size = buffer_size;
        item_count = 0;
        status = PdhGetFormattedCounterArray(
            counter, PDH_FMT_DOUBLE, &supplied_size, &item_count, items);
        if (status == PDH_MORE_DATA)
        {
            buffer_size = supplied_size;
            continue;
        }
        if (status != ERROR_SUCCESS)
            return false;

        for (DWORD index = 0; index < item_count; ++index)
        {
            const auto& formatted_value = items[index].FmtValue;
            if (formatted_value.CStatus != PDH_CSTATUS_VALID_DATA &&
                formatted_value.CStatus != PDH_CSTATUS_NEW_DATA)
            {
                continue;
            }
            if (!std::isfinite(formatted_value.doubleValue) || items[index].szName == nullptr)
                continue;

            CounterValueItem value_item;
            value_item.name = items[index].szName;
            value_item.value = formatted_value.doubleValue;
            values.push_back(value_item);
        }
        return !values.empty();
    }
    return false;
}
