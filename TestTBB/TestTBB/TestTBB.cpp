#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"

using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::ofstream;
using std::ifstream;
using namespace tbb;

const int list_count = 1000000; //Adjust to suit your system's power.
int *data = new int[list_count];

//This code builds an array of size list_count consisting of the numbers 1-1,000,000 repeated as 
//many times as needed.  This array will be used as the dataset for the find_primes and 
//count_primes functions.
void make_array() {
	int multiplier = 1;
	for (int i = 0; i < list_count; i++) {
		if (i % 1000000 == 0) {
			multiplier = 1;
		}
		data[i] = multiplier;
		multiplier++;
	}
	cout << "Array created." << endl;
}


//This is a helper function that is not called.
//This function does some a simple transformation on its input and returns a result.  It is
//just used to provide some work for the processors in the change_array function.
int busyfunc(int x) {
	double y = x / M_PI;
	y = sqrt(y);
	return int(y);
}

//This function is provided for reference and not called.  
//Change_array and parallel_change_array can be used as a model when converting find_primes
//to parallel_find_primes.
//This function will perform some work on each element in an array.  It is the serial version 
//of a simple, instructional function that will be converted to use TBB parallel_for.
void change_array() {
	for (int i = 0; i < list_count; i++) {
		data[i] = busyfunc(data[i]);
	}
}

//This function is provided for reference and not called.
//Change_array and parallel_change_array can be used as a model when converting find_primes
//to parallel_find_primes.
//The parallel version of change_array, using TBB parallel_for with a lambda operator.
//The parallel_for takes the for loop, breaks its work into tasks, and spreads the
//tasks across all the available cores.  No other code (besides include directives) 
//is needed for this!  This parallel_for is coded in the default (simplest) format, with 
//no special options.  There are many parameters that can be modified to tune performance.  
//TBB parallel_for also supports a second format that does not require lambda operators 
//and so will work with additional compilers.  For more information see the TBB Tutorial.
void parallel_change_array() {
	parallel_for(blocked_range<int>(0, list_count),
		//The first parameter for parallel_for is a blocked_range.
		//The blocked_range object specifies a the total range of the for loop, in 
		//this case, 0 to list_count.  The parallel_for implementation will create 
		//many tasks, each operating over a portion of the range.  It then schedules
		//tasks on threads in real-time, while balancing load.
		[=](const blocked_range<int>& r) {
		//The second parameter for parallel_for is a lambda function that 
		//defines the work of each task.  Each task will iterate through 
		//its assigned range, doing the user-defined task.
		for (int i = r.begin(); i != r.end(); i++) {
			data[i] = busyfunc(data[i]);
		}
	});
}

//This function goes through the array and determines if each number is prime.  If a number 
//at a given index is prime, a 1 is placed into the prime_array parameter at the same index.
//Otherwise a 0 is placed into the prime_array at the same index.  
void find_primes(int*  &my_array, int *&prime_array) {
	int prime, factor, limit;
	for (int list = 0; list < list_count; list++) {
		prime = 1;
		if ((my_array[list] % 2) == 1) {
			limit = (int)sqrt((float)my_array[list]) + 1;
			factor = 3;
			while (prime && (factor <= limit)) {
				if (my_array[list] % factor == 0) prime = 0;
				factor += 2;
			}
		}
		else prime = 0;
		if (prime) {
			prime_array[list] = 1;
		}
		else
			prime_array[list] = 0;
	}
}

//The parallel version of find_primes, using TBB parallel_for with a lambda operator.
//See the change_array_parallel function for another example along with an explanation.
void parallel_find_primes(int *&my_array, int *& prime_array) {
	parallel_for(blocked_range<int>(0, list_count),
		[=](const blocked_range<int>& r) {
		int prime, factor, limit;
		for (int list = r.begin(); list != r.end(); list++) {
			prime = 1;
			if ((my_array[list] % 2) == 1) {
				limit = (int)sqrt((float)my_array[list]) + 1;
				factor = 3;
				while (prime && (factor <= limit)) {
					if (my_array[list] % factor == 0) prime = 0;
					factor += 2;
				}
			}
			else prime = 0;
			if (prime)
				prime_array[list] = 1;
			else
				prime_array[list] = 0;
		}
	});
}

