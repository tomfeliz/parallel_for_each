# parallel_for_each
I’ve used the Microsoft Parallel Patterns library (PPL) in the past and one of the most powerful features is a parallel for_each function that automatically breaks up an operation on an iterable data structure into chunks so the work can be automatically spread over multiple CPUs. Unfortunately, the PPL is only supported on Windows, so it’s not portable. I created a parallel_for_each function that works in a similar manner, except it’s only dependent on the std::thread library and is cross-platform capable. It's an easy way to enable parallel execution in existing apps where it doesn’t yet exist. The attached Visual Studio solution is a simple example application that demonstrates how it all works. 

Using it is as simple as including parallel.h and using the function like so:

parallel_for_each(begin(data), end(data), [](datatype_of_data ref)
{
       // Place code here   
});

