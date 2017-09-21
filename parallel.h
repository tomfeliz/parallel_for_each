#pragma once
#include <algorithm>
#include <thread>
#include <vector>

template<typename Iterator, typename Function>
void parallel_for_each(const Iterator& first, const Iterator& last, const Function& l)
{
	const auto nthreads = 12; // std::thread::hardware_concurrency();
	const size_t chunk = (last - first) / nthreads;
	std::vector<std::thread> threads;
	for (Iterator it = first; it < last; it += chunk)
	{
		Iterator begin = it;
		Iterator end = it + chunk;
		if (end > last)
			end = last;

		threads.push_back(std::thread([begin, end, l]()
		{
			for (Iterator i = begin; i != end; ++i)
			{
				l(*i);
			}
		}));
	}
	std::for_each(begin(threads), end(threads), [](std::thread& th) { th.join(); });
}
