
from  gait_tracking import *
from PyQt5.QtWidgets import QApplication,QMainWindow,QListWidgetItem, QWidget
from PyQt5.uic import loadUi
import sys
from PyQt5.QtCore import Qt
from PyQt5 import QtCore
import time
import threading 
import serial
import csv

status_flag = False
get_data_flag = False
start_session_flag = False
stop_session_flag = False
lines=[]
class Main(QMainWindow):

    def __init__(self):
    #loading the super class consctructor
        super(Main, self).__init__()
        loadUi("main.ui",self)
        # todos= ["hello", "legend"]
        # for todo in todos:
        #     item  = QListWidgetItem(todo)
        #     item.setFlags( item.flags() | Qt.ItemIsUserCheckable)
        #     item.setCheckState(Qt.Unchecked)
        #     #mohamed_list is the id of the widget list object that identify it 
        #     #we can change it from qt designer
        #     self.mohamed_list.addItem(item)  
        self.start_processing.clicked.connect(self.start_processing_pushed)
        self.start_getting.clicked.connect(self.start_getting_pushed)
        self.stop_session.clicked.connect(self.stop_session_pushed)
        self.start_session.clicked.connect(self.start_session_pushed)

    def start_processing_pushed(self):
        print("Button Clicked")
        global status_flag
        
        status_flag = True
        self.show_processing()

    def start_getting_pushed(self):
        print("Button Clicked")
        global get_data_flag
        global status_flag
        status_flag = True
        get_data_flag = True


    def stop_session_pushed(self):
        print("Button Clicked")
        global stop_session_flag
        global status_flag
        stop_session_flag = True
        status_flag = True

    def start_session_pushed(self):
        print("Button Clicked")
        global status_flag
        global start_session_flag
        status_flag = True
        start_session_flag = True
    def show_processing(self):
        process()
        # self.pushButton.setText("Processing")
        # self.pushButton.setEnabled(False)




  #self.pushButton.setText("Start Processing")
class Thread(QtCore.QThread):

    def run(self):
        print("Button Clicked")


def start_app():
    app = QApplication(sys.argv)
    window = Main()
    window.show()
    app.exec_()

def start_serial():
    global start_session_flag
    global stop_session_flag
    global get_data_flag
    global lines
    master = serial.Serial('COM3', 115200, timeout=1)
    #getting data
    while True:

        append_data = False
        if master.inWaiting():
            line = master.readline()
            line=line.decode("utf-8", "replace")
            line = line.replace('\n','')
            line = line.strip()
            print(line)
            if line == "END":
                append_data = True
                
            elif(len(line)>0  and line[0].isdigit()):
                lines.append(line)
        ### write the data when device send all the data
        if append_data==True:
            print("start edit the file")
            with open("test.csv","w", newline='') as f:
                print("we opened the file")
                writer = csv.writer(f,quoting=csv.QUOTE_NONE,delimiter=" ")
                for line in lines:
                    decoded_bytes = line.replace(' ',',')
                    decoded_bytes = decoded_bytes.split('*')
                    print("saving data", decoded_bytes)
                    writer.writerow(decoded_bytes)
                lines = []


        elif start_session_flag == True:
            print("start session")
            master.write("Start".encode())
            start_session_flag = False
        elif stop_session_flag == True:
            print("stop session")
            master.write("Stop".encode())
            stop_session_flag = False
        elif get_data_flag == True:
            print("start getting data")
            master.write("Send".encode())
            get_data_flag = False

if __name__ == '__main__':
    t1 = threading.Thread(target=start_serial, args=())
   # starting thread 1
    t1.start()  
    start_app()
