#pragma once
#include <mutex>
#include <memory>
#include <condition_variable>

template <class T>
struct FutureState;

template <class T>
class Future
{
private:
    std::shared_ptr<FutureState<T>> m_futureState;

public:
    Future(std::shared_ptr<FutureState<T>> futureState) :  m_futureState{futureState} 
    {

    };
    
    void Wait()
    {
        std::unique_lock<std::mutex> lock{m_futureState->Mutex};
        m_futureState->Cv.wait(lock, [this] () -> bool {return this->m_futureState->Ready;});
    };

    T Get()
    {
        Wait();
        return m_futureState->Value;
    }
    
};

template <class T>
class Promise
{
private:
    std::shared_ptr<FutureState<T>> m_futureState;

public:
    Promise(): m_futureState{std::make_shared<FutureState<T>>()}
    {};
    
    void SetValue(T val)
    {
        m_futureState->Value = std::move(val);
        std::unique_lock<std::mutex> lock{m_futureState->Mutex};
        m_futureState->Ready = true;
        m_futureState->Cv.notify_one(); 
    };

    Future<T> GetFuture()
    {
        return Future<T>(m_futureState);
    };
};
template <class T>
struct FutureState
{
    T Value;
    bool Ready = false;
    std::mutex Mutex;
    std::condition_variable Cv;
};


