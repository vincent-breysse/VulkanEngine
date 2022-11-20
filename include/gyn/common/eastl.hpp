#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/config.hpp>
#include <gyn/common/error.hpp>
#include <gyn/common/misc.hpp>

namespace Gyn
{
    template <typename T, typename... Args>
    inline UniquePtr<T> NewUnique(Args&&... args)
    {
        return UniquePtr<T>(new T(eastl::forward<Args>(args)...));
    }

    template <typename TRet, typename TNew, typename... Args>
    inline UniquePtr<TRet> NewUnique(Args&&... args)
    {
        return UniquePtr<TRet>(new TNew(eastl::forward<Args>(args)...));
    }

    template <typename T, typename... Args>
    SharedPtr<T> NewShared(Args&&... args)
    {
        // allocate with the default allocator.
        return eastl::allocate_shared<T>(EASTL_SHARED_PTR_DEFAULT_ALLOCATOR, eastl::forward<Args>(args)...);
    }

    template <typename T>
    ea::span<byte> AsByteSpan(T& item)
    {
        auto* ptr = reinterpret_cast<byte*>(&item);
        const auto size = sizeof(T);

        return ea::span(ptr, size);
    }

    template <typename T>
    ea::span<const byte> AsByteSpan(const T& item)
    {
        const auto* ptr = reinterpret_cast<const byte*>(&item);
        const auto size = sizeof(T);

        return ea::span(ptr, size);
    }

    template <typename T>
    ea::span<const byte> ToByteSpan(ea::span<const T> data)
    {
        const auto* ptr = reinterpret_cast<const byte*>(data.data());
        const auto size = data.size() * sizeof(T);

        return ea::span(ptr, size);
    }

    template <typename T>
    ea::span<const T> SpanCast(ea::span<const byte> byteSpan)
    {
        const auto uPtr = reinterpret_cast<uintptr>(byteSpan.data());

        GYN_ASSERT_MSG((byteSpan.size() % sizeof(T)) == 0, "Wrong cast");
        GYN_ASSERT_MSG((uPtr % alignof(T)) == 0, "Trying to cast a misaligned byte span to a typed span");

        const auto typedPtr = reinterpret_cast<const T*>(byteSpan.data());
        const auto typeSize = byteSpan.size() / sizeof(T);

        return ea::span<const T>(typedPtr, typeSize);
    }

    template <typename T>
    ea::span<T> SpanCast(ea::span<byte> byteSpan)
    {
        ea::span<const T> constTypedSpan = SpanCast<T>(ea::span<const byte>(byteSpan));
        T* p = const_cast<T*>(constTypedSpan.data());

        return ea::span<T>(p, constTypedSpan.size());
    }

    template <typename T, typename Allocator = EASTLAllocatorType>
    struct EAVectorData
    {
        T* mpBegin{};
        T* mpEnd{};
        ea::compressed_pair<T*, Allocator>  mCapacityAllocator{};
    };

    template <typename T, typename Allocator>
    EAVectorData<T*, Allocator> ConstCast(const EAVectorData<const T*, Allocator>& dataConst)
    {
        EAVectorData<T*> data{};
        data.mpBegin = const_cast<T**>(dataConst.mpBegin);
        data.mpEnd = const_cast<T**>(dataConst.mpEnd);
        data.mCapacityAllocator = { const_cast<T**>(dataConst.mCapacityAllocator.first()), dataConst.mCapacityAllocator.second() };

        return data;
    }

    template <typename T>
    ea::span<T> ConstCast(ea::span<const T> span)
    {
        return ea::span<T>(const_cast<T*>(span.data()), span.size());
    }

    template <typename T>
    SharedPtr<const T>& AsConst(SharedPtr<T>& x)
    {
        return reinterpret_cast<SharedPtr<const T>&>(x);
    }

    template <typename T>
    const SharedPtr<const T>& AsConst(const SharedPtr<T>& x)
    {
        return reinterpret_cast<const SharedPtr<const T>&>(x);
    }

    template <typename T>
    ea::vector<SharedPtr<const T>>& AsConst(ea::vector<SharedPtr<T>>& x)
    {
        return reinterpret_cast<ea::vector<SharedPtr<const T>>&>(x);
    }

