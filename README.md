# iomote-app-starterkit

## The hardware
The starter kit comes with the X400 kit bundle, and contains the following hardware:
* Analog slider (connected with **analog A1**)
* Temperature/humidity sensor **HTS221** (connected with **I2C bus**) 
* Alphanumerical display (4x20) (connected with **I2C bus**)
* Push button (connected with **digital pin 4**)
* LED (connected with **digital pin 7**)

## The app
This app is intendend for evaulating the starter kit hardware and basic communication with MyMote Cloud.

### Required libraries
The app requires the following libraries:

* SmartEverything HTS221 by Seve (tested Version: 1.1.2) 
* LiquidCrystal I2C by Frank de Brabander (tested Version: 1.1.2)

You can install the libraries directly from the Arduino IDE (sketch->Include library->Manage libraries).

### Usage
* Copy the .ino file inside your project
* Verify/Compile
* Upload the file to the board
To get debug informations, open the serial monitor of the IDE.

### How it works
Once started, the app will show on the LCD display the following data:
* Temperature + humidity from the HTS221
* Analog value shown as slide
* Daily messages left (X400 device has a quota of 100 messages/day to the cloud)
* Number of pending messages (messages stored in the flash memory of the device waiting to be sent to the cloud).

Pushing the button the device will send a message to the cloud containing temperature and humidity data.
