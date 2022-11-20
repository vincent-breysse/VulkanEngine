#pragma once

#include <shared_mutex>
#include <unordered_map>

template <typename KeyT, typename ElemT>
struct VexThreadedHashMap
{
    std::unordered_map<KeyT, ElemT> container;
    mutable std::shared_mutex mutex;
};

template <typename KeyT, typename ElemT>
const ElemT& VexThreadedHashMapAt(const VexThreadedHashMap<KeyT, ElemT>& map, const KeyT& key)
{
    std::shared_lock lock(map.mutex);
    return map.container.at(key);
}

template <typename KeyT, typename ElemT>
void VexThreadedHashMapRemove(VexThreadedHashMap<KeyT, ElemT>& map, const KeyT& key)
{
    std::unique_lock lock(map.mutex);
    map.container.erase(key);
}

template <typename KeyT, typename ElemT>
void VexThreadedHashMapRemoveRange(VexThreadedHashMap<KeyT, ElemT>& map, uint32_t numKey, const KeyT* keys)
{
    std::unique_lock lock(map.mutex);

    for (uint32_t i = 0; i < numKey; ++i)
    {
        map.container.erase(keys[i]);
    }
}

template <typename KeyT, typename ElemT>
ElemT* VexThreadedHashMapTryAdd(VexThreadedHashMap<KeyT, ElemT>& map, const KeyT& key)
{
    std::unique_lock lock(map.mutex);
    auto pair = map.container.try_emplace(key);
    return pair.second ? &pair.first->second : nullptr;
}

template <typename KeyT, typename ElemT, typename FnInit>
void VexThreadedHashMapTryAddRange(VexThreadedHashMap<KeyT, ElemT>& map, uint32_t numKey, const KeyT* keys, const FnInit& fnInit)
{
    std::unique_lock lock(map.mutex);

    for (uint32_t i = 0; i < numKey; ++i)
    {
        const KeyT& key = keys[i];

        auto pair = map.container.try_emplace(key);

        if (pair.second)
            fnInit(key, pair.first->second);
    }
}