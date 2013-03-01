# -*- coding: utf-8 -*-

from random import random

def generate_matrix(size, f):
	m = ""
	for i in xrange(size):
		for j in xrange(size):
			m += str(random()) + " "
		m += "\n"
	
	f.write(str(size) + "\n")
	f.write(m)
	f.close()

def main():
	size = int(input())
	generate_matrix(size, open("matrix1", "w"))
	generate_matrix(size, open("matrix2", "w"))
	print "Matrices generated!"

if __name__ == '__main__':
	main()
