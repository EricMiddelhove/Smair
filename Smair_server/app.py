from flask import Flask, request
from enum import Enum
from uuid import uuid4
import pymongo

from models import *


myclient = pymongo.MongoClient("mongodb://127.0.0.1:27017/")
mydb = myclient["mydatabase"]
arduino_clients = mydb["arduino_clients"]
rooms = mydb["rooms"]

global windows
windows = []

global noRoom 
noRoom = Room("noRoom", "0")

app = Flask(__name__)



@app.route('/')
def hello_world():
    return 'hello world'

@app.route('/heyThere/')
def heyThere():
    """Initialising new Window. Gets called when an Arduino initialises"""
    global windows
    global noRoom

    newWindow = Window(
        ID = str(uuid4()),
        room = noRoom,
        name = "newWindow"
    )

    newWindow.updateTimestamp()
    #windows += [newWindow]
    arduino_clients.insert_one(newWindow.getDict())

    print("initialised window with ID " + newWindow.ID)
    return '{ "ID" : "' + newWindow.ID + '"}'

@app.route('/updateStatus/', methods=["POST"])
def updateStatus():
    global windows

    jsonDict = request.json

    windowID = jsonDict["ID"]

    switcher = {
        0: Status.CLOSED,
        1: Status.OPEN,
        2: Status.ANGLED,
        -1: Status.ERROR
    }

    status = switcher.get(jsonDict["status"], Status.ERROR)

    for w in windows:
        if w.ID == windowID:
            w.status = status
            w.updateTimestamp()


    print("updated status of window with ID " + windowID)
    return "200 OK"

@app.route('/setRoom/', methods = ["POST"])
def setRoom():
    global rooms
    global windows

    jsonDict = request.json
    windowID = jsonDict["ID"]
    roomName = jsonDict["roomName"]
    
    print('set room of window "' + windowID + '" to "' + roomName + '"')

    i = 0
    for w in windows:
        if w.ID == windowID:
            break
        i = i + 1

    for r in rooms:
        if r.name == roomName:
            windows[i].room = r
            return "200 OK"

    newRoom = Room(roomName) 
    rooms += [newRoom]
    windows[i].room = newRoom


    return "200 OK"

@app.route('/setName/', methods = ["POST"])
def setName():
    global windows
    jsonDict = request.json
    
    windowID = jsonDict["ID"]
    name = jsonDict["name"]

    print('set name of window "' + windowID + '" to "' + name + '"')

    for w in windows: 
        if w.ID == windowID:
            w.name = name

    return "200 OK"

@app.route('/windows/')
def getWindows():
    windows = []

    for w in arduino_clients.find():
        windows += [w]

    if len(windows) == 0:
        return'[]'

    out = '['
    for w in windows:
        out += str(w)
        out += ','

    out = out[:-1]

    out += ']'

    print("returned window data of all windows")
    return out


@app.route('/rooms/')
def getRooms():
    global rooms

    if len(rooms) == 0: 
        return "[]"

    out = '['
    for r in rooms:
        out += r.getJson()
        out += ','
    
    out = out[:-1]
    out += ']'

    print("returned room data of all rooms")
    return out





if __name__ == '__main__':
    app.run()
