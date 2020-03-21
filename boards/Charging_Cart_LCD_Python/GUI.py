import time
import random
import csv
import numpy as np
from kivy.app import App
from kivy.base import runTouchApp
from kivy.lang import Builder
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.properties import ListProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import StringProperty
from kivy.uix.screenmanager import ScreenManager, Screen, FadeTransition


initData = np.array([['Charging', 'Voltage', 'BMS', 'Shutdown', 'TSAL']])

print("initial:\n", str(initData));

nextRow = np.array([0, 0, 0, 0, 0])
newData = initData

class MainScreen(Screen):
    Grid = StringProperty(str(newData))
    def updateDisplay(self):
        self.text = str(newData)
    pass

class ChargingScreen(Screen):
    Grid = StringProperty('Charging1: 0\nCharging2: 0\nCharging3: 0')
    def updateDisplay(self):
        self.text = 'Change Text Charging'
    pass

class VoltageScreen(Screen):
    Grid = StringProperty('Voltage1: 0\nVoltage2: 0\nVoltage3: 0')
    def updateDisplay(self):
        self.text = 'Change Text Charging'
    pass
class BMSScreen(Screen):pass
class ShutdownNodesScreen(Screen):pass
class TSALScreen(Screen):pass






with open('test.csv') as file:
    readerFile = csv.reader(file, delimiter=',')

    count = 0
    #rowCount = len(list(readerFile))

    for row in readerFile:
        nextRow = row
        newData = np.vstack((nextRow, newData))
        print("update:\n", str(newData))

        if count > 5:
            break

        count += 1


class MyScreenManager(ScreenManager):
    def new_colour_screen(self):
        name = str(time.time())
        '''
        s = ColourScreen(name=name,
                         colour=[random.random() for _ in range(3)] + [1])
        '''
        self.add_widget(s)
        self.current = name

Builder.load_file('Markup_Charging.kv')
Builder.load_file('Markup_Voltage.kv')
root_widget = Builder.load_file('Markup_Main.kv')

class ScreenManagerApp(App):
    def build(self):
        return root_widget
        return HomeTouch()

ScreenManagerApp().run()
