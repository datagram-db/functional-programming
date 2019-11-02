#include <iostream>
#include <functional>
#include <vector>
#include <memory>


#include "include/languageStyleIterator/cppIterators.h"
#include "include/functionalIterator.h"

int main() {
    std::vector<int> test{1,3,5};
    std::vector<int> test2{1,2,3,4,5,6,7,8};

    //auto beg = Iterator<int, std::vector<int>::iterator>(test.begin());
    //auto en = Iterator<int, std::vector<int>::iterator>(test.end());
   // MapIterator<int, std::string,  std::vector<int>::iterator> mi{};

   cpp::VectorIterable<int> it{test};
   std::shared_ptr<java::Iterator<int>> l = it.shared_iterator();

   cpp::VectorIterable<int> it2{test2};
    std::shared_ptr<java::Iterator<int>> m = it2.shared_iterator();


   // Chaining all the vectors into one single iteration

   functional::Join<int> j{};
    java::Iterator<int>* join = (j >> l >> m).done();

   //java::Iterator<int>* join = ((new functional::Unnest<int>())->add(it.iterator()))->add(it2.iterator())->quit();

   // Mapping all the numbers into something else
   java::Iterator<int>* fmap = new functional::MapIterator<int,int>(join, [](int k){return k+3;});

   // Filtering the elements
   java::Iterator<int>* mpi = new functional::FilterIterator<int>(fmap, [](int k){return k % 2;});

   // Final iteration
   while (mpi->hasNext()) {
       std::cout << mpi->next() << std::endl;
   }


   delete mpi;
   delete fmap;

    return 0;
}