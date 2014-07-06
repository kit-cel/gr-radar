GNU Radio Radar Toolbox
========

**Install guide**  
Change to any folder in your home directory and enter following commands in your terminal.

`git clone https://github.com/kit-cel/gr-radar.git` // clone this repository  
`cd gr-radar/`  
`sudo ./examples/setup/setup_core` // increase maximum of shared memory segment and USRP stuff, needed for large buffers (change is resetted after reboot)  
`mkdir build` // make build folder  
`cd build/`  
`cmake ../` // build makefiles  
`make` // build toolbox  
`ctest` // run tests  
`sudo make install` // install toolbox

If ctest responds with '100% tests passed', the toolbox should be build correctly.

**Development progress blog**  
https://grradar.wordpress.com/

**Documentation**  
For full documentation open docs/html/index.html in any web browser.

**Development platform**  
GNU Radio 3.7.4  
Ubuntu 14.04

**Dependencies**  
Qt 4.8.6  
Qwt 6.0.0  
python-matplotlib  

Qt and Qwt should be installed with GNU Radio. But there can be issues if Qwt is below version 6. python-matplotlib is included in some testcases.

**Contact**  
Stefan Wunsch  
stefan.wunsch[at]student.kit.edu
