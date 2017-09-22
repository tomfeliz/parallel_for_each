#pragma once
#include <algorithm>
#include <thread>
#include <vector>

template<typename Iterator, typename Function>
void parallel_for_each(const Iterator& first, const Iterator& last, const Function& l)
{
	const auto numThreads = std::thread::hardware_concurrency();
	const int64_t chunk = (last - first) / numThreads;
	std::vector<std::thread> threads;
	for (Iterator it = first; it != last; it += chunk)
	{
		Iterator begin = it;
		auto distance = std::distance(it, last);
		Iterator end = (std::distance(it, last) > chunk) ? it + chunk : last;
//		Iterator end = it + chunk;

		threads.push_back(std::thread([begin, end, l]()
		{
			for (Iterator i = begin; i != end; ++i)
			{
				l(*i);
			}
		}));
	}

	std::for_each(begin(threads), end(threads), [](std::thread& th)
	{
		th.join();
	});
}
