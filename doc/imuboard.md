### UML diagram

![IMU-Board UML diagram](imuBoardDocu.png "IMU-Board UML diagram")

### Example usage

```cpp
// IMUBoardDemo.cpp
#include <IMUBoard.h>

using namespace ifabot;

void dataReceivedCallback(IMUBoardStatus status){
  // Use received data here
}

void errorOccuredCallback(ErrorType type, std::string info){
  switch(type){
    case LOST_CONNECTION: 
        break;
    case LOST_PACKAGE: 
        // info tells which package it expected and which it received
        break;
    case UNDECODABLE_DATASTREAM: 
        // info contains the hexdump
        break;
  }
}

int main() {
  IMUBoard board;

  board.setOnDataReceivedCallback(dataReceivedCallback);
  board.setOnErrorCallback(errorOccuredCallback);

  if(board.connectAndStartReceiving()){
    // Connected
    board.startGyroscopeCalibration(500);
    // ...
  }
}
```

Compile Example:

```Shell
g++ IMUBoardDemo.cpp -o IMUBoardDemo -lifabot -lboost_system -lboost_thread
```