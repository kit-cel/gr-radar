/*! \mainpage

\section install Install guide

For the actual install guide read the <code>README</code> at the main folder. Please take care of the dependencies. In particular there can be some issues if Qwt is below version 6. Furthermore python-matplotlib is used in some testcases. UHD must be installed because of the USRP interface. This should be done with the GNU Radio installation.

\subsection buffer Buffer size issues

Radar applications often need huge buffer sizes. Therefore you have to do two things to push the maximum buffer size to the maximum. First you have to set the maximum output buffer of all blocks in the flowgraph on two times of the value that you need. With this you can use buffer of about 2^17 float complex items. This number depends on your system. In GNU Radio Companion you can set this value in the properties of the blocks or you use the call <code><+block+>.set_min_output_buffer(<+value+>)</code> in a python flowgraph.

If you need greater buffer sizes you have to increase the limit of a single shared memory segment. The commands are given in a bash script in gr-radar/examples/setup/setup_core. If you want to set the parameters in gr-radar/examples/setup/setup_core on every system start, edit /etc/sysctl.conf with following lines.

<code>\# Updates for GNU Radio\n
net.core.rmem_max=50000000\n
net.core.wmem_max=1048576\n
kernel.shmmax=2147483648\n</code>

\subsection rebuild_doc Rebuild documentation

If there is any need to rebuild the documentation with doxygen go to gr-radar/docs/doxygen and run <code>doxygen Doxyfile.in</code>. The documentation is not rebuild with every build of the toolbox (unlike the default out-of-tree module behaviour). The actual documentation shall be included in the repository.

\section basic Basic ideas of the toolbox

Requirement to read this documentation is a basic idea of GNU Radio and its components. If you are not familiar with some terms or definitions please check out the GNU Radio documentation and wiki.

The basic flowgraph for many radar applications is shown below. First a signal is generated and send and received with some hardware. TX and RX signal are compared with an estimator and the result is displayed on the screen.

The toolbox uses tagged streams for packaging data and to make sure that corresponding data is processed together in one work function call. In most cases streams are used up to the evaluation of the signal attributes which are used for calculating range, velocity or azimuth. This attributes are most often peaks of a FFT spectrum. After this point there is no use for tagged streams and it is practical to switch to the message system of GNU Radio. This data is packed as PMTs (polymorphic types). Read the subsection 'Message structure and identifiers' for more information.

The send and receive part of the flowgraph is implemented in two ways. First you can use the USRP Echotimer. This block takes a tagged stream and ensure that this package is send and received synchronously. Further information in the section 'USRP Echotimer'. If you want to test your flowgraphs without the need of hardware you can use a simulator for the propagation effects. A simulator for static targets with constant attributes like range and velocity is implemented. It is possible to emulate a moving target if you use sliders for variables in GNU Radio Companion. The static target simulators has implemented a callback that updates the targets attributes in runtime.

\image html basic_flowgraph.svg

\subsection msg Message structure and identifiers (symbols)

If the toolbox exchanges messages a standard structure for the data is used. The idea is to tag a dataset with an identifier. This identifier is a string packed as a PMT symbol. Then the data is packed together as a list of two items (the identifier and the data). Multiple datasets can be send in one message as a list of these identifier/data pairs. With this structure the order of the datasets is not important and it is easy to enlarge or manipulate a message without have to think about the processing in the following blocks. Used PMT functionality are lists to create this structure. For visualisation the data structure looks like this:

A single dataset as identifier data pair: <code>[<+symbol0+>, <+data0+>]\n</code>
A whole message with multiple datasets: <code>[ [<+symbol0+>, <+data0+>], [<+symbol1+>, <+data1+>], ... ]</code>

Used identifiers (symbols) should be predefined to keep the handling simple. Current used identifiers are following strings.

<b>Signal information:</b>\n
<code>rx_time</code> : Timestamp of the RX signal\n
<code>frequency</code> : Frequency information of peaks in a FFT spectrum\n
<code>power</code> : Amplitude (magnitude square) of peaks, e. g. in a FFT spectrum or within matrix processing\n
<code>phase</code> : Phase of the complex value of peaks in a FFT spectrum\n
<code>axis_x</code> : Number of bin on x axis for matrix processing\n
<code>axis_y</code> : Number of bin on y axis for matrix processing\n



<b>Target information:</b>\n
<code>range</code> : Target range\n
<code>velocity</code> : Target velocity

\section echotimer USRP Echotimer

The USRP Echotimer is a UHD interface for USRPs. Known well working devices are N210, B210 and USRP 2. Check out www.ettus.com for more information about the hardware.

Basically the Echotimer bursts a data packet defined as tagged stream. Synchronously a packet with the same length is received. The receive time is given with a stream tag on the first item of the tagged stream with the identifier <code>rx_time</code>. This is put into practice with a send and receive command on a shared point of time on all USRPs. Therefore the time has to be synchronized on all used devices. This can be done with a MIMO cable or with a GPS clock. The strings are <code>mimo</code> or <code>gpsdo</code> for the time source. Either you can choose the GPS clock on both devices or set one clock on <code>internal</code> as master and set the clock of the slave device over MIMO cable.

Look at the GNU Radio Companion flowgraph samples in <code>/examples/usrp</code> for reference. Mostly all values and strings are push through and used without a change within the UHD API. Therefore the UHD API documentation is also a good reference.

*/
