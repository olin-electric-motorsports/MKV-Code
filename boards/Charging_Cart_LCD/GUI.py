from kivy.app import App
from kivy.base import runTouchApp
from kivy.lang import Builder
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.properties import ListProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import StringProperty

from kivy.uix.screenmanager import ScreenManager, Screen, FadeTransition

import time
import random
import csv
import numpy as np

Builder.load_file('HomeTouch.kv')

class MainScreen(Screen):
    Grid = StringProperty('Charging:\nVoltage:\nBMS:\nShutdown:\nTSAL:')
    def changeIP(self):
        self.text = 'Change Text Main'
    pass
class ChargingScreen(Screen):
    Grid = StringProperty('Charging1:\nCharging2:\nCharging3:')

    def changeIP(self):
        self.text = 'Change Text Charging'

    pass
class VoltageScreen(Screen):pass
class BMSScreen(Screen):pass
class ShutdownNodesScreen(Screen):pass
class TSALScreen(Screen):pass

class HomeTouch(Button):
    IPLabel = StringProperty("000.000.000.000")

    def changeIP(self):
        self.text = 'poop'




initData = np.array([['Charging', 'Voltage', 'BMS', 'Shutdown', 'TSAL']])

print("initial:\n", str(initData));

nextRow = np.array([0, 0, 0, 0, 0])
newData = initData

with open('test.csv') as file:
    reader = csv.reader(file, delimiter=',')

    count = 0

    #print("resultant:\n", str(newData))

    for row in reader:
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

Builder.load_file('Markup_Main.kv')
Builder.load_file('Markup_Charging.kv')
root_widget = Builder.load_file('Markup_Main.kv')

class ScreenManagerApp(App):
    def build(self):
        return root_widget
        return HomeTouch()

ScreenManagerApp().run()