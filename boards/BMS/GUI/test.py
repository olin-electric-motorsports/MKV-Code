


import kivy

from kivy.app import App

from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from kivy.lang import Builder
# from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from functools import partial

Builder.load_string('''
<ButtonsGalore>:
    grid: Grid
    BoxLayout:
        orientation: 'vertical'
    Button:
        text: 'Top'
    GridLayout:
        id: Grid
        cols: 16
        rows: 6
''')

blue = (54,86,145,255)
pink = (155,0,51,53)

class ButtonsGalore(BoxLayout):
    grid = ObjectProperty(None)

class FirstApp(App):

    def disable(self, instance, *args):

        instance.disabled = True

    def update(self, instance, *args):

        instance.text = "I am disabled!"

    def build(self):
        g = ButtonsGalore()

        superBox = BoxLayout(orientation='horizontal')

        button1 = Button(text='One')
        button2 = Button(text='Two')
        mybtn =  Button(text="Why Hello There!",pos=(300,350),size_hint=(.25,.18))

        superBox.add_widget(button1)
        for i in range(24):
            g.grid.add_widget(Button(text='test'))
        return g
  
        # mybtn.bind(on_press=partial(self.disable, mybtn))
        
        # mybtn.bind(on_press=partial(self.update, mybtn))

        return superBox




if __name__ == '__main__':
    FirstApp().run()