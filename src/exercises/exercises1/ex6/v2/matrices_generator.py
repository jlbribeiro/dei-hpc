# -*- coding: utf-8 -*-

from random import random

def generate_matrix(size):
	m = ""
	for i in xrange(size):
		for j in xrange(size):
			m += str(random()) + " "
		m += "\n"

	return m

def main():
	size = int(input("Matrices size: "))

	f = open("matrices.in", "w")

	f.write(str(size) + "\n")

	f.write(generate_matrix(size))
	f.write(generate_matrix(size))

	f.close()

	print "Matrices generated!"

if __name__ == '__main__':
	main()
