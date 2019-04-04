#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <numeric>
#include <cmath>

template<class InputIt, class T>
T accumulateParallel(InputIt first, InputIt last, T init, int numOfThreads)
{
    int numOfElements = std::distance(first,last);
    int sizeOfPart = ceil(static_cast<double>(numOfElements)/static_cast<double>(numOfThreads));
    std::vector<std::thread> threadsSum;
    for(int i = 0; i<numOfThreads; i++) 
    {
        threadsSum.emplace_back([=, &init](){init+=std::accumulate(first,first+sizeOfPart,0);});
        first+=sizeOfPart;
    }
    for(auto && thread: threadsSum) thread.join();
    return init;
}

int main()
{
    std::vector<int> v(4450001,2);
    auto x = accumulateParallel(v.begin(), v.end(), 0, 4);
    if(std::accumulate(v.begin(),v.end(),0)==x)
    {
        std::cout<<x<<std::endl;
    }
    else
    {
        std::cout<<"error"<<std::endl;
    }
    return 0;
}
