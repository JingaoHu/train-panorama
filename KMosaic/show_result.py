import string
import matplotlib.pyplot as plt

f=open("match_result.txt")
a=[]
count=0
line=f.readline()
while line:
	i=string.atoi(line)
	a.append(i)
	count=count+1
	line=f.readline()
b=xrange(count)
f.close()

Fig=plt.figure()
Ax=Fig.add_subplot(111)
Ax.plot(b,a,'ro')
Fig.show()
wait=raw_input()
