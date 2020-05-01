from tkinter import *
import tkinter.ttk as ttk
import csv


def setting():
    line = 0
    tree.tag_configure('div', background='#4367b3', foreground='#eeeeee')
    tree.tag_configure('0', background='#1c1c1c')
    tree.tag_configure('1', background='#1f1f1f')

    tree.delete(*tree.get_children())
    with open('data.txt') as f:
        reader = csv.DictReader(f, delimiter=',')
        for row in reader:
            charging = row['Charging']
            voltage = row['Voltage']
            bms = row['BMS']
            shutdownnodes = row['Shutdown Nodes']
            tsal = row['TSAL']
            tree.insert("", 0, values=(charging, voltage, bms, shutdownnodes, tsal), tags = (str(line%2),))
            line += 1
    tree.insert('', 0, values=('OEM Terminal', '', '', '', 'ver 0.0.1'), tags = ('div',))


    root.after(500, setting)


def heading0():
    print('hello')
def heading1():
    print('hello1')
def heading2():
    print('hello2')
def heading3():
    print('hello3')
def heading4():
    print('hello4')
def heading5():
    print('hello5')
def heading6():
    print('hello6')


root = Tk()
root.title("LCD Display Terminal")
width = 565
height = 300
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
x = (screen_width/2) - (width/2)
y = (screen_height/2) - (height/2)
root.geometry("%dx%d+%d+%d" % (width, height, x, y))
root.resizable(0, 0)

style = ttk.Style()
style.configure("mystyle.Treeview", highlightthickness=0, bd=0, font=('Arial', 10), foreground='#f1f1f1', background='#1f1f1f') # Modify the font of the body
style.configure("mystyle.Treeview.Heading", font=('Arial', 10), foreground='#f8b01d', background='#111111', relief='solid') # Modify the font of the headings
style.layout("mystyle.Treeview", [('mystyle.Treeview.treearea', {'sticky': 'nswe'})]) # Remove the borders

TableMargin = Frame(root, width=0)
TableMargin.pack(side=RIGHT)
scrollbarx = Scrollbar(TableMargin, orient=HORIZONTAL)
scrollbary = Scrollbar(TableMargin, orient=VERTICAL)
tree = ttk.Treeview(TableMargin, style="mystyle.Treeview", columns=("Charging", "Voltage", "BMS", "Shutdown Nodes", "TSAL"), height=400, selectmode="extended", yscrollcommand=scrollbary.set, xscrollcommand=scrollbarx.set)
scrollbary.config(command=tree.yview)
scrollbary.pack(side=RIGHT, fill=Y)
scrollbarx.config(command=tree.xview)
scrollbarx.pack(side=BOTTOM, fill=X)
tree.heading('Charging', text="Charging", anchor=W, command=heading1)
tree.heading('Voltage', text="Voltage", anchor=W, command=heading2)
tree.heading('BMS', text="BMS", anchor=W, command=heading3)
tree.heading('Shutdown Nodes', text="Shutdown Nodes", anchor=W, command=heading4)
tree.heading('TSAL', text="TSAL", anchor=W, command=heading5)
tree.column('#0', stretch=NO, minwidth=0, width=0)
tree.column('#1', stretch=NO, minwidth=50, width=110)
tree.column('#2', stretch=NO, minwidth=50, width=110)
tree.column('#3', stretch=NO, minwidth=50, width=110)
tree.column('#4', stretch=NO, minwidth=50, width=110)
tree.column('#5', stretch=NO, minwidth=50, width=110)

tree.pack()
setting()

root.mainloop()
