#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random

size = 100000

def main():
	f = open("numbers.txt", "w")
	for x in xrange(2*size):
		f.write(str(random.random()) + '\n')
	f.close()

if __name__ == '__main__':
	main()