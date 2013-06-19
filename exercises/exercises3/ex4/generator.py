# -*- coding: utf-8 -*-

from random import randint

def main():
	
	f = open('input', 'w')

	degree = randint(5,10)
	f.write(str(degree) + ' ')
	for i in xrange(degree + 1):
		x = 0
		while x == 0: 
			x = randint(-10,10)
		f.write(str(x) + ' ')
	f.write('\n')

	n_tests = randint(5,10)
	f.write(str(n_tests) + '\n')
	for i in xrange(n_tests):
		f.write(str(randint(1,10)) + '\n')

	f.close()


if __name__ == '__main__':
	main()
