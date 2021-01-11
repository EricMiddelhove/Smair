//
//  Window.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 29.12.20.
//

import Foundation

class Window: Hashable, ObservableObject{
    
    static func == (lhs: Window, rhs: Window) -> Bool {
        return lhs.id == rhs.id
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(id)
        hasher.combine(name)
        hasher.combine(location)
    }
    
    var id: String
    @Published var name: String
    @Published var location: Room
    var status: String
    
    init(id: String, name: String, room: Room){
        
        self.id = id
        self.name = name
        self.location = room
        self.status = "0"
    }
    
    init(from dict: [String: AnyObject]){
        self.id = dict["ID"] as! String
        self.name = dict["name"] as! String
        self.location = Room(from: dict["room"] as! [String: AnyObject])
        self.status = dict["status"] as! String
    }
    
    func getDescription() -> String{
        return self.name != "newWindw" ? self.id : self.name
    }
    
    func getStatusString() -> String{
        switch(self.status){
            case "0": return "CLOSED";
            case "1": return "ANGLED";
            case "2": return "OPEN";
            default: return "ERROR"; 
        }
    }
}

class WindowArray: ObservableObject{
    @Published var array: [Window]
    
    init(array: [Window]){
        self.array = array
    }
}
