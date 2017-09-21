#include <atomic>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "parallel.h"

uint64_t fibonacci(uint64_t n)
{
	if (n == 0 || n == 1)
		return n;

	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
	std::mutex mutex_cons;
	std::atomic<int> count = 0;

	std::vector<int> data(1000);
	std::default_random_engine rand_eng{};
	std::uniform_int_distribution<int> dist{ 20, 40 };
	std::for_each(begin(data), end(data), [&rand_eng, &dist](int& n)
	{
		n = dist(rand_eng);
	});

	const auto threads = std::thread::hardware_concurrency();
	std::cout << "Using " << threads << " execution threads." << std::endl;

	const auto startGlobal = clock();

	parallel_for_each(begin(data), end(data), [&mutex_cons, &count](uint64_t n)
	{
		const auto start = clock();
		auto fib = fibonacci(n);

		auto finish = clock();
		mutex_cons.lock();
		std::cout << "Result " << ++count << ", Thread " << std::this_thread::get_id() 
			<< " : fibonacci(" << n << ") = " << fib << " (" 
			<< static_cast<float>(finish - start) / CLOCKS_PER_SEC << " ms)" << std::endl;
		mutex_cons.unlock();
	});

	auto finishGlobal = clock();
	std::cout << "Total execution time: " << static_cast<float>(finishGlobal - startGlobal) / CLOCKS_PER_SEC 
		<< " seconds" << std::endl;

	std::string temp;
	std::cout << "Press enter to terminate program..." << std::endl;
	std::getline(std::cin, temp);
	return 0;
}
