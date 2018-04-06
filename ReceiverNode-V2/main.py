#!/usr/bin/python 

from flask import Flask, render_template, session, request
from flask_socketio import SocketIO, emit, disconnect
import mysql.connector as mariadb 
import sqlite3
import os.path
import time 
import sys
from random import random
from time import sleep
import threading
from threading import Lock
from datetime import datetime, date
from digi.xbee.devices import XBeeDevice
import myxbee

mUser = 'test'
mPassword = '123'


#Set to either eventlet, gevent, threading or none
async_mode = None 

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app,async_mode=async_mode)
thread = None 
thread_lock = Lock()

PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600
device = XBeeDevice(PORT, BAUD_RATE)
REMOTE_NODE_ID = "REMOTE"

thread_flag = False



def main():
	print(" +------------------------------------------------+")
	print(" |		KENNEL REMOTE SENSOR NETWORK 	  |")
	print(" +------------------------------------------------+\n")
	
	dbconn = mariadb.connect(user = mUser, password = mPassword)
	dbc = dbconn.cursor()
	dbc.execute( ''' CREATE DATABASE IF NOT EXISTS kennel_data''')
	dbconn.commit()
	dbconn.close()
	
	conn = mariadb.connect(user = mUser, password = mPassword, database='kennel_data')
	c = conn.cursor()
	c.execute( ''' CREATE TABLE IF NOT EXISTS users (
					user_id INT PRIMARY KEY AUTO_INCREMENT,
					name	TEXT NOT NULL,
					password TEXT NOT NULL ) ''')

	conn.commit()
	conn.close()
	

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



@app.route('/')
@app.route('/index.html')
def index():
	return render_template('index.html', async_mode=socketio.async_mode)
	
@app.route('/tables.html')
def tables():
	return render_template('tables.html', async_mode=socketio.async_mode)
	
	
@app.route('/charts.html')
def charts():
	return render_template('charts.html', async_mode=socketio.async_mode)
	
	
@app.route('/login.html')
def login():
	return render_template('login.html', async_mode=socketio.async_mode)

	
@socketio.on('connect', namespace='/history_data')
def histConnect():
    # need visibility of the global thread object
	global thread
	with thread_lock:
		if thread is None:
			thread = socketio.start_background_task(target=backgroundHistoryThread)
			thread_flag = True
	emit('my_response', {'data': 'Connected'})
	

@socketio.on('disconnect', namespace='/index_data')
@socketio.on('disconnect', namespace='/history_data')
def testConnect():
	print("Client disconnecting")
	thread_flag = False
	
	
	
@socketio.on('connect', namespace='/index_data')
def indexConnect():
	# need visibility of the global thread object
	global thread
	with thread_lock:
		if thread is None:
			thread = socketio.start_background_task(target=backgroundIndexThread)
			thread_flag = True
	emit('my_response', {'data': 'Connected'})

	
def backgroundHistoryThread():
	numKennels = getNumSensorNodes()
	for i in range(1,numKennels+1):
		data = getDataFromKennel(i,'all')
		socketio.emit('ken_data_push', {'data':data}, namespace='/history_data')
	while (thread_flag):
		time.sleep(1)
	thread.terminate()
			
		
def backgroundIndexThread():
	while (thread_flag):
		numKennels = getNumSensorNodes()
		for i in range(1,numKennels+1):
			data = getDataFromKennel(i,'single')
			print("\n\n\n\n\n\n")
			print(data)
			socketio.emit('ken_data_push', {'data':'test'}, namespace='/index_data')
			# socketio.emit('ken_data_push', {'count':numKennels,'\
			# kennelNum':i,'data':data}, namespace='/index_data')
			time.sleep(3)
	thread.terminate()
					

def getDataFromKennel(num,type):
	kennel = 'kennel'+str(num)
	conn = mariadb.connect(user = mUser, password = mPassword, database='kennel_data')
	c = conn.cursor(buffered=True)
	sql = "SELECT * FROM "+kennel+" ORDER BY timestamp DESC"
	c.execute(sql)
	data = c.fetchall() if type == 'all' else c.fetchone()
	conn.commit()
	conn.close()
	return data
	


if __name__ == '__main__':
	main()
	socketio.run(app, debug=True, host='0.0.0.0')
