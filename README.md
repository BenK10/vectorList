# vectorList
experimental noncontiguous vector

VectorList, aka aka multivector, aka Megavector, is an experimental data structure providing a vector interface and vector-like behavior
without the requirement that the vector be contiguous. Its intended use is for very large quantities of data
where there may not be sufficient memory for the entire vector to fit into a single contiguous block of memory, or where
vector-like semantics are needed but insertion is slow due to the vector's size.

For large vectors of large-ish objects (bigger than cache-line width) the multivector outperforms a standard vector for insertion
since only the elements in a subvector needs to be shifted to make room. However, indexing is slow due to the overhead involved 
in calculating where in the data structure the logical index refers to. This isn't so bad for occasional accesses 
but iteration is very slow. Maybe this can be improved, but it will never be competitive with standard vectors. 

The multivector, if it can be improved enough, could serve as a niche structure where you need the indexing behavior of a vector
but don't have enough memory for a huge vector, or you are doing many insertions on a large vector where the multivector will be faster.

Some of the iterator operators are incomplete and intentionally left "wrong" since only those necessary to test the
multivector's performance were implemented fully.

Some of the demo code was inspired by http://www.codeproject.com/Articles/340797/Number-crunching-Why-you-should-never-ever-EVER-us
