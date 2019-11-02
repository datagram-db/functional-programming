//
// Created by datagram-db on 13/10/2019.
//

#ifndef SKELETON_FUNCTIONALITERATOR_H
#define SKELETON_FUNCTIONALITERATOR_H

#include "languageStyleIterator/javaIterators.h"
#include <functional>
#include <ostream>

namespace functional {

    /**
     * Maps the elements of a collection without recreating a new value
     * @tparam T      type of the original iterator
     * @tparam U      type of the transformed iterator
     */
    template<typename T, typename U>
    class MapIterator : public java::Iterator<U> {
        java::Iterator<T> *iterator;
        std::function<U(T)> fun;

    public:
        MapIterator(java::Iterator<T> *iterator, const std::function<U(T)> &fun) : iterator(iterator), fun(fun) {}

        MapIterator(MapIterator<T, U>& cp) : iterator{cp.iterator}, fun{cp.fun} {}

        ~MapIterator() override {
            if (iterator) delete iterator;
        }

        bool hasNext() override {
            return iterator && iterator->hasNext();
        }

        U next() override {
            return fun(iterator->next());
        }

        /**
         * Clones the current element into a shared_ptr
         * @return
         */
        std::shared_ptr<java::Iterator<U>> clone() override { return std::make_shared<java::Iterator<U>>(*this); }
    };


    template<typename T, typename U>
    java::Iterator<T> *operator>>(java::Iterator<T> *os, const std::function<U(T)> &pref) {
        return new MapIterator<T, U>(os, pref);
    }


    /**
     * Filters the elements of a collection without creating a shrinked collection
     * @tparam T
     */
    template<typename T>
    class FilterIterator : public java::Iterator<T> {
        java::Iterator<T> *iterator;
        std::function<bool(T)> predicate;
        T elem;

    public:
        FilterIterator(java::Iterator<T> *iterator, const std::function<bool(T)> &fun) : iterator(iterator),
                                                                                         predicate(fun) {}

        FilterIterator(FilterIterator<T>& cp) : iterator{cp.iterator}, predicate{cp.predicate} {}

        ~FilterIterator<T>() override {}

        bool hasNext() override {
            if (!iterator->hasNext())
                return false;
            while (iterator->hasNext()) {
                if (predicate(elem = iterator->next()))
                    return true;
            }
            return false;
        }


        T next() override {
            return elem;
        }

        std::shared_ptr<java::Iterator<T>> clone() override { return std::make_shared<java::Iterator<T>>(*this); }
    };

    template<typename T>
    java::Iterator<T> *operator&(java::Iterator<T> *os, std::function<bool(T)> pref) {
        return new FilterIterator<T>(os->quit(), pref);
    }


    /**
     * Creates a collection with one single element
     * @tparam T
     */
    template<typename T>
    class Inject : public java::Iterator<T> {
        bool got;
        T value;

    public:
        Inject(T x) : java::Iterator<T>{}, value{x}, got{false} {}

        Inject(Inject<T>& cp) : Inject{cp.value} {}

        ~Inject() override {}

        bool hasNext() override {
            return !got;
        }

        T next() override {
            got = false;
            return value;
        }


        std::shared_ptr<java::Iterator<T>> clone() const override { return std::make_shared<java::Iterator<T>>(*this); }
    };


    struct done {};
    struct done quit_join;

    /**
     *
     * @tparam T
     */
    template<typename T>
    class Join {
        class Unnest : public java::Iterator<T> {

        public:
            typedef typename std::vector<std::shared_ptr<java::Iterator<T>>>::iterator itT;
            typedef typename std::vector<std::shared_ptr<java::Iterator<T>>> vec;

            ~Unnest() override {}

            Unnest() : java::Iterator<T>{} {}

            void add(const java::Iterator<T>* m) {
                v.push_back(std::make_shared(m));
            }

            void add(const std::shared_ptr<java::Iterator<T>> m) {
                v.push_back(m->clone());
            }

            Unnest &quit() {
                beg = v.begin();
                en = v.end();
            }

            bool hasNext() override {
                while (beg != en) {
                    if ((*beg)->hasNext())
                        return true;
                    else (beg++);
                }
                return beg != en;
            }

            T next() override {
                return (*beg)->next();
            }

            std::shared_ptr<java::Iterator<T>> clone() override { return std::make_shared<java::Iterator<T>>(*this); }

        private:
            itT beg, en;
            vec v;

        };

        std::unique_ptr<Unnest> result;

    public:
        Join<T>() : result{new Unnest()} {}

        virtual ~Join<T>() {}

        Join<T> &add(const java::Iterator<T>* m) {
            result->add(m);
            return *this;
        }

        Join<T> &add(const std::shared_ptr<java::Iterator<T>> m) {
            result->add(m);
            return *this;
        }

        java::Iterator<T>* done() {
            result->quit();
            return result.get();
        }
    };

    template <typename T>
    Join<T> &operator>>(Join<T> &os, const java::Iterator<T>* join) {
        return join ? os.add(join) : os;
    }

    template <typename T>
    Join<T> &operator>>(Join<T> &os, const std::shared_ptr<java::Iterator<T>>& join) {
        return join ? os.add(join) : os;
    }

}

#endif //SKELETON_FUNCTIONALITERATOR_H
