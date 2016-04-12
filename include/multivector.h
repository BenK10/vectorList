#ifndef multivector_H
#define multivector_H

#include <vector>
#include <iostream>
#include <iterator>

//forward declaration
template<typename T>
class multivector_iterator;

template<class T>
class multivector
{
private:
    int subvector_capacity;
    std::vector< std::vector<T>* > vectors;

public:
    //default ctor
    multivector() : subvector_capacity(10)
    {
        auto vptr = new std::vector<T>;
        vptr->reserve(subvector_capacity);
        vectors.push_back(vptr);
    }

    multivector(int capacity, int subvectors) : subvector_capacity(capacity/subvectors)
    {
        //assume for now subvectors evenly divides capacity

        vectors.reserve(subvectors);
        for(int i=0; i<subvectors; ++i)
        {
            auto vptr = new std::vector<T>;
            vptr->reserve(subvector_capacity);
            vectors.push_back(vptr);
        }
    }

    //fill constructor
    multivector(int capacity, int subvectors, const T& item)
    {
        //assume for now subvectors evenly divides capacity
        subvector_capacity = capacity/subvectors;
        vectors.reserve(subvectors);
        for(int i=0; i<subvectors; ++i)
        {
            auto vptr = new std::vector<T> (subvector_capacity, item);
            vectors.push_back(vptr);
        }
    }

    //TODO: copy, move, move-assign operators

    //deep copy cstor
    multivector(const multivector& other)
    {
        if (this == &other) return; // handle self copy

        for(auto ptr : vectors)
            delete ptr;

        vectors.clear();
        for(auto ptr : other.vectors)
        {
            auto vptr = new std::vector<T>;
            copy(*ptr.begin(), *ptr.end(), *vptr.begin());
            vectors.push_back(vptr);
        }

    }

    ~multivector()
    {
        for (auto ptr : vectors)
            delete ptr;
    }

    //assignment with deep copy
    multivector& operator=(const multivector& other)
    {
        if (this == &other) return *this; // handle self assignment
        else return multivector(other);
    }

    //fill with some value
    void fill_vec(T val)
    {
        for( auto vec : vectors)
            for(int i=0; i<subvector_capacity; ++i)
                vec->push_back(val);
    }

    //index operator
    T& operator[](int idx)
    {
        int subvector = 0;
        int subvector_idx = idx%subvector_capacity;

        //skip unused capacity at end of subvector and take from next subvector
        int size_sum = 0;
        for(auto vec : vectors)
        {
            size_sum+=vec->size();
            if(size_sum >= idx + 1) break;
            ++subvector;
        }

        int logical_idx_of_subvector = size_sum - vectors[subvector]->size();
        return (*vectors[subvector])[idx - logical_idx_of_subvector];
    }

    //TODO: change to iterator parameter to match std::vector interface
    //TODO: change access to version in operator[]
    void insert(int idx, const T& item)
    {
        //compute index location
        int subvector = 0;
        int size_sum = 0;
        for(auto vec : vectors)
        {
            size_sum+=vec->size();
            if(size_sum >= idx + 1) break;
            ++subvector;
        }

        //insert
        //if subvector to insert into already full, then...
        if( (*vectors[subvector]).size() == (*vectors[subvector]).capacity() )
        {
            //..insert new subvector, place last element of subvector to be inserted into in it, and insert
            auto vptr = new std::vector<T>;
            vptr->reserve(subvector_capacity);

            vptr->push_back((*vectors[subvector]).back());
            (*vectors[subvector]).pop_back();
            (*vectors[subvector]).insert((*vectors[subvector]).begin() + idx, item);

            vectors.insert(vectors.begin() + subvector+1, vptr);
        }
        else (*vectors[subvector]).insert( (*vectors[subvector]).begin()+idx, item );

    }


    //DEBUG
    std::vector<T>* subvector(int idx)
    {
        return vectors[idx];
    }

    multivector_iterator<T> begin()
    {
        return multivector_iterator<T>(*this, vectors[0]->begin());
    }

    multivector_iterator<T> end()
    {
        return multivector_iterator<T>(*this, vectors[vectors.size()-1]->end());
    }

    void print()
    {
        for( auto vec : vectors)
        {
            for(auto item : (*vec) )
                std::cout<<item<<" ";
            std::cout<<std::endl;
        }
    }

    friend class multivector_iterator<T>;
};

template<typename T>
class multivector_iterator : std::iterator<std::random_access_iterator_tag, T>
{
    friend class multivector<T>;

private:
    typename std::vector<T>::iterator iter;
    multivector<T>& mv;

public:

    typedef multivector_iterator<T> iterator;

    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    multivector_iterator<T> ()
    {
        mv=nullptr;
    }
    multivector_iterator<T>(multivector<T>& mvr, typename std::vector<T>::iterator it) : iter(it), mv(mvr) {}

    multivector_iterator operator+(const int& rhs)
    {
        //TODO:
        //find out which vector the iter is in (check if bounded by subvector's begin, end)
        //get the logical index of iter (iter - current vector.begin)
        //add rhs to that and find out which subvector that is in
        //set iter to target logical index

        return this.iter + rhs;
    }

    multivector_iterator operator+(const int& rhs)
    {
        return this.iter + rhs;
    }

    multivector_iterator operator-(const int& rhs)
    {
        //TODO: similar to +
        return this.iter - rhs;
    }

    difference_type operator-(multivector_iterator& rhs)
    {
        return this.iter-rhs.iter;
    }

    value_type& operator*()
    {
        return *(this->iter);
    }

    bool operator!=(multivector_iterator<T> rhs)
    {
        if((rhs.iter) != this->iter) return true;
        return false;
    }

    bool operator==(multivector_iterator<T>& rhs)
    {
        if((rhs.iter) != this->iter) return false;
        return true;
    }

    iterator& operator++(int) //postfix
    {
        //check that not at end of a subvector
        for(int i=0; i<mv.vectors.size(); ++i)
        {
            if(iter == mv.vectors[i]->end())
            {
                iter = mv.vectors[i+1]->begin();
                return *this;
            }
        }
        iter+=1;
        return *this;
    }

    iterator& operator++() //prefix
    {
        //check that not at end of a subvector
        for(int i=0; i<mv.vectors.size(); ++i)
        {
            if(iter == mv.vectors[i]->end())
            {
                iter = mv.vectors[i+1]->begin();
                return *this;
            }

        }
        iter+=1;
        return *this;
    }

    iterator operator--(int) //postfix
    {
        //check that not at beginning of a subvector
        for(int i=mv.vectors.size()-1; i>0; --i)
        {
            if(iter == mv.vectors[i]->begin())
            {
                iter = mv.vectors[i-1]->end()-1;
                return iter;
            }
        }
        iter-=1;
        return iter;
    }

    iterator operator--() //prefix
    {
        //check that not at beginning of a subvector
        for(int i=mv.vectors.size()-1; i>0; --i)
        {
            if(iter == mv.vectors[i]->begin())
            {
                iter = mv.vectors[i-1]->end()-1;
                return iter;
            }
        }
        iter-=1;
        return iter;
    }

    bool operator<(multivector_iterator& rhs)
    {
        //TODO: when iters not in same subvector?
        return this.iter < rhs.iter; //only valid if both in same subvector
    }

    void swap(const multivector_iterator& rhs)
    {
        std::swap(this.iter, rhs.iter);
    }

};

#endif // multivector_H
