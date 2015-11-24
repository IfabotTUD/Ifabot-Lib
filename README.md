# Ifabot Lib

The Ifabot Lib contains the device driver for the Robot-Board and IMU-Board. It depends on the Boost Library.

## Install Dependencies

```Shell
sudo apt-get install libboost-all-dev
```

## Build library

The following commands will build a shared library and install it to /usr/local/

```Shell
make
sudo make install
```

## API Documentation

* [RobotBoard](doc/robotboard.md)
* [IMUBoard](doc/imuboard.md)