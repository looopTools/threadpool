# Threadpool 

Threadpool is intended to provide an all C++ based thread pool using modern C++ components such as `jthread`.
We see this as a proposal for how standard library (STL) threadpool could/should look and to provide a potential starting point for such a component to be added to the STL. 

## Why jthread

`jthread` is the future and we need threadpool to be future proof that is why we have chosen `jthread` over traditional `thread`.

### C++20 and only

As we are using `jthread` threadpool can only be compiled using c++20 and up. 


## Design Goal 

The design goal from Threadpool is to make a super easy to use Threadpool that doesn't require extensive knowledge of concurrent programming to use. 

