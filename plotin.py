import pylab
import math
def plot(file_name):
	f = open(file_name,'r')
	lines = f.readlines()
	count = 0
	results1  = []
	results2  = []
	results3  = []
	cdf1 = []
	current = 0
	temp = 0

	for i in lines:
		results1.append(float(i.split(' ')[1]))
		results2.append(float(i.split(' ')[2]))
		results3.append(float(i.split(' ')[4]))
	results1.sort()
	results2.sort()
	results3.sort()

	temp = 0

	for i in results1 :
		cdf1.append(count/len(results1))
		count += 1.
	count = 0
	
	
	pylab.figure()
	pylab.plot(results1,cdf1,results2,cdf1,results3,cdf1)
	pylab.show()

plot('my_text.txt')
