//
//  WindowView.swift
//  Smair iOS
//
//  Created by Eric Middelhove on 30.12.20.
//

import Foundation
import SwiftUI


struct WindowView: View{
    
    @ObservedObject var windows: WindowArray = WindowArray(array: [])
    
    var body: some View {
        NavigationView{
            
            List{
                ForEach(windows.array.indices, id: \.self) { i in
                    NavigationLink(destination: EditWindowView(currentWindow: self.windows.array[i])){
                        CellView(window: self.$windows.array[i])
                    }.navigationBarTitle("Windows")
                }
            }
            .onAppear(perform: initialise)
        }
    }
    
    func initialise(){
        let str = serverAdress + "/windows/"
        let url = URL(string: str)!
        let request = URLRequest(url: url)
        
        
        print("Initialise")
        
        URLSession.shared.dataTask(with: request){ (dat, res, err) in
            guard let data = dat else {
                print("no data")
                return
            }
            
            guard let _ = res else {
                print("No response")
                return
            }
            
            if let error = err {
                print(error)
                return
            }
            
            do {
                let arrayDicts = try JSONSerialization.jsonObject(with: data, options: []) as! [[String: AnyObject]]
                
                var arrayOfWindows: [Window] = []
                
                for dict in arrayDicts{
                    arrayOfWindows += [Window(from: dict)]
                }
                
                DispatchQueue.main.sync{
                    windows.array = arrayOfWindows
                    print("Download finished")
                }
            }catch{
                print("Error while decoding")
            }
            
        }.resume()
        
    }
    
}

struct CellView: View{
    
    @Binding var window: Window
    @State var statusColor: Color = .black
    
    
    var body: some View{
        HStack{
            Text(window.name).font(.title3)
            Text(window.location.name).fontWeight(.light).font(.footnote)
            Spacer()
            Text(window.getStatusString()).fontWeight(.medium).foregroundColor(statusColor)
        }.onAppear(perform: initialise)
    }
    
    func initialise() {
        switch window.status {
            case 0: statusColor = Color.green
            case 1: statusColor = Color.orange; break;
            case 2: statusColor = Color.red; break;
            
            default: statusColor = Color.black; break;
        }
    }
}


struct WindowView_Previews: PreviewProvider {
    static var previews: some View {
        NavigationView{
                EditWindowView(currentWindow: Window(id: "id-asdjkhfaf-hasipdfh-hfaisodaf", name: "newWindow", room: Room(id: "sdoiufh-asdfasdf-asdf", name: "noRoom")))
        }
    }
}




