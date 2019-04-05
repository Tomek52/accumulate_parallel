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
    if(numOfElements==0) return init;
    int sizeOfPart = ceil(static_cast<double>(numOfElements)/static_cast<double>(numOfThreads));
    std::vector<std::thread> threadsSum;
    std::vector<T> partialSum(numOfThreads);
    for(int i = 0; i<numOfThreads; i++) 
    {
        threadsSum.emplace_back([=, &partialSum](){partialSum[i]=std::accumulate(first,first+sizeOfPart,0);});
        first+=sizeOfPart;
    }
    for(auto && thread: threadsSum) thread.join();
    return std::accumulate(partialSum.begin(),partialSum.end(),0);
}

int main()
{
    std::vector<double> v(4450001,2);
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
