import pandas
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout

from kivy.uix.label import Label
from kivy.uix.recycleview.views import RecycleDataViewBehavior
from kivy.uix.button import Button
from kivy.properties import BooleanProperty, ListProperty, ObjectProperty
from kivy.uix.recyclegridlayout import RecycleGridLayout
from kivy.uix.behaviors import FocusBehavior
from kivy.uix.recycleview.layout import LayoutSelectionBehavior
from kivy.core.window import Window

data = []
class SelectableRecycleGridLayout(FocusBehavior, LayoutSelectionBehavior,
                                  RecycleGridLayout):
    ''' Adds selection and focus behaviour to the view. '''


class SelectableButton(RecycleDataViewBehavior, Button):
    ''' Add selection support to the Label '''
    index = None
    selected = BooleanProperty(False)
    selectable = BooleanProperty(True)

    def refresh_view_attrs(self, rv, index, data):
        ''' Catch and handle the view changes '''
        self.index = index
        return super(SelectableButton, self).refresh_view_attrs(
            rv, index, data)

    def on_touch_down(self, touch):
        ''' Add selection on touch down '''
        if super(SelectableButton, self).on_touch_down(touch):
            return True
        if self.collide_point(*touch.pos) and self.selectable:
            return self.parent.select_with_touch(self.index, touch)

    def apply_selection(self, rv, index, is_selected):
        ''' Respond to the selection of items in the view. '''
        self.selected = is_selected




class QuestionDb(BoxLayout):
    items_list = ObjectProperty(None)
    column_headings = ObjectProperty(None)
    rv_data = ListProperty([])

    def __init__(self, **kwargs):
        super(QuestionDb, self).__init__(**kwargs)
        self.get_dataframe()

    def get_dataframe(self):
        df = pandas.read_csv("items.csv")
        print(df.columns)
        lists = [[row[col] for col in df.columns] for row in df.to_dict('records')]
        print(lists)

        # Extract and create column headings
        for heading in df.columns:
            self.column_headings.add_widget(Label(text=heading))

        # Extract and create rows
        global data
        for row in range(len(lists)):
            line = lists[row]
            for i in range(len(line)):
                #print(line[i])
                data.append([line[i], ''])
            self.rv_data = [{'text': str(x[0]), 'Index': str(x[1]), 'selectable': True} for x in data]

    def add_row(self, touch):
        df = pandas.read_csv("items.csv")
        lists = [[row[col] for col in df.columns] for row in df.to_dict('records')]
        global data
        line = lists[len(lists)-1]
        for i in range(len(line)):
            data.append([line[i], ''])
        self.rv_data = [{'text': str(x[0]), 'Index': str(x[1]), 'selectable': True} for x in data]


class QuestionApp(App):
    def build(self):
        Window.clearcolor = (1, 1, 1, 1)    # white background
        return QuestionDb()



if __name__ == "__main__":
    QuestionApp().run()
