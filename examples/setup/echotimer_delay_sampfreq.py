from matplotlib import pyplot as plt

samp_freq = (250e3, 500e3, 1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6, 10e6, 11e6, 12e6, 13e6, 14e6, 15e6, 16e6, 17e6, 18e6, 19e6, 20e6, 21e6, 22e6, 23e6, 24e6, 25e6)
delay_samp = (24, 24, 24, 21, 19, 20, 30, 22, 28, 24, 26, 31, 29, 40, 40, 32, 32, 39, 39, 39, 39, 39, 39, 39, 57, 57, 57)

plt.plot(samp_freq,delay_samp,'-o')
plt.title('Delay samples/Sample frequency')
plt.xlabel('Sample frequency [Hz]')
plt.ylabel('Delay samples')
plt.show()
