import mysql.connector as mariadb 
from digi.xbee.devices import XBeeDevice
import digi.xbee.exception
from datetime import datetime, date
import serial
import threading
import time 

mUser = 'test'
mPassword = '123'

PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600
device = XBeeDevice(PORT, BAUD_RATE)
REMOTE_NODE_ID = "REMOTE"

# def main():
	# print(" +-------------------------------------------------+")
	# print(" | 		XBEE RECEIVER NODE					 |")
	# print(" +-------------------------------------------------+\n")

	# try:
		# device.open()
		# device.flush_queues()
		# print("Waiting for data...\n")

		# while True:
			# xbeeReceive()
	# finally:
		# if device is not None and device.is_open():
			# device.close()
			
			
def xbeeReceive():
	conn = mariadb.connect(user = mUser, password = mPassword, database='kennel_data')
	c = conn.cursor()
	xbee_message = device.read_data()
	if xbee_message is not None:
		data = xbee_message.data.decode().split(',')
		temp = float(data[0])
		humidity = float(data[1])
		presence = float(str(data[2]).replace("\x00",""))
		addr = str(xbee_message.remote_device.get_64bit_addr())
		
		print("RECEIVED MSG")
		
		kennelNum = isSensorInNetwork(addr)
		
		print ("message from kennel " + str(kennelNum))
		
		kennelData = (addr,temp,humidity,presence,datetime.now())

		if not kennelNum:
			print ("no table exists for msg")
			kennel = "kennel" + str((getNumSensorNodes() + 1))
			sql = "CREATE TABLE IF NOT EXISTS " + kennel + ''' (
				msgID INT AUTO_INCREMENT PRIMARY KEY,
				sensorID CHAR(16) NOT NULL,
				temp FLOAT,
				humidity FLOAT,
				presence FLOAT,
				timestamp TEXT ) '''
			print(sql)
			c.execute(sql)
		else:
			print ("kennel table exists")
			kennel = "kennel"+ str(kennelNum)
		print (kennel)
		sql = "INSERT INTO " + kennel + "(sensorID,temp,humidity,presence,timestamp) \
				VALUES ('%s', '%f', '%f', '%f', '%s')" % (addr,temp,humidity,presence,str(datetime.now()))
		c.execute(sql)	
		
	conn.commit()
	conn.close()


def isSensorInNetwork(addr):
	print ("search for kennel id")
	conn = mariadb.connect(user = mUser, password = mPassword, database='kennel_data')
	c = conn.cursor(buffered = True)
	i = 1
	matchFound = False
	pos = 0
	while i <= getNumSensorNodes():
		kennel = "kennel"+str(i)
		print (kennel)
		print("looking in kennel " + str(i))
		sql = "SELECT * FROM " +kennel+ " WHERE sensorID = '"+addr+"'"
		print(sql)
		c.execute(sql)
		if c.rowcount:
			print("match found")
			pos = i
			matchFound = True
		i += 1
	conn.commit()
	conn.close()
	if matchFound:
		return pos
	return pos
		
		
		

def getNumSensorNodes():
	conn = mariadb.connect(user = mUser, password = mPassword, database='information_schema')
	c = conn.cursor()
	c.execute('''SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = 'kennel_data' ''')
	tableNum = int(c.fetchone()[0])
	print ("num tables:" + str(tableNum))
	conn.commit()
	conn.close()
	print (tableNum -1)
	return (tableNum - 1)
	
	
def xbeeSend(REMOTE_NODE_ID, DATA_TO_SEND):
	if REMOTE_NODE_ID is None:
		print("Sending broadcast data: %s..." % DATA_TO_SEND)
		device.send_data_broadcast(DATA_TO_SEND)
	else:
		print("Sending data to %s >> %s..." % (remote_device.get_64bit_addr(), DATA_TO_SEND))
		device.send_data(remote_device, DATA_TO_SEND)

	print("Success")

# if __name__ == '__main__':
	# main()

def run():
	print(" +-------------------------------------------------+")
	print(" | 		XBEE RECEIVER NODE					 |")
	print(" +-------------------------------------------------+\n")

	while True:
		try:
			device.open()
			device.flush_queues()
			print("Waiting for data...\n")

			while True:
				xbeeReceive()
		except (serial.serialutil.SerialException, digi.xbee.exception.InvalidOperatingModeException):
			print("Error setting up xbee module")
			time.sleep(2)
			break
			
		finally:
			if device is not None and device.is_open():
				device.close()
				break
	
thread = threading.Thread(target=run, daemon=True)
thread.start()
