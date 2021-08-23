from types import SimpleNamespace
from kivy.app import App
from kivy.core import text
from kivy.core.text import FONT_ITALIC
from kivy.graphics import *
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.image import AsyncImage
from kivy.core.window import Window
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.clock import Clock
from kivy.app import async_runTouchApp
from kivy.lang.builder import Builder, Instruction
import can
import asyncio

from kivy.uix.widget import Widget
import time
import math
import datetime

import random

HDD = {1920,1080}
Window.size = HDD

img = 'bat.png'
ico = 'logo.png'
logfile_name = "Unneeded_log_file.csv"

"""These objects hold onto the sensor classes that the CAN messages 
deposit data into"""

log_enabled = False
debug_mode = True

# Configure architecture
segments = 6
series = 16
sTotal = segments * series

CAN_id_start = 64 # through 87

# Threshold colours
underVoltage = (255/255,240/255,36/255,1) # Yellow Less than 2.5V
justRight =    (21/255,232/255,26/255,1) # Green Between 2.5V and 4.2V
overVoltage =  (255/255,43/255,23/255,1) # Red  Above 4.2V

def calculate_height_of_cell(voltage):
    """Returns height out of 100 given a cell voltage"""
    assert 0 <= voltage < 6.554, f"Voltage {voltage} out of feasible range"

    if voltage > 4.2:
        # provide visible discomfort
        return 100

    if 2.5 <= voltage <= 4.2:
        # 90 is max for what fits into the cell figure
        return ((voltage - 2.5) / 1.7) * 70 + 20 
    else:
        return voltage / 2.5 * 20

def calculate_cell_color(voltage):
    assert 0 <= voltage < 6.554
    if voltage <= 2.5:
        return underVoltage
    elif voltage > 4.2:
        return overVoltage
    else:
        return justRight

def unpack_BMS_CAN_message(msg_data):
    cell_1 = ((msg_data[0] << 8) + msg_data[1]) / 10e3
    cell_2 = ((msg_data[2] << 8) + msg_data[3]) / 10e3
    cell_3 = ((msg_data[4] << 8) + msg_data[5]) / 10e3
    cell_4 = ((msg_data[6] << 8) + msg_data[7]) / 10e3

    return (cell_1,cell_2,cell_3,cell_4)


class RootWidget(BoxLayout):
    """This is the base or root widget for the app."""
    orientation = 'vertical'

class Single_Cell(Label):
    """Label for a message coming from the CAN bus."""
    def __init__(self,text_function,position,cell_location,**kwargs):
        """Init  Single_Cell
        """
        super(Single_Cell, self).__init__(**kwargs)
        self.font_size = '30sp'
        self.get_text = text_function
        self.voltage = text_function(cell_location[0],cell_location[1])
        self.text = str(self.voltage)
        self.pos_hint = {"x":position[0],"y":position[1]}
        self.cell_location = cell_location
        self.color = (255,255,255,1)
        self.cell_color = underVoltage
        self.update_figure()

    def update(self, *args):
        """Updates label text."""
        self.voltage = round(self.get_text(self.cell_location[0],self.cell_location[1]),2)
        self.cell_color = calculate_cell_color(self.voltage)
        # self.canvas.clear()
        self.canvas.before.remove(self.fig)
        self.canvas.before.remove(self.cell)
        self.update_figure()
        self.text = str(self.voltage) 
    
    def update_figure(self):
        self.fig = InstructionGroup()
        self.fig.add(Color(self.cell_color[0],self.cell_color[1],self.cell_color[2],self.cell_color[3]))
        self.fig.add(Rectangle(pos = (self.center_x - 100/2 + 5, self.center_y - 100/2),size=(90,calculate_height_of_cell(self.voltage))))
        self.canvas.before.add(self.fig)

        self.cell = InstructionGroup()
        self.cell.add(Rectangle(source = img,pos = (self.center_x - 100/2, self.center_y - 100/2)))
        self.canvas.before.add(self.cell)

class Accumulator_Storage:
    def __init__(self) -> None:
        
        self.cell_data = segments*[series*[0]]
        print(self.cell_data[0][0])
    def intake_message(self,msg):
        msg_id = int(msg.arbitration_id)
        # assert CAN_id_start <= msg_id and msg_id < segments * 4, f'CAN ID {msg_id} out of range'

        segment = math.floor((msg_id - CAN_id_start) / 4) + 1
        assert 1 <= segment and segment <= segments, "segment out of range"

        cell_set = (msg_id - CAN_id_start) % 4 + 1
        assert 1 <= cell_set and cell_set <= 4

        # print(f'segment:{segment} and cell_set:{cell_set}')
        unpacked_cell_voltages = unpack_BMS_CAN_message(msg.data)

        for i in range(4):
            self.cell_data[segment - 1][(cell_set-1)*4 + i] = unpacked_cell_voltages[i]



    def get_cell_voltage(self,seg,series):
        return self.cell_data[seg-1][series-1]
        # return (seg + 1) / segments * 4.2 + .7
        # return random.randint(1,490)/100
        
