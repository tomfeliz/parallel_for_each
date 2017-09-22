#include <atomic>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "parallel.h"

// Naive Fibonacci function to load CPU (2^(n-1) recursive calls)
uint64_t fibonacci(uint64_t n)
{
	if (n == 0 || n == 1)
		return n;

	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
	std::mutex mutex_cons; // protest default console output
	std::atomic<int> count = 0; // atomic result counter

	std::vector<int> data(1000); // adjust vector length to increse/decrease running time

	// Populate test data vector with random Fibonacci seeds (in a range)
	std::default_random_engine rand_eng{};
	std::uniform_int_distribution<int> dist{ 20, 45 };
	std::for_each(begin(data), end(data), [&rand_eng, &dist](int& n)
	{
		n = dist(rand_eng);
	});

	// Report number of threads we'll be using
	const auto threads = std::thread::hardware_concurrency();
	std::cout << "Using " << threads << " execution threads." << std::endl;

	const auto startGlobal = clock();

	// Make call to parallel_for_each and do the actual work.
	parallel_for_each(begin(data), end(data), [&mutex_cons, &count](uint64_t n)
	{
		const auto start = clock();
		auto fib = fibonacci(n);

		// Report the thread id, result, and time to execute each thread.
		auto finish = clock();
		mutex_cons.lock();
		std::cout << "Result " << ++count << ", Thread " << std::this_thread::get_id() 
			<< " : fibonacci(" << n << ") = " << fib << " (" 
			<< static_cast<float>(finish - start) / CLOCKS_PER_SEC << " ms)" << std::endl;
		mutex_cons.unlock();
	});

	// Report total execution time.
	auto finishGlobal = clock();
	std::cout << "Total execution time: " << static_cast<float>(finishGlobal - startGlobal) / CLOCKS_PER_SEC 
		<< " seconds" << std::endl;

	// Wait for user to respond
	std::string temp;
	std::cout << "Press enter to terminate program..." << std::endl;
	std::getline(std::cin, temp);
	return 0;
}
