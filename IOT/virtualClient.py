import requests
import json
from tkinter import *


BASE_URL = "https://iot-project-28689-default-rtdb.europe-west1.firebasedatabase.app/sensors/"
sensor_no = 1
sensor_description = "Virtual Python Sensor"
finalUrl = BASE_URL + "sensor" + str(sensor_no) + ".json"

def sendRequest():
    tempature = tempatureScale.get()
    moisture = moistureScale.get()
    payload = {
    "description" : sensor_description,
    "tempature" : tempature,
    "moisture" : moisture
    }
    json_object = json.dumps(payload, indent = 4) 
    response = requests.put(finalUrl, data = json_object)
    print(response)

window = Tk()
window.geometry("300x150")

tempatureScale = Scale(window, from_=100, to=0)
tempatureScale.place(x=90, y=10)

moistureScale = Scale(window, from_=100, to=0)
moistureScale.place(x=10, y=10)

tempatureLabel = Label(text = "Tempature")
tempatureLabel.place(x = 90, y = 120)

moistureLabel = Label(text = "Moisture")
moistureLabel.place(x = 10, y = 120)

broadcastButton = Button(window, text = "Sent", command = sendRequest)
broadcastButton.config(width = 10, height = 10)
broadcastButton.place(x = 170, y = 0)

mainloop()