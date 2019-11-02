//
// Created by giacomo on 13/10/2019.
//

#ifndef SKELETON_JAVAITERATORS_H
#define SKELETON_JAVAITERATORS_H

#include <memory>

/**
 * This namespace contains all the java-style implementations of the iterators. This implies having an iterator and an iterable.
 * This construction is far way easier to implement functional programming iterators, without the need of explicitely
 * allocating intermediate data structures
 */
namespace java {

    /**
     * Implements an iterator over a collection
     * @tparam T
     */
    template <typename T> class Iterator {
    public:
        virtual ~Iterator<T>() {}

        Iterator<T>() {};
        Iterator<T>(Iterator<T>&& m) {};
        Iterator<T>(Iterator<T>& m) {};
        Iterator<T>(Iterator<T>* m) {};

        /**
         * Checks whethere there is the next element
         * @return
         */
        virtual bool hasNext() {
            return false;
        }
        /**
         * Returns the current element, and prepares the next one.
         * @return
         */
        virtual T next() {
            return (T)0;
        }

        virtual /**
         * Clones the current element into a shared_ptr
         * @return
         */
        std::shared_ptr<Iterator<T>> clone() { return std::make_shared<Iterator<T>>(*this); }

    };

    /**
     * An iterator is an object returning a new iterator over the same object
     * @tparam T
     */
    template <typename T> class Iterable {
    public:
        virtual ~Iterable<T>() {}
        virtual std::unique_ptr<Iterator<T>> iterator() const {
            std::unique_ptr<Iterator<T>> ptr{new Iterator<T>()};
            return std::move(ptr);
        }
        virtual explicit operator std::unique_ptr<Iterator<T>>() {
            std::unique_ptr<Iterator<T>> ptr{new Iterator<T>()};
            return std::move(ptr);
        }
    };
}


#endif //SKELETON_JAVAITERATORS_H
