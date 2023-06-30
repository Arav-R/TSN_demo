import tkinter
import tkinter.messagebox
import customtkinter
from typing import Callable, Union

import matplotlib.pyplot as plt
import numpy as np


import customtkinter
import coap_client
import time
import asyncio

motor1Step = 0
motor2Step = 0

destroy = False
on = False

NUM = 100
TICK = 5
PAN = 25

motor1RoundTrip = []
motor2RoundTrip = []

step = 1


customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"


class FloatSpinbox(customtkinter.CTkFrame):
    def __init__(self, *args,
                 width: int = 100,
                 height: int = 32,
                 step_size: Union[int, float] = 1,
                 motor: int = 0,
                 command: Callable = None,
                 **kwargs):
        super().__init__(*args, width=width, height=height, **kwargs)

        self.step_size = step_size
        self.command = command
        self.value = 0.0
        self.motor = motor

        self.configure(fg_color=("gray78", "gray28"))  # set frame color

        self.grid_columnconfigure((0, 2), weight=0)  # buttons don't expand
        self.grid_columnconfigure(1, weight=1)  # entry expands

        self.subtract_button = customtkinter.CTkButton(self, text="-", width=height-6, height=height-6, command=self.subtract_button_callback)
        self.subtract_button.grid(row=0, column=0, padx=(3, 0), pady=3)

        # self.entry = customtkinter.CTkEntry(self, width=width-(2*height), height=height-6, border_width=0)
        # self.entry.grid(row=0, column=1, columnspan=1, padx=3, pady=3, sticky="ew")
        self.label = customtkinter.CTkLabel(self, width=width-(2*height), height=height-6, anchor="center", font=("Arial", 15))
        self.label.grid(row=0, column=1, columnspan=1, padx=3, pady=3, sticky="ew")

        self.add_button = customtkinter.CTkButton(self, text="+", width=height-6, height=height-6, command=self.add_button_callback)
        self.add_button.grid(row=0, column=2, padx=(0, 3), pady=3)

        # default value
        # self.entry.insert(0, "0.0")

    def add_button_callback(self):
        if self.command is not None:
            self.command()
        try:
            self.value += self.step_size
            print(self.value)

            if (self.motor == 1):
                asyncio.run(coap_client.single_put("coap://10.1.1.59/multistep/cmd", "1"))
            elif (self.motor == 2):
                asyncio.run(coap_client.single_put("coap://10.1.1.60/multistep/cmd", "1"))
            
        
        except ValueError:
            return

    def subtract_button_callback(self):
        if self.command is not None:
            self.command()
        try:
            self.value -= self.step_size
            print(self.value)
            
            if (self.motor == 1):
                asyncio.run(coap_client.single_put("coap://10.1.1.59/multistep/cmd", "-1"))
            elif (self.motor == 2):
                asyncio.run(coap_client.single_put("coap://10.1.1.60/multistep/cmd", "-1"))

        except ValueError:
            return

    # def get(self) -> Union[float, None]:
    #     try:
    #         return float(self.entry.get())
    #     except ValueError:
    #         return None

    # def set(self, value: float):
    #     self.entry.delete(0, "end")
    #     self.entry.insert(0, str(float(value)))
    def setLabel(self, name: str):
        self.label.configure(text=name)


