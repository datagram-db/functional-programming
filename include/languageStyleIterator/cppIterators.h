//
// Created by datagram-db on 13/10/2019.
//

#ifndef SKELETON_CPPITERATORS_H
#define SKELETON_CPPITERATORS_H

#include "javaIterators.h"
#include <vector>

/**
 * This namespace provides the wrappers for the iterators over the standard c++ collections
 */
namespace cpp {
    /**
     * Iterator for vectors
     * @tparam T
     */
    template<typename T>
    class VectorIterator : public java::Iterator<T> {
        typedef typename std::vector<T> Tvec;

        Tvec &beg;
        size_t i;
    public:
        virtual ~VectorIterator() {}
        VectorIterator<T>(std::vector<T> &ref) : java::Iterator<T>{}, beg{ref}, i{0} {
        }
        VectorIterator<T>(VectorIterator<T>& ref) : java::Iterator<T>{}, beg{ref.beg}, i{0} {}

        VectorIterator<T>(VectorIterator<T>&& ref) : java::Iterator<T>{}, beg{std::move(ref.beg)}, i{ref.i} {}

        VectorIterator<T> &operator=( VectorIterator<T> && ref) {
            beg = std::move(ref.beg);
            i = ref.i;
        }

        virtual bool hasNext() override {
            return i >= 0 && i < beg.size();
        }

        virtual T next() override {
            return beg[i++];
        }

        std::shared_ptr<java::Iterator<T>> clone() override { return std::shared_ptr<java::Iterator<T>>(this); }
    };

    /**
     * Iterable for vectors
     * @tparam T
     */
    template <typename T>
    class VectorIterable : public java::Iterable<T> {
        std::vector<T> it;

    public:
        virtual ~VectorIterable<T>() {}
        VectorIterable<T>(std::vector<T>& c) : java::Iterable<T>{}, it{c} {}

        virtual std::unique_ptr<java::Iterator<T>> unique_iterator() {
            std::unique_ptr<java::Iterator<T>> ptr{new VectorIterator<T>(it)};
            return std::move(ptr);
        }

        virtual std::shared_ptr<java::Iterator<T>> shared_iterator() {
            std::shared_ptr<java::Iterator<T>> ptr{new VectorIterator<T>(it)};
            return ptr;
        }
    };

}

#endif //SKELETON_CPPITERATORS_H
