#!/usr/bin/python
from matplotlib import pyplot as plt

filename = "store_msgs.txt"
args = ["range","velocity"]
element = 0
data = [[],[]]

s = open(filename,"r")
lines = s.readlines()
s.close()

for k in range(len(lines)):
	for m in range(len(args)):
		if args[m] in lines[k]:
			split = lines[k].split(' ')
			if len(split)>2:
				data[m].append(float(split[1+element]))


for k in range(len(args)):
	plt.plot(data[k],'o')
	plt.title(args[k])
	plt.show()
