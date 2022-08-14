#include <cstdlib>

#include <thread>
#include <mutex>

#include <vector>
#include <map>
#include <queue>

#include <functional>

#include <chrono>

namespace threadpool
{
    template<typename ResultType, typename TaskParamType>
class pool
{

public:

    pool(size_t size, std::function<ResultType(TaskParamType)>& thread_function,  std::function<void(size_t, ResultType)>& result_callback) : _size(size), _idle(size), _result_callback(result_callback)
    {   
        for (size_t i = 0; i < size; ++i)
        {
            _pool.push_back(std::thread([this, thread_function, &result_callback, i](){

                do
                {
                    TaskParamType task;
                    bool task_found = false;

                    {
                        std::lock_guard<std::mutex> lock(this->_mutex);
                        if (!this->_task_queue.empty())
                        {
                            task_found = true;
                            task = this->_task_queue.front();
                            this->_task_queue.pop();
                            this->_idle = this->_idle - 1;
                        }
                    }

                    if (task_found)
                    {
                        ResultType result = thread_function(task);
                        result_callback(i, result);
                    }

                    {
                        std::lock_guard<std::mutex> lock(this->_mutex);
                        this->_idle = this->_idle + 1;
                    }
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(100ms); /// provid a default and chrono literals
                }
                while(!this->_stop);
                //while(!this->_stop_token.stop_requested);
            }));
        }
    }

    ~pool()
    {
        stop();
    }
    
    void stop()
    {

        {
            std::lock_guard<std::mutex> lock(_mutex);
            //_stop_token.request_stop();
            _stop = true;
        }
        for(auto& thread : _pool)
        {
            thread.join();
        }
        
    }

    // Change to return task id
    void add_task(const TaskParamType& task)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _task_queue.push(task);
    }

    size_t size() const {return _size;}


    static unsigned int hardware_concurrency() noexcept
    {
        return std::thread::hardware_concurrency();
    }

private:
    size_t _size;
    size_t _idle;

    // Move to using stop token 
    // std::stop_token _stop_token;
    bool _stop; 
    std::vector<std::thread> _pool;

    std::mutex _mutex;

    std::queue<TaskParamType> _task_queue;

    std::function<void(size_t, ResultType)>& _result_callback;
};
}
