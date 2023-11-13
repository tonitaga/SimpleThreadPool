#include <thread_pool/thread_pool.h>

#include <iostream>

int main() {
  pool::thread_pool pool(16, pool::stop_type::waiting);

  int count = 0;
  for (int i = 0; i != 100500; ++i) {
	pool.submit([&]{
	  count++;
	}, pool::submit_type::immediate);
  }

  pool.wait();

  pool.submit([&]{
	std::cout << count << std::endl;
  });

  return EXIT_SUCCESS;
}