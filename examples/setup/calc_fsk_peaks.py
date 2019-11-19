c_light = 3e8

R = 100
v = 10

samp_rate = 2**17
bandwith = samp_rate/4
center_freq = 5.7e9
samp_per_freq = 1
blocks_per_tag = 2**12

R_max = c_light/8/bandwith
f_doppler = 2*v*center_freq/c_light

print("// FSK peaks")
print("doppler:", f_doppler)
print("max R:", R_max)
print()

print("// FSK resolutions")
print("freq res:", samp_rate/2/blocks_per_tag)
