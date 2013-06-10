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
	for i in range(9):

		f = open("stats/" + str(i+2) + "machines")
		times = f.readlines()
		f.close()

		for i in range(len(times)):
			times[i] = int(times[i][:-3])/1000

		for i in range(5):
			times.remove(max(times))

		avg_time.append(avg(times))

	ppt.plot([i for i in range(2, 10)], avg_time, 'blue')
	ppt.show()

if __name__ == '__main__':
	main()