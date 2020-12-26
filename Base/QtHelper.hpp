#ifndef QTHELPER_HPP
#define QTHELPER_HPP

#include <type_traits>

#define CONNECT3(obj, func, lambda) QObject::connect(obj, &std::remove_pointer_t<decltype(obj)>::func, lambda)
#define CONNECT4(obj1, func1, obj2, func2) QObject::connect(obj1, &std::remove_pointer_t<decltype(obj1)>::func1, obj2, &std::remove_pointer_t<decltype(obj2)>::func2)

#define CONNECT_MACRO(_1, _2, _3, _4, FUNC, ...) FUNC
#define CONNECT(...) CONNECT_MACRO(__VA_ARGS__, CONNECT4, CONNECT3)(__VA_ARGS__)

#endif // QTHELPER_HPP