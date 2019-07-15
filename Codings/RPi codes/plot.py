import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)


def animate(i):
	graph_data = open('test.txt','r').read()
	lines = graph_data.split('\n')
	xs = []
	ys = []
	for line in lines:
    		if len(line) > 1:
        		x, y = line.split(',')
        		xs.append(float(x))
        		ys.append(float(y))
	ax1.clear()
	ax1.scatter(xs,ys)

ani = animation.FuncAnimation(fig, animate, interval=500)

#print ys
#plt.title('  ')
#plt.ylabel('Voltage (V)')
#plt.xlabel('Time (us)')
#plt.scatter(xs,ys,linewidth=0.8)
#plt.ylim(ymax=-0.04)
#plt.ylim(ymin=-0.08)
plt.show()