Array.hpp: Container class template for an array with r-value references for move constructor and assignment.

Deque.hpp: Macro to implement a template for a double-ended queue.

Map.hpp: Container class template for a map with key-value pairs of any type with an iterator, constant iterator, and reverse iterator.  Utilized skip lists to achieve an O(lg(N)) time complexity for key lookup, insertion, and deletion.

SmartPtr.hpp: Implemented a class template for a non-intrusive, thread-safe, exception-safe, and reference-counting smart pointer using std:mutex.  To delete the object the smart pointer pointed to, I included deleter base and derived classes to arbitrarily create a deleter object and explicitly define it

Sum.cpp:  Implemented an index class, variadic sum function, and base sum function that added an assortment of elements of a std∷array and indices of the array
