#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <cmath>

template<typename It, typename T>
T accumulateParallel(It first, It last, T init)
{
    auto length = std::distance(first, last);
    if(length == 0) return init;

    auto hardwareThreads = std::thread::hardware_concurrency();
    auto numOfThreads = hardwareThreads == 0 ? 2 : hardwareThreads;
    auto blockSize = ceil(static_cast<double>(length) / numOfThreads);

    std::vector<T> partialSum(numOfThreads);
    std::vector<std::thread> vecOfThreads;

    It beginOfBlock = first;
    It endOfBlock = first;

    for(auto i = 0; i < numOfThreads - 1; i++)
    {
        std::advance(endOfBlock, blockSize);
        vecOfThreads.emplace_back([=, &partialSum](){
                partialSum[i]=std::accumulate(beginOfBlock, endOfBlock, 0);
        });
        beginOfBlock = endOfBlock;
    }

    partialSum[numOfThreads-1]=std::accumulate(beginOfBlock, last, 0);

    for(auto&& thread : vecOfThreads) thread.join();

    return std::accumulate(partialSum.begin(), partialSum.end(), init);
}

int main()
{
    std::cout<<"Hardware threads: " <<std::thread::hardware_concurrency()<<std::endl;

    std::vector<int> v(10000000);
    std::fill(v.begin(), v.end(), 1);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = accumulateParallel(v.begin(), v.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration parallel: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    start = std::chrono::high_resolution_clock::now();
    result = std::accumulate(v.begin(), v.end(), 0);
    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration seq: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    return 0;
}
