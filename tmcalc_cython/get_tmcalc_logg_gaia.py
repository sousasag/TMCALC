#!/usr/bin/python
## My first python code

##imports:

import os
import numpy as np
from astropy import units as u
from astroquery.vizier import Vizier
from astropy.table import Table

import sys
#sys.path.insert(0, "/home/sousasag/Programas/GIT_Projects/MR_spec/")
sys.path.insert(0, "/home/sousasag/Programas/GIT_projects/MR_spec/")
sys.path.insert(0, "/home/sousasag/Programas/GIT_projects/TMCALC/tmcalc_cython/")
teff_cal = "/home/sousasag/Programas/GIT_projects/TMCALC/ratios_list.dat"
feh_cal  = "/home/sousasag/Programas/GIT_projects/TMCALC/feh_calib_lines.dat"
import logg_paralax_mass as loggdr3
import MR_spec as MR
import tmcalc_module as tm


## My functions:

def get_gaiadr3_data(gaiadr2, gaia3, name_search=None):
    vq2 = Vizier(columns=['Source','Plx','e_Plx', 'FG','e_FG','Gmag','e_Gmag', 'BPmag','e_BPmag', 'RPmag','e_RPmag', 'o_Gmag'], row_limit=5000) 
    radius_search = 20.0*u.arcsec
    if name_search is None:
        result_gaia_vizier_dr3=vq2.query_object("Gaia DR2 "+str(gaiadr2), catalog=["I/350/gaiaedr3"], radius=radius_search)
    else:
        result_gaia_vizier_dr3=vq2.query_object(name_search, catalog=["I/350/gaiaedr3"], radius=radius_search*15.)
    try:
        iline3 = np.where(result_gaia_vizier_dr3[0]['Source'] == int(gaia3))[0][0]
        print(result_gaia_vizier_dr3[0]['Source','Plx','e_Plx', 'Gmag', 'e_Gmag', 'FG', 'e_FG', 'RPmag', 'e_RPmag', 'BPmag', 'e_BPmag'][iline3])
        std_g_flux_norm1_v = result_gaia_vizier_dr3[0]['e_FG'][iline3]*np.sqrt(result_gaia_vizier_dr3[0]['o_Gmag'][iline3])/result_gaia_vizier_dr3[0]['FG'][iline3]
    except IndexError:
        result_gaia_vizier_dr3=vq2.query_object("Gaia DR2 "+str(gaiadr2), catalog=["I/350/gaiaedr3"], radius=radius_search*25.)
        try:
            iline3 = np.where(result_gaia_vizier_dr3[0]['Source'] == int(gaia3))[0][0]
            print(result_gaia_vizier_dr3[0]['Source','Plx','e_Plx', 'Gmag', 'e_Gmag', 'FG', 'e_FG', 'RPmag', 'e_RPmag', 'BPmag', 'e_BPmag'][iline3])
            std_g_flux_norm1_v = result_gaia_vizier_dr3[0]['e_FG'][iline3]*np.sqrt(result_gaia_vizier_dr3[0]['o_Gmag'][iline3])/result_gaia_vizier_dr3[0]['FG'][iline3]

        except IndexError:
            result_gaia_vizier_dr3 =  Table( [[-1], [-1],[-1], [-1] ,[-1], [-1] ,[-1] ,[-1]   ,[-1] , [-1]    ] ,
                                   names=('Plx','e_Plx','Gmag','e_Gmag','FG','e_FG','RPmag','e_RPmag','BPmag','e_BPmag'))
            return result_gaia_vizier_dr3, -1
    print("Iline2:", iline3)
    print(result_gaia_vizier_dr3)
    return result_gaia_vizier_dr3[0][iline3], std_g_flux_norm1_v

