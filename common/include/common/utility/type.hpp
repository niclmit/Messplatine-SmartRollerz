#ifndef COMMON_UTILITY_TYPE_HPP_
#define COMMON_UTILITY_TYPE_HPP_

#include <cstdint>

namespace common::utility {

template<typename Type> 
using constant = Type const;

template<typename Type> 
using volat = Type volatile;

template<typename Type> 
using pointer = Type*;

template<typename Type> 
using const_pointer = constant<Type*>;

template<typename Type> 
using lreference = Type&;

template<typename Type> 
using reference = lreference<Type>;

template<typename Type> 
using const_reference = constant<lreference<Type>>;

template<typename Type> 
using rreference = Type&&;

template<typename Type, class Class> 
using member_pointer = Type Class::*;

template<typename Type, std::size_t Size> 
using raw_array = Type[Size];

template<typename Return, typename... Args> 
using function = Return(Args...);

template<typename Return, typename... Args> 
using function_pointer = pointer<Return(Args...)>;

} // namespace common::utility 

#endif // COMMON_UTILITY_TYPE_HPP_
