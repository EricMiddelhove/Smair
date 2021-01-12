from flask import Flask, request
from enum import Enum
from uuid import uuid4
import pymongo

from models import *


myclient = pymongo.MongoClient("mongodb://127.0.0.1:27017/")
mydb = myclient["mydatabase"]
arduino_clients = mydb["arduino_clients"]
rooms = mydb["rooms"]


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
    arduino_clients.insert_one(newWindow.getDict())

    print("initialised window with ID " + newWindow.ID)
    return '{ "ID" : "' + newWindow.ID + '"}'

@app.route('/updateStatus/', methods=["POST"])
def updateStatus():
    global windows

    jsonDict = request.json

    windowID = jsonDict["ID"]

    arduino_clients.update_one({"_id": windowID},{"$set": {"status": jsonDict["status"]}})

    print("updated status of window with ID " + windowID)
    return "200 OK"

@app.route('/setRoom/', methods = ["POST"])
def setRoom():

    jsonDict = request.json
    windowID = jsonDict["ID"]
    roomName = jsonDict["roomName"]
    
    print('set room of window "' + windowID + '" to "' + roomName + '"')
    
    foundSomething = False
    myRoom = {}
    for r in rooms.find({"name": roomName}): #finding stuff room with a matching name
        print("found a room")
        foundSomething = True
        myRoom = r

    if foundSomething == False: #if no room was found, creating an new room
        print("found no room, creating an new one")
        myRoom = Room(roomName).getDict()
        rooms.insert_one(myRoom)

    arduino_clients.update_one({"_id": windowID}, {"$set": {"room": myRoom}})

    return "200 OK"

@app.route('/setName/', methods = ["POST"])
def setName():
    jsonDict = request.json
    
    windowID = jsonDict["ID"]
    name = jsonDict["name"]

    print('set name of window "' + windowID + '" to "' + name + '"')

    arduino_clients.update_one({"_id": windowID},{"$set": {"name": name}})

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

    allRooms = []
    for r in rooms.find():
        allRooms += [r] 

    if len(allRooms) == 0: 
        return "[]"

    out = '['
    for r in allRooms:
        out += str(r)
        out += ','
    
    out = out[:-1]
    out += ']'

    print("returned room data of all rooms")
    return out

if __name__ == '__main__':
    app.run()