    template <typename T>
    const ea::vector<SharedPtr<const T>>& AsConst(const ea::vector<SharedPtr<T>>& x)
    {
        return reinterpret_cast<const ea::vector<SharedPtr<const T>>&>(x);
    }

    template <typename T, typename Allocator>
    EAVectorData<T, Allocator> ReleaseVector(ea::vector<T, Allocator>& vector)
    {
        EAVectorData<T, Allocator> data{};
        data.mpBegin = vector.mpBegin;
        data.mpEnd = vector.mpEnd;
        data.mCapacityAllocator = vector.mCapacityAllocator;

        vector.mpBegin = nullptr;
        vector.mpEnd = nullptr;
        vector.mCapacityAllocator = { NULL, Allocator(EASTL_VECTOR_DEFAULT_NAME) };

        return data;
    }

    template <typename T, typename Allocator>
    ea::vector<T, Allocator> CreateVector(const EAVectorData<T, Allocator>& data)
    {
        ea::vector<T, Allocator> vector{};
        vector.mpBegin = data.mpBegin;
        vector.mpEnd = data.mpEnd;
        vector.mCapacityAllocator = data.mCapacityAllocator;

        return vector;
    }

    template <typename T>
    ea::vector<T> CopyToVector(ea::span<const T> span)
    {
        return ea::vector<T>(span.begin(), span.end());
    }

    template <typename T, typename Allocator>
    ea::vector<T*, Allocator> ConstCastMove(ea::vector<const T*, Allocator>& vector)
    {
        return CreateVector(ConstCast(ReleaseVector(vector)));
    }

    template <typename T>
    ea::span<const T* const> ToSpanConst(ea::vector<T*>& vector)
    {
        return ea::span(const_cast<const T* const*>(vector.data()), vector.size());
    }

    template <typename T>
    ea::span<const T* const> ToSpanConst(const ea::vector<T*>& vector)
    {
        return ea::span(const_cast<const T* const*>(vector.data()), vector.size());
    }

    template <typename T>
    ea::span<const UniquePtr<const T>> ToSpanConst(ea::span<const UniquePtr<T>> span)
    {
        const UniquePtr<T>* p = span.data();
        return { reinterpret_cast<const UniquePtr<const T>*>(p), span.size() };
    }

    template <typename T>
    ea::span<const SharedPtr<const T>> ToSpanConst(ea::span<const SharedPtr<T>> span)
    {
        const SharedPtr<T>* p = span.data();
        return { reinterpret_cast<const SharedPtr<const T>*>(p), span.size() };
    }

    template <typename T, typename U>
    ea::span<const UniquePtr<const T>> SpanDownCast(ea::span<const UniquePtr<const U>> span)
    {
        static_assert(std::is_base_of_v<U, T>,
            "In order to down-cast U to T, T must inherit from U in function " GYN_FUNC);
        return { reinterpret_cast<const UniquePtr<const T>*>(span.data()), span.size() };
    }

    template <typename T, typename U>
    ea::span<UniquePtr<T>> SpanDownCast(ea::span<UniquePtr<U>> span)
    {
        static_assert(std::is_base_of_v<U, T>,
            "In order to down-cast U to T, T must inherit from U in function " GYN_FUNC);
        return { reinterpret_cast<UniquePtr<T>*>(span.data()), span.size() };
    }

    inline ea::string ToEAString(const std::string& str)
    {
        return ea::string(str.c_str());
    }

    template <typename T>
    ea::vector<T> ToEAVector(std::vector<T>&& v)
    {
        ea::vector<T> ret{};
        const auto size = v.size();

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            ret.resize(size);
            memcpy(ret.data(), v.data(), size * sizeof(T));
        }
        else
        {
            ret.reserve(size);

            for (auto& i : v)
            {
                ret.emplace_back(std::move(i));
            }
        }

        return ret;
    }

    template <typename Key, typename T>
    bool HashMapHasValue(const ea::hash_map<Key, T>& map, const Key& key)
    {
        return map.find(key) != map.end();
    }

    void* OperatorNewArray(size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line);

    void* OperatorNewArray(size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
}

inline void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return ::Gyn::OperatorNewArray(size, pName, flags, debugFlags, file, line);
}

inline void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return ::Gyn::OperatorNewArray(size, alignment, alignmentOffset, pName, flags, debugFlags, file, line);
}