#include <cstdlib>

#include <thread>
#include <mutex>

#include <vector>
#include <map>
#include <queue>

#include <functional>


namespace threadpool
{
    template<typename ResultType, typename FunctionType, typename TaskParamType>
class pool
{

public:

    pool(size_t size, std::function<FunctionType> thread_function,  std::function<void(size_t, ResultType)> result_callback) : _size(size), _idle(size), _result_callback(result_callback)
    {
        _sleep_time = 1000;
        
        for (size_t i = 0; i < size; ++i)
        {
            _pool.push_back(std::jthread([this, thread_function](){

                do
                {
                    TaskParamType task;
                    bool task_found = false;

                    {
                        std::lock_guard<std::mutex> lock(this->_mutex);
                        if (!this->task_queue.empty())
                        {
                            task_found = true;
                            task = this->task_queue.front();
                            this->task_queue.pop();
                        }
                    }

                    if (task_found)
                    {
                        auto result = thread_function(task);
                    }

                    std::this_thread::sleep_for(200ms); /// provid a default and chrono literals
                }
                while(!this->_stop_token.stop_requested);
            }));
        }
    }

    ~pool()
    {
        stop();
    }
    
    void stop()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop_token.request_stop();        
    }

    // Change to return task id
    void add_task(const TaskParamType& task)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _task_queue.push(task);
    }

    // Change to chrono literals and stuff 
    void sleep_time(size_t sleep_time)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _sleep_time = sleep_time;
            
    }

    size_t sleep_time() {return _sleep_time;}

    

private:
    size_t _size;
    size_t _idle;

    size_t _sleep_time;

    std::stop_token _stop_token; 
    std::vector<jthread> _pool;

    std::mutex _mutex;

    std::queue<TaskParamType> _task_queue;

    std::function<void(size_t, ResultType)>& _result_callback;
};
}
