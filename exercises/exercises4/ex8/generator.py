#!/usr/bin/env python

import sys
import random

DEFAULT_N = 50000

def main():
	# number of integers to be generated
	n = int(sys.argv[1], 10) if len(sys.argv) >= 2 else DEFAULT_N

	numbers = range(-n, n)
	random.shuffle(numbers)

	for n in numbers:
		print n

if __name__ == "__main__":
	main()