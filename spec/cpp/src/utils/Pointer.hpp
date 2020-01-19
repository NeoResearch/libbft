#ifndef LIBBFT_POINTER_HPP
#define LIBBFT_POINTER_HPP

#include <memory>

template <class T> std::unique_ptr<T> clonePtr(const T &obj) {
   return std::unique_ptr<T>(new T(obj));
}

#endif //LIBBFT_POINTER_HPP
