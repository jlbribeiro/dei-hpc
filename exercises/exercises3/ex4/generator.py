# -*- coding: utf-8 -*-

import sys
from random import randint

def generate(degree, n_tests):
	
	f = open('input', 'w')

	f.write(str(degree) + ' ')
	for i in xrange(degree + 1):
		x = 0
		while x == 0: 
			x = randint(-10,10)
		f.write(str(x) + ' ')
	f.write('\n')

	f.write(str(n_tests) + '\n')
	for i in xrange(n_tests):
		f.write(str(randint(1,10)) + '\n')

	f.close()


def main():
	if (len(sys.argv) < 3):
		print 'Wrong usage!'
		return
	generate(int(sys.argv[1]), int(sys.argv[2]))

if __name__ == '__main__':
	main()
