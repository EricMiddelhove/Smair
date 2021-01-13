In order to prevent my home from getting robbed, I want to create a app which displays my windows and if they are opened, closed or angled. 

Smair_server:
  This is the server application, in my case running on a raspberry pi 3B+ on Ubuntu-Server 20.10.
  The server is a flask application which saves all windows, and window states to a mongoDB database. The data transfer uses the JSON format.
  
Smair_arduino_client:
  As the name says, is this the program running on the Arduino. On the first power up the Arduino sends a /heyThere/ request to the Server, which initialises a window object on the server.
  After that the Arduino sends every 2 seconds an update of its opening status to the server via the /updateStatus/ request. If the arduino misses out 5 updates, the server will mark the window with an Error
  That could be cause by low power of the Arduino for example.
  
  Hardware: 
  I use a arduino nano with an ESP8226-01 Wifi chip and a 6Y-521 Gyroscope
  
Smair_mobile_client:
  This is the iOS Application which serves as the user interface. It requests the information about all windows via the /windows/ request and displays it in a list. 
