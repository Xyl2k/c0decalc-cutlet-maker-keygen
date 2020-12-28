#include <iostream>
#include <cstdio>

// Keygen for Cutlet Maker
// 2018, By Xyl, cybercrime-tracker.net

using namespace std;

int rol(int x, int n) {
	int y = x << n | x >> (32 - n);

	for (int i = 0; i < n; i++)
		x = ((x & 0x80) ? 0x01 : 0x00) | (x << 1);

	x = x & 0xFFFF0000;
	y = y & 0x0000FFFF;

	x = x + y;
	return x;
}

int main(int argc, const char** argv)
{
	int nCode = 0;
	cout << "Input Code: ";
	cin >> nCode;

	int nAnswer = rol(0x75BCD17 * (~(nCode & 0xABD13D59) & ~(~nCode & 0x542EC2A6)), 16);
	nAnswer = 0xDFB38D3u * nAnswer % 0x5F5E100;

	cout << "Output Answer: " << nAnswer << endl;
	return 0;
}