from datetime import date, datetime
from enum import Enum
from uuid import uuid4

class Status(Enum):
    CLOSED = 0
    OPEN = 1
    ANGLED = 2
    ERROR = -1

class Window():
    def __init__(self, ID, room, name):
        self.ID = str(ID)
        self.room = room
        self.name = str(name)
        self.timestamp = datetime.now()
        self.status = Status.CLOSED

    def updateTimestamp(self):
        self.timestamp = datetime.now

    def checkTimestamp(self):
        """ return true if timestamp is valid -> not older than 10 seconds ago """
        currentTime = datetime.now()
        difference = currentTime - self.timestamp

        return (difference.total_seconds() < 10)

    def getJson(self):
        out = '{'
        out += '"ID" : "' + self.ID + '",'
        out += '"room" : ' + self.room.getJson() + ','
        out += '"name" : "' + self.name + '",'
        out += '"status" : "' + str(self.status.value) + '"'
        out += '}'



        return out


class Room():
    def __init__(self, name, ID = str(uuid4())):
        self.ID = str(ID)
        self.name = name
    
    def getJson(self):
        out = '{'
        out += '"ID" : "' + self.ID + '",'
        out += '"name" : "' + self.name +'"'
        out += '}'

        return out