class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()



        # configure window
        self.title("CustomTkinter complex_example.py")
        self.geometry(f"{1100}x{580}")

        # configure grid layout (4x4)
        self.grid_columnconfigure(1, weight=1)
        self.grid_columnconfigure((2), weight=0)
        self.grid_rowconfigure((0, 1, 2), weight=1)

        # create sidebar frame with widgets
        self.sidebar_frame = customtkinter.CTkFrame(self, width=140, corner_radius=0)
        self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
        self.sidebar_frame.grid_rowconfigure(4, weight=1)
        self.logo_label = customtkinter.CTkLabel(self.sidebar_frame, text="TSN Demo", font=customtkinter.CTkFont(size=24, weight="bold"))
        self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))

        switch_var = customtkinter.StringVar(value="off")
        self.switchON = customtkinter.CTkSwitch(self.sidebar_frame, command=self.on_switch_event, text="ON", switch_width=90, switch_height= 40, width=80, height= 75, font=("Arial", 20), onvalue="on", offvalue="off", variable=switch_var)
        self.switchON.grid(row=1, column=0, padx=(15,0), pady=10)
        self.bind("<space>", self.space)

        self.exit_button = customtkinter.CTkButton(self.sidebar_frame, command=self.exit_button_event, text="Exit")
        self.exit_button.grid(row=2, column=0, padx=20, pady=10)
        self.bind("<Escape>", self.exit_button_event)
        
        self.appearance_mode_label = customtkinter.CTkLabel(self.sidebar_frame, text="Appearance Mode:", anchor="w")
        self.appearance_mode_label.grid(row=5, column=0, padx=20, pady=(10, 0))
        self.appearance_mode_optionemenu = customtkinter.CTkOptionMenu(self.sidebar_frame, values=["Light", "Dark", "System"],
                                                                       command=self.change_appearance_mode_event)
        self.appearance_mode_optionemenu.grid(row=6, column=0, padx=20, pady=(10, 10))
        self.scaling_label = customtkinter.CTkLabel(self.sidebar_frame, text="UI Scaling:", anchor="w")
        self.scaling_label.grid(row=7, column=0, padx=20, pady=(10, 0))
        self.scaling_optionemenu = customtkinter.CTkOptionMenu(self.sidebar_frame, values=["80%", "90%", "100%", "110%", "120%", "150%"],
                                                               command=self.change_scaling_event)
        self.scaling_optionemenu.grid(row=8, column=0, padx=20, pady=(10, 20))

        # create main entry and button
        # self.entry = customtkinter.CTkEntry(self, placeholder_text="CTkEntry")
        # self.entry.grid(row=3, column=1, columnspan=2, padx=(20, 0), pady=(20, 20), sticky="nsew")

        # self.main_button_1 = customtkinter.CTkButton(master=self, fg_color="transparent", border_width=2, text_color=("gray10", "#DCE4EE"))
        # self.main_button_1.grid(row=3, column=3, padx=(20, 20), pady=(20, 20), sticky="nsew")

        # create textbox

        # create tabview
        

        # create radiobutton frame
        self.rightbar_frame = customtkinter.CTkFrame(self, width=800, corner_radius=0, border_width= 800)
        self.rightbar_frame.grid(row=0, column=2, rowspan=4, sticky="nsew")
        self.rightbar_frame.grid_rowconfigure(4, weight=1)
        self.telem_label = customtkinter.CTkLabel(self.rightbar_frame, text="Telemetry", font=customtkinter.CTkFont(size=24, weight="bold"))
        self.telem_label.grid(row=0, column=0, padx=20, pady=(20, 10))

        self.motor1_label = customtkinter.CTkLabel(self.rightbar_frame, text="Motor 1: " + str(motor1Step), font=customtkinter.CTkFont(size=20))
        self.motor1_label.grid(row=1, column=0, padx=20, pady=(10, 10))
        self.motor2_label = customtkinter.CTkLabel(self.rightbar_frame, text="Motor 2: " + str(motor2Step), font=customtkinter.CTkFont(size=20))
        self.motor2_label.grid(row=2, column=0, padx=20, pady=(10, 10))

        self.cycle_label = customtkinter.CTkLabel(self.rightbar_frame, text="Cycle speed: 0 Hz", font=customtkinter.CTkFont(size=20), width= 100, anchor="w", padx=25)
        self.cycle_label.grid(row=3, column=0, padx=5, pady=(10, 10))

        self.graph_button = customtkinter.CTkButton(self.rightbar_frame, text="Graph", font=customtkinter.CTkFont(size=20), command=self.graph_button_event)
        self.graph_button.grid(row=4, column=0, padx=20, pady=(0, 10))

        # create slider and progressbar frame
        self.control_frame = customtkinter.CTkFrame(self, fg_color="transparent")
        self.control_frame.grid(row=0, column=1, padx=(20, 0), pady=(20, 0), sticky="nsew")
        self.control_frame.grid_columnconfigure(0, weight=1)
        self.control_frame.grid_rowconfigure(4, weight=1)
        self.speed_label = customtkinter.CTkLabel(self.control_frame, text="Speed", font=customtkinter.CTkFont(size=20))
        self.speed_label.grid(row=0, column=0, padx=(20, 10), pady=(10, 10), sticky="ew")
        self.speed_slider = customtkinter.CTkSlider(self.control_frame, from_=0, to=2000, number_of_steps= 100, command=self.speed_slider_callback)
        self.speed_slider.grid(row=1, column=0, padx=(20, 10), pady=(10, 10), sticky="ew")

        self.accel_label = customtkinter.CTkLabel(self.control_frame, text="Acceleration", font=customtkinter.CTkFont(size=20))
        self.accel_label.grid(row=2, column=0, padx=(20, 10), pady=(10, 10), sticky="ew")
        self.accel_slider = customtkinter.CTkSlider(self.control_frame, from_=0, to=10000, number_of_steps= 100, command=self.accel_slider_callback)
        self.accel_slider.grid(row=3, column=0, padx=(20, 10), pady=(10, 10), sticky="ew")

        self.option_var = customtkinter.StringVar(value="Full Step")
        self.optionmenu_1 = customtkinter.CTkOptionMenu(self.control_frame, values=["Full Step", "Half Step", "Quarter Step", "Eighth Step"], command=self.microstep_callback, anchor="center", width=200, height=40, font=("Arial", 20))
        self.optionmenu_1.grid(row=4, column=0, padx=(20, 10), pady=(10, 10))
        self.optionmenu_1.set("MicroSteps")

        self.spinbox_frame = customtkinter.CTkFrame(self.control_frame, fg_color="transparent", )
        self.spinbox_frame.grid(row=5, column=0, padx=(15, 0), pady=(20, 0), sticky="nsew")
        self.spinbox_frame.grid_columnconfigure((0,1), weight=1)
        # self.spinbox_frame.grid_rowconfigure(4, weight=1)
        
        spinbox_1 = FloatSpinbox(self.spinbox_frame, width=150, step_size=1, motor=1)
        spinbox_1.grid(row=0, column=0, padx=(20, 10), pady=(10, 10))
        spinbox_1.setLabel("Motor 1")

        spinbox_2 = FloatSpinbox(self.spinbox_frame, width=150, step_size=1, motor=2)
        spinbox_2.grid(row=0, column=1, padx=(20, 10), pady=(10, 10))
        spinbox_2.setLabel("Motor 2")

        # spinbox_1.set(35)
        # print(spinbox_1.get())

        
        # spinbox_2.grid(row=6, column=0, padx=(20, 10), pady=(10, 10))

        self.zero_label = customtkinter.CTkButton(self.control_frame, text="Zero Motors", command=self.zero_button_event)
        self.zero_label.grid(row=7, column=0, padx=(20, 10), pady=(25, 10))



        # set default values
        

        self.appearance_mode_optionemenu.set("Dark")
        self.scaling_optionemenu.set("100%")
    
    def space(self, event = None):
        self.switchON.toggle()

    def microstep_callback(self, value):
        global step
        if value == "Full Step":
            asyncio.run(coap_client.single_put("coap://10.1.1.59/microstep/cmd", "1"))
            asyncio.run(coap_client.single_put("coap://10.1.1.60/microstep/cmd", "1"))
            step = 1

        elif value == "Half Step":
            asyncio.run(coap_client.single_put("coap://10.1.1.59/microstep/cmd", "2"))
            asyncio.run(coap_client.single_put("coap://10.1.1.60/microstep/cmd", "2"))
            step = 2

        elif value == "Quarter Step":
            asyncio.run(coap_client.single_put("coap://10.1.1.59/microstep/cmd", "4"))
            asyncio.run(coap_client.single_put("coap://10.1.1.60/microstep/cmd", "4"))
            step = 4
        elif value == "Eighth Step":
            asyncio.run(coap_client.single_put("coap://10.1.1.59/microstep/cmd", "8"))
            asyncio.run(coap_client.single_put("coap://10.1.1.60/microstep/cmd", "8"))
            step = 8
        print(value)
    
    def graph_button_event(self):
        print(motor1RoundTrip)
        print(motor2RoundTrip)
        

        ypoints1 = np.array(motor1RoundTrip)
        ypoints2 = np.array(motor2RoundTrip)

        

        plt.ion()
        plt.autoscale()
        plt.show()  

        plt.plot(ypoints1, color="r")
        plt.plot(ypoints2, color="b")
        plt.draw()
        plt.pause(0.001)

    def zero_button_event(self):
        asyncio.run(coap_client.single_put("coap://10.1.1.59/total/cmd", "0")) # = 0
        asyncio.run(coap_client.single_put("coap://10.1.1.60/total/cmd", "0")) # = 0
        print("zero")

    def speed_slider_callback(self, value):
        self.speed_label.configure(text="Speed: " + str(int(value)))

        asyncio.run(coap_client.single_put("coap://10.1.1.59/speed/cmd", str(int(value))))
        asyncio.run(coap_client.single_put("coap://10.1.1.60/speed/cmd", str(int(value))))
        print(value)
    def accel_slider_callback(self, value):
        self.accel_label.configure(text="Acceleration: " + str(int(value)))

        asyncio.run(coap_client.single_put("coap://10.1.1.59/accel/cmd", str(int(value))))
        asyncio.run(coap_client.single_put("coap://10.1.1.60/accel/cmd", str(int(value))))
        print(value)

    def open_input_dialog_event(self):
        dialog = customtkinter.CTkInputDialog(text="Type in a number:", title="CTkInputDialog")
        print("CTkInputDialog:", dialog.get_input())

    def change_appearance_mode_event(self, new_appearance_mode: str):
        customtkinter.set_appearance_mode(new_appearance_mode)

    def change_scaling_event(self, new_scaling: str):
        new_scaling_float = int(new_scaling.replace("%", "")) / 100
        customtkinter.set_widget_scaling(new_scaling_float)

    def on_switch_event(self):
        global on
        if on:
            on = False
        else:
            on = True
        print(on)
        # self.switchON.toggle()
    
    def exit_button_event(self, event=None):
        plt.close()
        app.destroy()

        global destroy
        destroy = True


