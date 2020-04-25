import kivy
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from kivy.lang import Builder
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.properties import StringProperty
from functools import partial
from kivy.uix.widget import Widget
from kivy.core.window import Window
from kivy.graphics import *
import math
import random
Window.clearcolor = (1, 1, 1, 1)


Builder.load_string('''
<myCell>:
    canvas:
        Color: #Defines color of the PNG
            rgba: 1, 1, 1, 1
        Rectangle:
            source: "bat.png"
            pos: self.pos
            size: self.size
    Label:
        id:cell_text
        center:root.center
        text: 'Cell'
        color: 0,0,0,1

<Rec>:
    bgcolor:0.0,1.0,0.0,0.8
    canvas:
        Color:
            rgba:self.bgcolor
        Rectangle:
            pos:self.pos
            size:self.size
    Label:
        id:cell_text
        color:0,0,0,1
        text:root.voltage
        text_size: root.width, None
        size: self.texture_size
        pos:root.pos

<ButtonsGalore>:
    grid: Grid
    # BoxLayout:
    #     orientation: 'vertical'
    #     padding: 10
    # Button:
    #     text: 'Top'
    GridLayout:
        size_hint:0.6,1
        id: Grid
        cols: 16
        rows: 6
        spacing: 5
''')

# Configure architecture
segments = 6
series = 16
sTotal = segments * series

# Threshold colours
red =  (1,0,0,0.8) # Less than 2.5V
green =(0,1,0,0.8) # Between 2.5V and 4.2V
blue = (0,0,1,0.8) # Above 4.2V


cValues = [100]*sTotal
for i in range(sTotal):
    cValues[i] = random.randrange(20,45)/10
print(cValues)

class ButtonsGalore(BoxLayout):
    grid = ObjectProperty(None)

class Rec(Widget):
    voltage = StringProperty()
    pass

class myCell(Widget):
    pass

# class allCells(Widget):

#     def __init__(self,**kwargs):
#         super(allCells,self).__init__(**kwargs)
#         self.bind(pos=self.update_canvas)
#         self.bind(size=self.update_canvas)
#         self.update_canvas()

#     def update_canvas(self,*args):
#         # need to reset everything
#         self.canvas.clear()
#         with self.canvas:
#             # Instruction
#             Color(0,1,0,1)

#             self.rect = Rectangle(pos=self.pos,size=self.size)

class cellDisplay(Widget):

    def __init__(self, **kwargs):
        super(cellDisplay, self).__init__(**kwargs)
        # self.cells = 96*[Rectangle(pos=self.center,size=(self.width/4,self.height/4))]
        self.cells = 96*[Rec()]
        
        with self.canvas:

            Color(0,1,0,.5)
            for i in range(len(cValues)):
                val = cValues[i]
                self.cells[i] = Rec()

        self.bind(pos=self.update_canvas,
                  size=self.update_canvas)

    def update_canvas(self, *args):
        xPartition = self.width/16
        yPartition = self.height/6
        hPartition = self.height/6
        wPartition = self.width/10
        for i in range(len(cValues)):
            val = cValues[i]
            x_loc = xPartition*(i%16)+10
            y_loc = yPartition*math.floor(i/16)+15
            h = (hPartition-30)*(val/4.2)
            w = (wPartition-20)
            self.cells[i].pos=(x_loc,y_loc)
            self.cells[i].size=(w,h)
            self.cells[i].voltage = str(val)

            # self.volts[i].pos=(x_loc,y_loc)
            # self.volts[i].text=str(val)

            if(val<2.5):
                self.cells[i].bgcolor = red
            elif(val<4.2):
                self.cells[i].bgcolor = green
            else:
                self.cells[i].bgcolor = blue




class FirstApp(App):

    def disable(self, instance, *args):

        instance.disabled = True

    def update(self, instance, *args):

        instance.text = "I am disabled!"

    def build(self):
        con = FloatLayout(size_hint=(.6,1))
        con.add_widget(cellDisplay())        
        # return con
        g = ButtonsGalore()

        superBox = BoxLayout(orientation='horizontal',padding=10,spacing=10)
        controlBox = BoxLayout(orientation='vertical',size_hint=(.4,1))

        button1 = Button(text='One')
        button2 = Button(text='Two')
        mybtn =  Button(text="Do stuff...hopefully",size_hint=(1,1))
        controlBox.add_widget(mybtn)
        
        for i in range(6*16):
            g.grid.add_widget(myCell())
        
        superBox.add_widget(con)
        # superBox.add_widget(g)
        superBox.add_widget(controlBox)

        # mybtn.bind(on_press=partial(self.disable, mybtn))
        
        # mybtn.bind(on_press=partial(self.update, mybtn))

        return superBox




if __name__ == '__main__':
    FirstApp().run()