def get_gaiadr3_dist(gaiadr2, gaia3, name_search=None):
    vqdr3_dist = Vizier(row_limit=5000)
    radius_search = 20.0*u.arcsec
    if name_search is None:
        try:
          result_dist = vqdr3_dist.query_object("Gaia DR2 "+str(gaiadr2), catalog=["I/352/gedr3dis"], radius=radius_search)
          iline_d = np.where(result_dist[0]['Source'] == int(gaia3))[0][0]
        except:
          try:
            result_dist = vqdr3_dist.query_object("Gaia DR2 "+str(gaiadr2), catalog=["I/352/gedr3dis"], radius=20*radius_search)
            iline_d = np.where(result_dist[0]['Source'] == int(gaia3))[0][0]
          except:
            print("problem")
            return -1,-1,-1,-1
    else:
        result_dist = vqdr3_dist.query_object(name_search, catalog=["I/352/gedr3dis"], radius=radius_search)
        iline_d = np.where(result_dist[0]['Source'] == int(gaia3))[0][0]

    rgeo = result_dist[0][iline_d]["rgeo"]
    b_rgeo = result_dist[0][iline_d]["b_rgeo"]
    B_rgeo = result_dist[0][iline_d]["B_rgeo"]
    ergeo = np.max([rgeo-b_rgeo, B_rgeo-rgeo])
    rpgeo = result_dist[0][iline_d]["rgeo"]
    b_rpgeo = result_dist[0][iline_d]["b_rpgeo"]
    B_rpgeo = result_dist[0][iline_d]["B_rpgeo"]
    erpgeo = np.max([rpgeo-b_rpgeo, B_rpgeo-rpgeo])
    return rgeo, ergeo, rpgeo, erpgeo


### Main program:
def main():
  print(sys.argv)
  filein = ""
  if len(sys.argv) <= 2 or len(sys.argv) >= 4:
    print("Wrong usage, please provide ares_file [*.ares] and gaia ID DR3: \n python get_tmcalc_logg_gaia ../HD20619.ares 3261878613062758144")
    return -1
  else:
    filein  = sys.argv[1]
    gaiadr3 = sys.argv[2]

  if filein[-5:] != ".ares":
    print(f"Wrong format for input file: (*{filein[-5:]}). Should be (*.ares).")
    return -1

  teff,erteff,erteff2,erteff3,nout,nindout = tm.get_temperature_py(teff_cal, filein)
  feh, erfeh, nout = tm.get_feh_py(feh_cal, filein, teff, erteff, erteff2, erteff3)

  print(f"  Teff from TMCalc: {teff} +- {erteff2}")
  print(f"[Fe/H] from TMCalc: { feh} +- {  erfeh}")
  gaiadr2 = gaiadr3
  gaia_data, std_g_flux = get_gaiadr3_data(gaiadr2, gaiadr3)
  rgeo, ergeo, rpgeo, erpgeo = get_gaiadr3_dist(gaiadr2, gaiadr3)
  print(gaia_data)
  print(rgeo, ergeo)

  loggsw = 4
  erloggsw = 0.5
  gmag     = gaia_data['Gmag']
  egmag    = gaia_data['e_Gmag']
  gaia_paralax  = gaia_data['Plx']
  egaia_paralax = gaia_data['e_Plx']

  meanlogg, stdlogg, meanmass, stdmass, r, er = loggdr3.get_logg_mass_radius_gaia_torres(teff, erteff2, loggsw, erloggsw, feh, erfeh, gmag, egmag, gaia_paralax, egaia_paralax, distance_gaia=rgeo, e_distance_gaia=ergeo)

  print(meanlogg, stdlogg, meanmass, stdmass, r, er)

  lines = []
#  filename_out = filein.split("/")[-1]+'tmcalc_gaia'
  fileso = open(filein[:-4:]+'tmcalc_gaia', "w")
  lines.append('file\tteff\terteff\tfeh\terfeh\tlogg_dr3\terlogg_dr3\tmass_t\termass_t\tradius_t\terradius_t\n')
  lines.append('----\t----\t------\t---\t-----\t--------\t----------\t------\t--------\t-------\t-----------\n')
  lines.append(f'{filein.split("/")[-1]}\t{teff:.0f}\t{erteff2:.0f}\t{feh:.2f}\t{erfeh:.2f}\t{meanlogg:.2f}\t{stdlogg:.2f}\t{meanmass:.2f}\t{stdmass:.2f}\t{r:.2f}\t{er:.2f}\n')
  for l in lines:
    fileso.write(l)
  fileso.close()



if __name__ == "__main__":
    main()