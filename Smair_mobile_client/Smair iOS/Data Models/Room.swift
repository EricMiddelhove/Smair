//
//  Room.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 01.01.21.
//

import Foundation

class Room: Hashable, ObservableObject{
    static func == (lhs: Room, rhs: Room) -> Bool {
        return lhs.id == rhs.id
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(id)
        hasher.combine(name)
    }
    
    
    var id: String
    @Published var name: String
    
    init(id: String, name: String){
        self.id = id
        self.name = name
    }
    
    init(from dict: [String: AnyObject]){
        self.id = dict["_id"] as! String
        self.name = dict["name"] as! String
    }
}

class RoomArray: ObservableObject{
    @Published var array: [Room]
    
    init(array: [Room]){
        self.array = array
    }
}
