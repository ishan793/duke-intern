import pylab
import math
def plot(file_name):
	f = open(file_name,'r')
	lines = f.readlines()
	count = 0
	results  = []
	cdf = []
	current = 0
	temp = 0

	for i in lines:
		temp = i.split(' ')[1]
		results.append(float(temp))
	results.sort()
	
	temp = 0

	for i in results :
		cdf.append(count/len(results))
		count += 1.
	pylab.figure()
	pylab.plot(results,cdf)
	pylab.show()

plot('my_text.txt')
