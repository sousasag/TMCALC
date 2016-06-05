import tmcalc_module as tm

(teff,erteff,erteff2,erteff3,nout,nindout) = tm.get_temperature_py('ratios_list.dat', 'HD20619.ares')
(fehout, erfehout, nout) = tm.get_feh_py('feh_calib_lines.dat', 'HD20619.ares', teff, erteff, erteff2, erteff3)
print 'Teff:', teff
print 'Feh:', fehout
