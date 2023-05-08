
from  gait_tracking import *
from PyQt5.QtWidgets import QApplication,QMainWindow,QListWidgetItem, QWidget
from PyQt5.uic import loadUi
import sys
from PyQt5.QtCore import Qt
from PyQt5 import QtCore
import time


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
        self.pushButton.clicked.connect(self.pushButton_pushed)

    def pushButton_pushed(self):
        self.thread = Thread(self)
        # self.thread.started.connect(self.show_processing)
        self.thread.started.connect(process)
        self.thread.start()
        time.sleep(1)

    def show_processing(self):
        self.pushButton.setText("Processing")
        self.pushButton.setEnabled(False)



  #self.pushButton.setText("Start Processing")
class Thread(QtCore.QThread):

    def run(self):
        print("Button Clicked")





       



if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Main()
    window.show()
    app.exec_()