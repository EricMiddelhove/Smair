//
//  ContentView.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 29.12.20.
//

import SwiftUI

let serverAdress = "http://192.168.2.115:4000"

var roomDemo = Room(id: "0", name: "EricsRoom")
var windowDemoData = [
    Window(id: "0", name: "EricsWindow", room: roomDemo),
    Window(id: "1", name: "EricsWindow2", room: roomDemo),
    Window(id: "2", name: "EricsWindow3", room: roomDemo)]

struct ContentView: View {
    
    var body: some View {
        TabView(){
                
                RoomView()
                    .tabItem {
                        Image(uiImage: UIImage(systemName: "camera.viewfinder")!)
                        Text("Rooms")
                    }

                WindowView()
                    .tabItem {
                        Image(uiImage: UIImage(systemName: "person.fill")!)
                        Text("Windows")
                    }
            }
    }
}




struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
