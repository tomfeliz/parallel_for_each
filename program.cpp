#include <iostream>
#include <vector>

#include "parallel.h"
#include <numeric>
#include <mutex>
#include <atomic>
#include <string>

uint64_t fibonacci(uint64_t n)
{
	if (n == 0 || n == 1)
		return n;

	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
	std::vector<uint64_t> data(300);
	std::fill(begin(data), end(data), 35);
	std::mutex mutex_cons;
	std::atomic<int> count = 0;

	const auto nthreads = std::thread::hardware_concurrency();
	std::cout << "Using " << nthreads << " execution threads." << std::endl;

	// Set global start clock
	const auto start0 = clock();

	parallel_for_each(begin(data), end(data), [&mutex_cons, &count](uint64_t n)
	{
		auto fib = fibonacci(n);

		mutex_cons.lock();
		std::cout << "Result " << ++count << ", Thread " << std::this_thread::get_id() << " : fibonacci(" << n << ") = " << fib << std::endl;
		mutex_cons.unlock();
	});

	auto finish0 = clock();
	std::cout << "Total execution time: " << static_cast<float>(finish0 - start0) / CLOCKS_PER_SEC << " seconds" << std::endl;

	std::string temp;
	std::cout << "Press enter to terminate program..." << std::endl;
	std::getline(std::cin, temp);
	return 0;
}
