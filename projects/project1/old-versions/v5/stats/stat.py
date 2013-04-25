#!/usr/bin/env python
# -*- coding: utf-8 -*-

from math import sqrt

def main():
	times_per_cores = []
	for cores in xrange(1, 5):
		f = open("time_{0}core{1}".format(cores, "s" if cores > 1 else ""))
		tps = f.readlines()
		f.close()

		time_per_cores_sum = 0
		for i in xrange(len(tps)):
			time, ntps = tps[i].split()

			tps[i] = float(ntps)
			time_per_cores_sum += float(time)

		N = len(tps)

		times_per_cores.append(time_per_cores_sum / N)

		avg = sum(tps) * 1.0 / len(tps)
		temp = 0
		for i in tps:
			temp += (i - avg)**2
		stdev = sqrt( 1.0 / (N - 1) * temp)

		title = "{0} core{1}".format(cores, "s" if cores > 1 else "")
		print title
		print "-" * len(title)
		print "Samples:", N
		print "Max:", max(tps)
		print "Min:", min(tps)
		print "Avg:", avg
		print "Stdev:", stdev
		print ""

	print ""
	for i in xrange(1, len(times_per_cores)):
		print "Speedup with {0} cores: {1:.3f}".format(i + 1, times_per_cores[0] / times_per_cores[i])

	for time in times_per_cores:
		print "{0:.3f}".format(time),

if __name__ == "__main__":
	main()