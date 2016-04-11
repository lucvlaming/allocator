#pragma once

#include "Common.h"

template<uint_fast32_t Index, typename T, typename Parent>
class Field {
private:
    char MakeEmpty[0];

    constexpr T* GetAddress() noexcept {
        return reinterpret_cast<T*>(Parent::FieldAddress(this, Index));
    }

public:
    //if we want uninitialized memory, thats ok.
    Field() {}
    
    template<typename ... Args>
    __always_inline
    Field(Args&& ... args) noexcept {
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
