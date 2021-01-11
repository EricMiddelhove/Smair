//
//  RoomView.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 01.01.21.
//

import Foundation
import SwiftUI

let noRoom = Room(id: "-1", name: "noRoom")

struct RoomView: View{
    
    @ObservedObject var rooms: RoomArray = RoomArray(array: [])
    @State var roomMatrix: [[Room?]] = [[nil,nil]]
    
    
    var body: some View{
        VStack{
            VStack(alignment: .center, spacing: 60, content: {
                if roomMatrix.count > 1 {
                    ForEach(roomMatrix.indices){ i in
                        RoomLine(content1: RoomButton(room: (roomMatrix[i][0])), content2: RoomButton(room: (roomMatrix[i][1])))
                    }
                }else if roomMatrix.count == 1{
                    RoomLine(content1: RoomButton(room: (roomMatrix[0][0])), content2: RoomButton(room: (roomMatrix[0][1])))
                }
            })
            Spacer()
        }.onAppear(perform: initialise)
    }

    func initialise(){
        //Getting all rooms
        let str = serverAdress + "/rooms/"
        let url = URL(string: str)!
        let req = URLRequest(url: url)
                
        URLSession.shared.dataTask(with: req){ (d,r,e) in
            
            guard let data = d else {
                return
            }
            
            guard let _ = r else {
                return
            }
            
            if let error = e {
                print(error)
                return
            }
            
            do {
                let arrayDicts = try JSONSerialization.jsonObject(with: data, options: []) as! [[String: AnyObject]]
                
                var arrayOfRooms: [Room] = []
                
                for d in arrayDicts {
                    arrayOfRooms += [Room(from: d)]
                }
                
                DispatchQueue.main.sync{
//                    print(arrayOfRooms)
//                    var numberOfRows = Int(arrayOfRooms.count / 2) % 2 == 0 ? Int(arrayOfRooms.count / 2) : Int(arrayOfRooms.count / 2) + 1
                    if arrayOfRooms.count == 0 {
                        self.roomMatrix = [[nil,nil]]
                        rooms.array = []
                    
                    }else if arrayOfRooms.count == 1 {
                        self.roomMatrix = [[arrayOfRooms[0],nil]]
                        rooms.array = [arrayOfRooms[0]]
                    }else{
                        var tmpMatrix: [[Room?]] = []
                        
                        var i = 0;
                        while i < arrayOfRooms.count {
                            
                            var row: [Room?] = []
                            for j in i...i+1 {
                                if j < arrayOfRooms.count {
                                    row.append(arrayOfRooms[j])
                                }else{
                                    row.append(nil)
                                }
                            }
                            print(row)
//                            let row: [Room] = [arrayOfRooms[i], arrayOfRooms[i+1]]
                            
                            tmpMatrix.append(row)
                            
                            i += 2
                        }
                        
                        self.roomMatrix = tmpMatrix
                        rooms.array = arrayOfRooms
                    }
                }
                
            }catch{
                return
            }
            
            
        }.resume()
        
        
        
    }
    
}

struct RoomLine: View{
    
    var content1: RoomButton?
    var content2: RoomButton?
    
    var body: some View{
        HStack{
            Spacer()
            if let c1 = content1{
                c1
            }
            Spacer()
            if let c2 = content2{
                c2
            }
            Spacer()
        }
    }
}

struct RoomButton: View{
    
    var room: Room?
    
    var body: some View{
        Text(room?.name ?? "")
    }
}

struct RoomView_Previews: PreviewProvider {
    static var previews: some View {
        RoomView()
    }
}
