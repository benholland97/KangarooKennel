#!/usr/bin/python 
import sqlite3
import os.path
import time 
import sys
from datetime import datetime, date
from digi.xbee.devices import XBeeDevice

# TODO: Replace with the serial port where your local module is connected to. 
PORT = "/dev/ttyUSB0"
# TODO: Replace with the baud rate of your local module.
BAUD_RATE = 9600



REMOTE_NODE_ID = "REMOTE"

device = XBeeDevice(PORT, BAUD_RATE)



def main():
	print(" +------------------------------------------------+")
	print(" |		KENNEL REMOTE SENSOR NETWORK 	  |")
	print(" +------------------------------------------------+\n")
	
	if (os.path.isfile('db/kenneldata.db') == False):
		conn = sqlite3.connect('db/kenneldata.db')
		c = conn.cursor()
		c.execute( ''' CREATE TABLE IF NOT EXISTS sensor_data (
						id INTEGER PRIMARY KEY AUTOINCREMENT,
						temp NUMERIC,
						humidity NUMERIC,
						presence INTEGER,
						sensor TEXT,
						[timestamp] timestamp ) ''')
		conn.commit()
		conn.close()


	try:
		DATA_TO_SEND = "Hello XBee!"
		mode = 0
		device.open()
		device.flush_queues()
		
		if len(sys.argv) > 1:
			if sys.argv[1] == "m":
				mode = 2
			else:
				print (sys.argv[1])
				DATA_TO_SEND = str(sys.argv[1])
				mode = 1
				REMOTE_NODE_ID = None 
					
		print('Received messages displayed below')
		
		while True:
			if mode == 0 :
				xbeeReceive()
			elif mode == 1:
				xbeeSend(REMOTE_NODE_ID, DATA_TO_SEND)
				time.sleep(3)
				
			elif mode == 2:
				viewDB()
				break
				
			
	finally:
		if device is not None and device.is_open():
			device.close()
		
def xbeeSend(REMOTE_NODE_ID, DATA_TO_SEND):
	if REMOTE_NODE_ID is None:
		print("Sending broadcast data: %s..." % DATA_TO_SEND)
		device.send_data_broadcast(DATA_TO_SEND)
	else:
		print("Sending data to %s >> %s..." % (remote_device.get_64bit_addr(), DATA_TO_SEND))
		device.send_data(remote_device, DATA_TO_SEND)

	print("Success")
	
def viewDB():
	conn = sqlite3.connect('db/kenneldata.db')
	c = conn.cursor()
	c.execute('''SELECT * FROM sensor_data ORDER BY timestamp DESC''')
	data = c.fetchall()
	for d in data:
		print (d)
		print("")



		
		

def xbeeReceive():
	conn = sqlite3.connect('db/kenneldata.db')
	c = conn.cursor()
	xbee_message = device.read_data()
	if xbee_message is not None:
		print("From %s >> %s" % (xbee_message.remote_device.get_64bit_addr(),
								 xbee_message.data.decode()))
		data = xbee_message.data.decode().split(',')
		temp = float(data[0])
		humidity = float(data[1])
		presence = float(data[2])
		
		# date = datetime.datetime.now().year
		# time = datetime.datetime.now().time
		addr = str(xbee_message.remote_device.get_64bit_addr())
		c.execute('''INSERT INTO sensor_data (temp,humidity,presence,sensor,timestamp) \
				VALUES(?,?,?,?,?)''',(temp,humidity,presence,addr,datetime.now()))
		conn.commit()
		conn.close()


if __name__ == "__main__":
    main()