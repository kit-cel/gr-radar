GNU Radio Radar Toolbox
========

**Install guide**  
Change to any folder in your home directory and enter following commands in your terminal.

`git clone https://github.com/kit-cel/gr-radar.git` // clone this repository  
`cd gr-radar/`  
`mkdir build` // make build folder  
`cd build/`  
`cmake ../` // build makefiles  
`make` // build toolbox  
`ctest` // run tests  
`sudo make install` // install toolbox

If ctest responds with '100% tests passed', the toolbox should be build correctly.

**Development progress blog**  
https://grradar.wordpress.com/

**Development platform**  
GNU Radio 3.7.4  
Ubuntu 14.04

Qt 4.8.6
Qwt 6.0.0

**Contact**  
Stefan Wunsch  
stefan.wunsch[at]student.kit.edu
