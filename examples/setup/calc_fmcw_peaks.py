c_light = 3e8

center_freq = 5.7e9 # center frequency [Hz]
samp_rate = 10000000 # sample frequency [Hz]
sweep_freq = samp_rate # sweep frequency FMCW chirp

samp_cw = 2**14 # samples CW block
samp_up = 2**14 # samples up-chirp
samp_down = samp_up # samples down-chirp

r = 100 # range [m]
v = 0 # velocity [m/s]

freq_cw = 2*v*center_freq/c_light
freq_up = 2*r*sweep_freq*samp_rate/samp_up/c_light - freq_cw
freq_down = -2*r*sweep_freq*samp_rate/samp_down/c_light - freq_cw

res_v = samp_rate/samp_cw*c_light/2/center_freq
res_r = c_light/2/sweep_freq
res_t = (float)(samp_cw+samp_up+samp_down)/samp_rate

res_f_cw = samp_rate/samp_cw
res_f_up = samp_rate/samp_up
res_f_down = samp_rate/samp_down

trans_rate = samp_rate*8.0/1024.0/1024.0
dec_fac = 12
trans_rate_res = trans_rate/dec_fac
samp_rate_res = samp_rate/dec_fac

print("// FMCW peaks")
print("CW:", freq_cw, "Hz")
print("up-chirp:", freq_up, "Hz")
print("down-chirp:", freq_down, "Hz")
print()
print("// FMCW resolutions")
print("res freq fft (cw, up, down):", res_f_cw, res_f_up, res_f_down, "Hz")
print("res v:", res_v, "m/s")
print("res r:", res_r, "m")
print("res t:", res_t, "s")
print()
print("// Transfer rate")
print("bandwith: ", samp_rate/1000.0, "kHz")
print("rate:", trans_rate, "MByte/s")
print("resampler decimation:", dec_fac)
print("bandwith after resampling:", samp_rate_res/1000, "kHz")
print("rate after resampling:", trans_rate_res, "MByte/s")
