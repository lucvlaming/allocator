#pragma once

#include "Common.h"

template<uint_fast32_t Index, typename T, typename Parent>
class Field {
private:
    char MakeEmpty[0];

    constexpr T* GetAddress() {
        auto addr = reinterpret_cast<size_t>(this);
        auto base = (addr & Parent::BlockMask);
        auto itemIndex = addr & ~Parent::BlockMask;
//        std::cout << (void*)base << " " << itemIndex << " " << Parent::BlockOffset(Index) << " " << sizeof(T) << " "
//                  << reinterpret_cast<T*>(base + Parent::BlockOffset(Index)) << " "
//                  << reinterpret_cast<T*>(base + Parent::BlockOffset(Index) + itemIndex * sizeof(T)) << std::endl;
        return reinterpret_cast<T*>(base + Parent::BlockOffset(Index) + itemIndex * sizeof(T));
    }

public:
    template<typename ... Args>
    __always_inline
    Field(Args&& ... args) {
        new (GetAddress()) T(std::forward<Args>(args)...);
    }

    T* operator* ()       { return GetAddress(); }
    T* operator* () const { return GetAddress(); }

    T* operator& ()       { return GetAddress(); }
    T* operator& () const { return GetAddress(); }

    operator T   () const { return *GetAddress(); }

    template<typename U>
    T& operator=(U val) {
        auto addr = GetAddress();
        *addr = val;
        return *addr;
    }
};

//for convenience we simply store both the size and offset
#define Field(Nr, Type) \
    static constexpr uint_fast32_t FieldSize##Nr = sizeof(Type); \
    static constexpr uint_fast32_t BOOST_PP_CAT(FieldOffset, BOOST_PP_INC(Nr)) = BOOST_PP_CAT(FieldOffset, Nr) + FieldSize##Nr; \
    Field<Nr, Type, ParentType>
