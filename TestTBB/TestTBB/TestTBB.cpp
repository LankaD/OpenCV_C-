#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include "tbb/tbb.h"

using namespace std;
using namespace tbb;

float sum2[] = {0};

int main(int argc, char* argv) {
	int n = 10000000;
	float sum = 0;	
	//sum2 = new double[1];
	//sum2[0] = 0;
	DWORD startTime = GetTickCount();
	for (size_t i = 0; i < n; i++)
	{
		sum += i;
	}
	DWORD endTime = GetTickCount();
	cout << "Non-Parallel - Sum is : " << sum << endl;
	cout << "Time elapsed : " << endTime - startTime << "ms" << endl;

	startTime = GetTickCount();
	parallel_for(1, n, 1, [=](int i) {
		sum2[0] += i;
	});
	endTime = GetTickCount();	
	cout << "Parallel - Sum is : " << *sum2<< endl;
	cout << "Time elapsed : " << endTime - startTime <<"ms"<< endl;
	system("pause");
}