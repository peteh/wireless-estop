# remote-estop
This project realizes a wireless E-Stop. It will handle the state of the actual button and also handle connection losses. 

The reaction time is configurable. 

## ESP-NOW! Connection
The client is a Wemos D1 Mini with an attached battery shield and a 1000mAh battery connected to it. Additionally a "Normally Closed" E-Stop button button is wired to an input pin. 

The master is also a Wemos D1 that is connected to a relay switch. The state of the E-Stop button is mirrored to the relay switch. 

The state of the button is sent to the master in a given frequency via ESP-NOW! protocol. 

There are two causes that can set the system to stopped state. 
 * The button is pressed (connection is open)
 * The master does not receive a message within a given time frame (timeout)


If the master does not receive a message within a given time it will assume the connection is lost and switch to STOP state. 