if __name__ == "__main__":

    print()
    app = App()
    while True:
        app.update()

        if destroy:
            print("destroy")
            break

        # plotting
        if plt.get_fignums():
            # window(s) open
            ypoints1 = np.array(motor1RoundTrip)
            ypoints2 = np.array(motor2RoundTrip)

            start = max(len(motor1RoundTrip) - PAN , 0)
            end = start + PAN

            # print(start)

            

            plt.ion()
            plt.autoscale()
            plt.show()  

            plt.plot(ypoints1, color="r")
            plt.plot(ypoints2, color="b")

            plt.xlim([start, end])
            # set ticks to 1
            plt.xticks(np.arange(start, end, TICK))
            plt.draw()
            plt.pause(0.001)

        step1 = asyncio.run(coap_client.single_put("coap://10.1.1.59/total/cmd", "-1"))
        step2 = asyncio.run(coap_client.single_put("coap://10.1.1.60/total/cmd", "-1"))     
        app.motor1_label.configure(text="Motor 1: " + str(int.from_bytes(step1.payload, "big")))
        app.motor2_label.configure(text="Motor 2: " + str(int.from_bytes(step1.payload, "big")))

        if on:
            cycleStart = time.time()

            first = time.time()
            asyncio.run(coap_client.single_put("coap://10.1.1.59/multistep/cmd", str(25*int(step))))
            seconds = time.time()
            y = seconds - first
            motor1RoundTrip.append(y)

            

            step1 = asyncio.run(coap_client.single_put("coap://10.1.1.59/total/cmd", "-1"))
            step2 = asyncio.run(coap_client.single_put("coap://10.1.1.60/total/cmd", "-1"))


            ret = asyncio.run(coap_client.single_get("coap://10.1.1.59/safe/cmd")) # = 0
            net = asyncio.run(coap_client.single_get("coap://10.1.1.60/safe/cmd")) # = 0
            bet = int.from_bytes(ret.payload, "big") * int.from_bytes(net.payload, "big")
            while (bet != 1):
                app.update()
                
                ret = asyncio.run(coap_client.single_get("coap://10.1.1.59/safe/cmd")) # = 0
                net = asyncio.run(coap_client.single_get("coap://10.1.1.60/safe/cmd")) # = 0
                bet = int.from_bytes(ret.payload, "big") * int.from_bytes(net.payload, "big")
                
            

            first = time.time()
            asyncio.run(coap_client.single_put("coap://10.1.1.60/multistep/cmd", str(25*int(step))))
            seconds = time.time()
            y = seconds - first
            motor2RoundTrip.append(y)
            
            step1 = asyncio.run(coap_client.single_put("coap://10.1.1.59/total/cmd", "-1"))
            step2 = asyncio.run(coap_client.single_put("coap://10.1.1.60/total/cmd", "-1"))


            ret = asyncio.run(coap_client.single_get("coap://10.1.1.59/safe/cmd")) # = 0
            net = asyncio.run(coap_client.single_get("coap://10.1.1.60/safe/cmd")) # = 0
            bet = int.from_bytes(ret.payload, "big") * int.from_bytes(net.payload, "big")
            while (int.from_bytes(ret.payload, "big") != 1):
                app.update()

                ret = asyncio.run(coap_client.single_get("coap://10.1.1.59/safe/cmd")) # = 0
                net = asyncio.run(coap_client.single_get("coap://10.1.1.60/safe/cmd")) # = 0
                bet = int.from_bytes(ret.payload, "big") * int.from_bytes(net.payload, "big")
            
            cycleTime = time.time() - cycleStart
            num = round(1/cycleTime, 2)
            app.cycle_label.configure(text="Cycle Speed: " +   f'{num:.2f}'   + " Hz")
            