from flask import Flask, request
from enum import Enum
from uuid import uuid4

global windows
windows = []

class Status(Enum):
    CLOSED = 0
    OPEN = 1
    ANGLED = 2
    ERROR = -1

class Window:

    def __init__(self, name, location):
        self.id = str(uuid4())
        self.name = name
        self.location = location
        self.status = Status.CLOSED


app = Flask(__name__)


@app.route('/')
def hello_world():
    return 'hello world'

@app.route('/windows/', methods = ["POST"])
def addWindow():
    """ registers window in memory and returns the window id for the client to save """
    
    global windows
    if request.method == "POST":
        jsonDict = request.json
        window = Window(jsonDict["name"], jsonDict["location"])
        windows += [window]

        return '{"id" :' + f' "{window.id}"' + '}'
    else:
       return "Bad Request"

@app.route('/windows/<id>')
def getWindow(id):
    global windows

    for w in windows:
        if w.id == id:
            out = '{'
            out += f'"id" : "{w.id}",'
            out += f'"name" : "{w.name}",'
            out += f'"location" : "{w.location}",'
            out += f'"status" : "{str(w.status)}"'
            out += '}'

            return out

@app.route('/updateStatus/', methods = ["POST"])
def updateStaus():
    global windows

    jsonDict = request.json
    windowID = jsonDict["id"]
    newStatus = jsonDict["status"]

    for w in windows:
        if w.id == windowID:
            switcher = {
                0: Status.CLOSED,
                1: Status.ANGLED,
                2: Status.OPEN

            }
            w.status = switcher.get(newStatus, Status.ERROR)

    return "200 OK"




if __name__ == '__main__':
    app.run()
