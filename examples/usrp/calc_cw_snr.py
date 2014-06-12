import numpy as np
from matplotlib import pyplot as plt

c_light = 3e8

center_freq = 2.4e9 # center frequency [Hz]

snr = np.linspace(-120,0,100)
R_max = [0]*len(snr)
for k in range(len(snr)):
	G = pow(10,10/10.0) # antenna gain [1]
	wave_len = c_light/center_freq # wave length [m]
	rcs = 4*np.pi*1/wave_len/wave_len # rcs
	P_t = pow(10,10/10.0) # transmit power [mW]
	P_r =  pow(10,snr[k]/10.0) # minimal receive power [mW]
	L = 1 # external loss

	R_max[k] = pow(P_t*G*G*wave_len*wave_len*rcs/pow(4*np.pi,3)/L/P_r,1.0/4)

print "RCS:", rcs

plt.plot(snr,R_max)
plt.xlabel('range [m]')
plt.ylabel('SNR [dB]')
plt.show()
