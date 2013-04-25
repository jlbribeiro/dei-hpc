#!/usr/bin/env python
# -*- coding: utf-8 -*-

from math import sqrt

def main():
	
	f = open("time")
	times = f.readlines()
	f.close()
	times = times[1::2]
	for i in xrange(len(times)):
		times[i] = float(times[i].split()[2]) / 1000

	N = len(times)
	avg = sum(times) * 1.0 / len(times)
	temp = 0
	for i in times:
		temp += (i - avg)**2
	stdev = sqrt( 1.0 / (N - 1) * temp)

	print 'Samples:', N
	print 'Max:', max(times)
	print 'Min:', min(times)
	print 'Avg:', avg
	print 'Stdev:', stdev

if __name__ == '__main__':
	main()