# -*- coding: utf-8 -*-

import matplotlib.pyplot as ppt

def show(times):
	n_machines = [i+1 for i in range(1, 9)]
	ppt.plot(n_machines, times, 'blue')
	ppt.show()

def avg(l):
	return sum(l)*1.0/len(l)

def main():
	avg_time = []
	for i in range(8):

		f = open("../stats/v1.0/" + str(i+2) + "machines")
		times = f.readlines()
		f.close()

		for j in range(len(times)):
			times[j] = int(times[j][:-3])/1000

		for j in range(5):
			times.remove(max(times))

		avg_time.append(avg(times))

		print str(i+2) + ' is done!'

	print avg_time

	ppt.plot([i for i in range(2, 10)], avg_time, 'blue')
	ppt.show()

if __name__ == '__main__':
	main()
