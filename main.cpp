#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <iostream>
#include "include\multivector.h"

using namespace std;
using namespace std::chrono;

template<int Size>
struct POD
{
   int a[Size];
};


int main()
{
    //TODO: compare with linked list

  // vector<int> s(256);
    const int x=128;
    POD<x> pod;

    vector<int> sizes = {100, 1000, 10000, 100000, 1000000};

    cout<<"POD size: "<<x<<endl;
    cout << "construction times" << endl;
    cout << "vector:" << endl;
    for(int size : sizes)
    {
        auto t0 = high_resolution_clock::now();
        vector<POD<x>> blub(size, pod);
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout << "multivector:" << endl;
    for(int size : sizes)
    {
        auto t0 = high_resolution_clock::now();
        multivector<POD<x>> mv(size, 10, pod);
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout << "linked list:" << endl;
    for(int size : sizes)
    {
        auto t0 = high_resolution_clock::now();
        list<POD<x>> ll(size, pod);
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl<<endl;
    cout<<"traversal times"<<endl;
    cout<<"vector, operator[]"<<endl;
     for(int size : sizes)
    {
        vector<POD<x>> v(size, pod);
        auto t0 = high_resolution_clock::now();
        for(int i=0; i<v.size(); ++i)
            v[i];
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout<<"vector, iterator"<<endl;
     for(int size : sizes)
    {
        vector<POD<x>> v(size, pod);
        auto t0 = high_resolution_clock::now();
        for(auto it=v.begin(); it!=v.end(); ++it) {}
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout<<"multivector, operator[]"<<endl;
     for(int size : sizes)
    {
        multivector<POD<x>> mv(size, 10);
        mv.fill_vec(pod);
       auto t0 = high_resolution_clock::now();
        for(int i=0; i<size; ++i)
            mv[i];
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout<<"multivector, iterator"<<endl;
     for(int size : sizes)
    {
        multivector<POD<x>> mv(size, 10);
     //   cout<<&(*mv.begin())<<" "<<&(*mv.end());
        mv.fill_vec(pod);
       auto t0 = high_resolution_clock::now();
        for(auto mvi = mv.begin(); mvi!= mv.end(); ++mvi) {}
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout << "linked list:" << endl;
    for(int size : sizes)
    {
        list<POD<x>> ll(size, pod);
        auto t0 = high_resolution_clock::now();
        for(auto ll_iter = ll.begin(); ll_iter!=ll.end(); ++ll_iter){}
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl<<endl;
    cout<<"worst case insertion times"<<endl;
    cout<<"vector"<<endl;
     for(int size : sizes)
    {
        vector<POD<x>> v(size, pod);
        auto t0 = high_resolution_clock::now();
        v.pop_back();
        v.insert(v.begin()+1, pod);
       auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

     cout<<endl;
     cout<<"multivector"<<endl;
     for(int size : sizes)
    {
        multivector<POD<x>> mv(size, 10);
        mv.fill_vec(pod);
        auto t0 = high_resolution_clock::now();
        mv.insert(1, pod);
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    cout<<endl;
    cout << "linked list:" << endl;
    for(int size : sizes)
    {
        list<POD<x>> ll(size, pod);
        auto t0 = high_resolution_clock::now();
        ll.insert(ll.begin(), pod);
        auto t1 = high_resolution_clock::now();
        cout<<size << " "<< duration_cast<microseconds>(t1-t0).count()<<" ";
    }

    return 0;
}

