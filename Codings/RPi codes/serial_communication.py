import serial
import matplotlib.pyplot as plt

file = open("test.txt", "w")
ser = serial.Serial('/dev/ttyACM0',9600)

while True:
	read_serial=ser.readline()
	if read_serial == "******\r\n":
		break
	file.write(read_serial)
	print read_serial
	file.close()
	file = open("test.txt", "a")

file.close()

#graph_data = open('test.txt','r').read()
#lines = graph_data.split('\n')
#xs = []
#ys = []
#for line in lines:
#    if len(line) > 1:
#        x, y = line.split(',')
#        xs.append(float(x))
#        ys.append(float(y))

#plt.scatter(xs,ys)
#plt.show()