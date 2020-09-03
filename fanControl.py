#Fan Control UI
#Cal.W 2020
import tkinter as tk

fanSpeed = 0
lowerESClimit = 0
upperESClimit = 180

#Only allow Integers
def limitToDigit(P):
    return (str.isdigit(P) or P == "")


def update_values():
    upperESClimitInput.set(upperESClimit)
    lowerESClimitInput.set(lowerESClimit)

def send_values():
    print("Sending Values: V{}, U{}, L{}".format(fanSpeed, lowerESClimit, upperESClimit))

def get_values():
    update_values()

root = tk.Tk()
root.title("MARS Fan Speed Control Software V1.0")
tk.Label(root, text="MARS Fan Speed Control Software V1.0").grid(row=0, columnspan = 3)
tk.Label(root, text="Fan Speed").grid(row=1)
tk.Label(root, text="Fan Speed").grid(row=2)
tk.Label(root, text="Upper Limit").grid(row=3)
tk.Label(root, text="Lower Limit").grid(row=4)

fanSpeedSlider = tk.Scale(root, from_=lowerESClimit, to=upperESClimit, tickinterval=upperESClimit, orient=tk.HORIZONTAL).grid(row=1, column=1)
fanSpeedNInput = tk.Entry(root, validate='all', validatecommand=(root.register(limitToDigit), '%P')).grid(row=2, column=1)
upperESClimitInput = tk.Entry(root, validate='all', validatecommand=(root.register(limitToDigit), '%P')).grid(row=3, column=1)
lowerESClimitInput = tk.Entry(root, validate='all', validatecommand=(root.register(limitToDigit), '%P')).grid(row=4, column=1)

tk.Button(root, text='Send Fan Speed to Arduino', command=send_values).grid(row=5, column=0, sticky=tk.W, pady=4)
tk.Button(root, text='Get Fan Speed from Arduino', command=get_values).grid(row=5, column=1, sticky=tk.W, pady=4)
tk.Button(root, text='Quit', command=root.quit).grid(row=5, column=2, sticky=tk.W, pady=4)

#Grab Values
get_values()

tk.mainloop()