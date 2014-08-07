#!/usr/bin/python
from matplotlib import pyplot as plt

filename = "rcs_test.txt"
args = ["range","velocity","rcs"]
element = 0
data = [[],[],[]]

s = open(filename,"r")
lines = s.readlines()
s.close()

for k in range(len(lines)):
	for m in range(len(args)):
		if args[m] in lines[k]:
			split = lines[k].split(' ')
			if len(split)>2:
				data[m].append(float(split[1+element]))


mean = 0
for k in range(len(data[2])):
	mean = mean+data[2][k]
print "MEAN:", mean/len(data[2])

for k in range(len(args)):
	plt.plot(data[k],'o')
	plt.title(args[k])
	plt.show()
