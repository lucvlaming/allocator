#pragma once

#include "Common.h"

#include <memory>

//note: this may contain state since c++11
template<class T>
class allocator {
public:
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T        value_type;

    allocator() noexcept;
    ~allocator() noexcept;
    pointer
    address(reference __x) const noexcept
    { return std::__addressof(__x); }

    const_pointer
    address(const_reference __x) const noexcept
    { return std::__addressof(__x); }

    pointer
    allocate(size_type __n, const void* = 0) {
        //just allocate, create one big block of requested size
        
        //if done -> offset dynamic
        //-->> one extra register used?
    }
    
    void
    deallocate(pointer __p, size_type)
    { ::operator delete(__p); }
    
    size_type
    max_size() const noexcept
    { return size_t(-1) / sizeof(T); }
    
    template<typename _Up, typename... _Args>
    void
    construct(_Up* __p, _Args&&... __args)
    { ::new((void *)__p) _Up(std::forward<_Args>(__args)...); }
    
    template<typename _Up>
    void 
    destroy(_Up* __p) { __p->~_Up(); }
    
    
    //TODO: fixme check if we have state
    template<typename _Tp>
    inline bool
    operator==(const allocator<_Tp>&, const allocator<_Tp>&)
    { return false; }
    
    template<typename _Tp>
    inline bool
    operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
    { return true; }
private:
    

};
