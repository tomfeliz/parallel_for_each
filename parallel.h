#pragma once
#include <algorithm>
#include <thread>
#include <vector>

// Template parameters will be inferred in most cases.
template<typename Iterator, typename Function>
void parallel_for_each(const Iterator& first, const Iterator& last, const Function& l)
{
	// Create threads based on number of virtual cores.
	const auto numThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;

	// Chunk is the size of each batch.
	const auto chunk = std::max(static_cast<int>((last - first) / numThreads), 1);

	Iterator it = first;
	auto moreWork = true;
	do
	{
		// Calculate iterator range (be sure not to overrun).
		Iterator start = it;
		Iterator end = (last - start >= chunk) ? start + chunk : last;

		// Create threads and store reference in thread pool.
		threads.push_back(std::thread([start, end, l]()
		{
			for (Iterator i = start; i != end; ++i)
			{
				l(*i); // i is an iterator, so must de-reference.
			}
		}));

		// Determine if we're done. Only increment 'it' if there's room (avoid overruns).
		if ((last - start) >= chunk)
			it += chunk;
		else
			moreWork = false;

	} while (moreWork);
	
	// Wait for all threads to complete.
	std::for_each(begin(threads), end(threads), [](std::thread& th)
	{
		th.join();
	});
}