Accumulator = Accumulator_Storage()

class CustomLayout(FloatLayout):

    def __init__(self, **kwargs):
        # make sure we aren't overriding any important functionality
        super(CustomLayout, self).__init__(**kwargs)

        with self.canvas.before:
            # This is the background color
            Color(1, 1, 1, 1)  # green; colors range from 0-1 instead of 0-255
            self.rect = Rectangle(size=self.size, pos=self.pos)
            # Line(circle = (self.center_x, self.center_y, 50), width = 10)

        # with self.canvas:

        self.bind(size=self._update_rect, pos=self._update_rect)

    def _update_rect(self, instance, value):
        self.rect.pos = instance.pos
        self.rect.size = instance.size

async def run_app_happily(root, other_task):
    '''This method, which runs Kivy, is run by the asyncio loop as one of the
    coroutines. 
    '''
    # we don't actually need to set asyncio as the lib because it is the
    # default, but it doesn't hurt to be explicit
    # await async_runTouchApp(root, async_lib='asyncio')  # run Kivy
    await root.async_run(async_lib='asyncio')
    print('App done')
    # now cancel all the other tasks that may be running
    other_task.cancel()

def sort_CAN_message(CAN_message):
    """Routes CAN message to appropriate station class"""
    CAN_id = CAN_message.arbitration_id

    if(64 <= CAN_id < 88):
        Accumulator.intake_message(CAN_message)

async def can_main():
    """Asyncio handler for handling all of the messages from the CAN bus"""
    try:
        while True:
            if debug_mode:
                can0 = can.Bus('vcan0', bustype='virtual', receive_own_messages=True)
            else:
                can0 = can.interface.Bus(bustype='pcan', channel='PCAN_USBBUS1', bitrate=1e6)
            reader = can.AsyncBufferedReader()

            listeners = [
                # sort_CAN_message,  # Callback function
                reader         # AsyncBufferedReader() listener
            ]
            if log_enabled:
                logger = can.Logger(logfile_name)
                listeners.append(logger)

            # Create Notifier with an explicit loop to use for scheduling of callbacks
            loop = asyncio.get_event_loop()
            notifier = can.Notifier(can0, listeners, loop=loop)
            cell_voltages = [2.5,3.4,4.2,4.5]
            msg_data = []
            for i in range(4):
                binary_voltage = int(cell_voltages[i] * 10e3)
                msg_data.append(binary_voltage >> 8)
                msg_data.append(binary_voltage & 0xff)
            # Start sending first message
            if debug_mode:
                while True:
                    can0.send(can.Message(arbitration_id=64,data=msg_data))
                    print('Bouncing 50 messages...')
                    for _ in range(24):
                        # Wait for next message from AsyncBufferedReader
                        msg = await reader.get_message()
                        sort_CAN_message(msg)
                        CAN_id = msg.arbitration_id
                        await asyncio.sleep(0.01)
                        CAN_id += 1
                        msg = can.Message(arbitration_id=CAN_id,data=msg_data)
                        can0.send(msg)
            else:
                while True:  
                    # Wait for next message from AsyncBufferedReader
                    msg = await reader.get_message()
                    # Sort the messages like a proper mailperson
                    sort_CAN_message(msg)

    except asyncio.CancelledError as e:
        # This catches the 'error' from closing the window 
        print('Wrapping up CAN node and logging:', e)
    finally:
        # when canceled, Clean-up
        notifier.stop()
        can0.shutdown()
        print(f'Data saved at:{logfile_name}')
        print('Done with CAN!')

class MainApp(App):
        # def __init__(self, **kwargs):
    #     super().__init__(**kwargs)

    def build(self):
        self.title = "Olin Electric Motorsports BMS"
        self.icon = ico
        root = RootWidget()
        c = CustomLayout()
        root.add_widget(c)

        # Add Background image
        # c.add_widget(
        #     AsyncImage(
        #         source=imgage_location,
        #         size_hint= (1, 1),
        #         pos_hint={'center_x':.5, 'center_y':.5}))

        # Add dynamic labels
        self.list_of_labels = []
        for seg in range(segments):
            for ser in range(series):
                self.list_of_labels.append(Single_Cell(Accumulator.get_cell_voltage,[-0.47 + ser / series,-0.4 + seg / segments],[seg,ser]))
        
        # Add all labels from list of labels to the float widget
        for lbl in self.list_of_labels:
            c.add_widget(lbl)
        
        # This dicates the frequency at which the app updates the labels
        Clock.schedule_interval(self.update_labels,2)

        # This is just regular kivy syntax for returning the app to run
        return root

    def update_labels(self,*args):
        """Update labels with new data from CAN bus"""
        for lbl in self.list_of_labels:
            lbl.update()

        
    

if __name__ == '__main__':
    def root_func():
        '''This will run both methods asynchronously and then block until they
        are finished
        '''
        can_loop = asyncio.ensure_future(can_main())
        return asyncio.gather(run_app_happily(MainApp(), can_loop), can_loop)

    loop = asyncio.get_event_loop()
    loop.run_until_complete(root_func())
    loop.close()
