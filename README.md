# ESP2688-SocketIO
Connect the ESP2688 to Socket IO and receive JSON Strings.
## Requirements
- ArduinoJson by Benoit Blanchon
- SocketIoClient by Vincent Wyszynski
## Explanation
Connect your ESP2688 to your SocketIo-Server an receive JSON-data to toggle the intern LED of the ESP.
In this case the ESP listens to the 'switch_change' event. If the event is emitted by the SocketIo Server the ESP will parse the JSON payload which looks like this:
'''json
{"name":"switch_1","val":false}
'''
After that it toggles the built-in LED in case of the boolean 'val'.
