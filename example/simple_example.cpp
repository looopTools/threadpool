#include "../src/threadpool/pool.hpp"
#include <iostream>
#include <string>

#include <vector>

void callback(size_t i, int result) { std::cout << "Thread id: " << std::to_string(i) << " result: " << std::to_string(result) << "\n";}

int handle_data(std::vector<int> data)
{
    int result = 0;

    for(auto elm : data)
    {
        result = result + elm;
    }
    
    return result;
}

int main(void)
{
    std::function<void(size_t, int)> mcallback = callback;
    std::function<int(std::vector<int>)> m_handle_data = handle_data;
        
    threadpool::pool<int, std::vector<int>> pool(1, m_handle_data, mcallback);
    pool.add_task(std::vector<int>{1,2,3,4});

}