//This function goes through the given array and for each number, determines if it is prime.  If 
//it is prime, a counter is incremented.  The total number of primes is returned.
int count_primes(int*  &my_array) {
	int prime, factor, limit, numprimes = 0;
	for (int list = 0; list < list_count; list++) {
		prime = 1;
		if ((my_array[list] % 2) == 1) {
			limit = (int)sqrt((float)my_array[list]) + 1;
			factor = 3;
			while (prime && (factor <= limit)) {
				if (my_array[list] % factor == 0) prime = 0;
				factor += 2;
			}
		}
		else prime = 0;
		if (prime) {
			numprimes++;
		}
	}
	return numprimes;
}

//The parallel version of count_primes, using TBB parallel_reduce with a lambda operator.
//Parallel_reduce must be used with this for loop since it is a total is being accumulated.
//Like the parallel_for, the parallel_reduce takes the for loop, breaks its work into tasks, 
//and spreads the tasks across all the available cores.  However in the case of a reduction, 
//each task returns a value, and the values are combined together to give a final result.
//In order to use TBB parallel_reduce you must define the work of each task (the first 
//lambda function below), and also how to combine the values returned from each task (the
//(second lambda function below), assuming 2 tasks values at a time will be combined.
int parallel_count_primes(int*  &my_array) {
	return parallel_reduce(blocked_range<int>(0, list_count),
		//The first parameter to parallel_reduce is a blocked_range.
		//See the change_array_parallel function for a description.
		0,
		//The second parameter is the initial value for the result that parallel_reduce
		//will return.  In this case, the total number of primes will be returned, and 
		//the value should start at 0.
		[=](const blocked_range<int>& r, int init)->int {
		//The third parameter is a lambda function defining the work to be done
		//by each task.  This lambda function must take as its parameters a
		//blocked_range and a variable to hold the result.  The parallel_reduce
		//implementation passes in the proper values for each of these parameters
		//to each task.
		int prime, factor, limit;
		for (int list = r.begin(); list != r.end(); list++) {
			prime = 1;
			if ((my_array[list] % 2) == 1) {
				limit = (int)sqrt((float)my_array[list]) + 1;
				factor = 3;
				while (prime && (factor <= limit)) {
					if (my_array[list] % factor == 0) prime = 0;
					factor += 2;
				}
			}
			else prime = 0;
			if (prime) {
				init++;
			}
		}
		return init;  //Each task returns a value.
	},
		[](int x, int y)->int {
		//The fourth parameter is a lambda function defining how the return values of
		//each task should be combined into one result.  The values will be combined
		//2 at a time.  In this case, since the function is totaling, the values should
		//be added.
		return x + y;
	}
	);
}

int _tmain(int argc, _TCHAR* argv[])
{
	char x;
	int *isprime = new int[list_count];
	cout << "Creating array of " << list_count << " numbers." << endl;
	make_array();

	cout << "Finding primes serially.  This may take a few seconds. " << endl;
	tick_count serial_prime_start = tick_count::now();
	//Uses TBB's tick_count class for thread-safe timing.
	find_primes(data, isprime);
	tick_count serial_prime_end = tick_count::now();
	cout << "Time to find primes serially for " << list_count << " numbers: " << (serial_prime_end - serial_prime_start).seconds() << " seconds." << endl;

	cout << "Counting primes serially.  This may take a few seconds. " << endl;
	serial_prime_start = tick_count::now();
	cout << "The total number of primes is " << count_primes(data) << endl;
	serial_prime_end = tick_count::now();
	cout << "Time to count primes serially for " << list_count << " numbers: " << (serial_prime_end - serial_prime_start).seconds() << " seconds." << endl;


	//task_scheduler_init init(2);  //Not necessary, but can be used to set the number of processing cores.  
	//If not set, TBB will use all cores present on the system.

	cout << "Finding primes in parallel.  This may take a few seconds. " << endl;
	tick_count parallel_prime_start = tick_count::now();
	parallel_find_primes(data, isprime);
	tick_count parallel_prime_end = tick_count::now();
	cout << "Time to find primes in parallel for " << list_count << " numbers: " << (parallel_prime_end - parallel_prime_start).seconds() << " seconds." << endl;


	cout << "Counting primes in parallel.  This may take a few seconds. " << endl;
	parallel_prime_start = tick_count::now();
	cout << "The total number of primes is " << parallel_count_primes(data) << endl;
	parallel_prime_end = tick_count::now();
	cout << "Time to count primes in parallel for " << list_count << " numbers: " << (parallel_prime_end - parallel_prime_start).seconds() << " seconds." << endl;

	//Make the console window wait.
	cin >> x;

	return 0;
}