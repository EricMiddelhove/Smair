//
//  EditWindowView.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 01.01.21.
//

import Foundation
import SwiftUI

struct EditWindowView: View{
    
    @State var currentWindow: Window
    @State var properties: [String] = []
    @State var statusColor: Color = .black
    
    var body: some View{
        VStack{
            EditWindowLine(content: $currentWindow.name, font: .title, editable: true, propertyName: "name", window: currentWindow)
            EditWindowLine(content: $currentWindow.location.name, font: .title2,editable: true, propertyName: "location name", window: currentWindow)
            EditWindowLine(content: $currentWindow.id, font: .footnote, editable: false, window: currentWindow)
            
            Spacer()
            
            Text(currentWindow.getStatusString())
                .font(.system(size: 65))
                .foregroundColor(statusColor)
            Spacer()
        }
        .onAppear(perform: initialise)
        
        Spacer()
    }
    
    func initialise(){
        print("initialise the Edit Window View")
        
        switch(currentWindow.status){
        
            case "0": statusColor = .green; break;
            case "1": statusColor = .orange; break;
            case "2": statusColor = .red; break;
            default: statusColor = .black; break;
        
        }
        
    }
    
}

struct EditWindowLine: View{
    
    @Binding var content: String
    @State var font: Font
    @State var editable: Bool
    var propertyName: String?
    var window: Window
    
    var body: some View {
        HStack {
            Spacer()
            HStack {
                if editable {
                    Image(systemName: "square.and.pencil")
                        .foregroundColor(.blue)
                        .offset(x: (320), y: 0)
                    TextField(content, text: $content, onCommit: committedText)
                        .font(font)
                } else {
                    Text(content).font(font).foregroundColor(.gray)
                }
            }
            Spacer()
        }
    }
    
    func committedText(){
        print("Post it")
        guard let prop = propertyName else{
            print("No propertyname")
            return
        }
        
        //Setting up request
        
        var str = serverAdress
        var request: URLRequest!
        var body: String!
        
        
        if prop == "name"{
            str += "/setName/"
            body = "{\"ID\" : \"" + window.id + "\", \"name\" : \"" + content + "\"}"
        } else if prop == "location name" {
            str += "/setRoom/"
            body = "{\"ID\" : \"" + window.id + "\", \"roomName\" : \"" + content + "\"}"
        }
        
        let url = URL(string: str)!
        request = URLRequest(url: url)
        
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Accept")
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpBody = body.data(using: String.Encoding.utf8)

        
        URLSession.shared.dataTask(with: request){dat ,res ,err in
            if let d = dat {
                print(String(data: d, encoding: String.Encoding.utf8)!)
            }
        }.resume()
    }
}
