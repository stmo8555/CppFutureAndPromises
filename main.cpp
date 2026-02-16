#include "FuturesAndPromises.h"
#include <iostream>
#include <thread>
#include <chrono>

void Foo(Promise<int>& promise);

int main(int argc, char const *argv[])
{
    auto promise = Promise<int>();
    auto future = promise.GetFuture();
    std::thread t1{Foo, std::ref(promise)};
    auto result = future.Get();

    std::cout << "Result: " << result << std::endl;
    t1.join();
    std::cout << "Thread done" << std::endl;

    return 0;
}

void Foo(Promise<int>& promise)
{
    int i = 0;

    while (i < 5)
    {
        std::cout << i << std::endl;
        ++i;
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }


    promise.SetValue(i);
}
