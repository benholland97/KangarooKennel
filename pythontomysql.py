#!/usr/bin/python 
import mysql.connector as mariadb 
import time 
mariadb_connection = mariadb.connect(user='gdpuser', password='newuser', database='dogs') 
cursor = mariadb_connection.cursor() 

while True: 
x=15 #Temperature 
y=35 #Humidity 
pres='y' #Presence 
cursor.execute("insert into kennel1 (temp,humi,presence) values (%s,%s,%s)",(x,y,pres)) 
mariadb_connection.commit() 
time.sleep(10)