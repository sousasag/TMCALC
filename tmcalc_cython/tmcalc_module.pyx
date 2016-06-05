""" Example of wrapping tmcalc c functions using Cython. """


# import both numpy and the Cython declarations for numpy
import numpy as np
cimport numpy as np

cdef extern from "lineRatioCalibCore.h":
  long file_lines(char* a)
  void get_temperature(char* filecal,
                       char* fileares,
                       double* Teffout, 
                       double* erTeffout1, 
                       double* erTeffout2, 
                       double* erTeffout3, 
                       long* nout, 
                       long* nindout)
  void get_feh(char* filecal, 
               char* fileares, 
               double teff, 
               double erteff1, 
               double erteff2, 
               double erteff3, 
               double* fehout, 
               double* erfehout, 
               long* nout)


def file_linespy(arg1):
  """
  Returns the number of lines in a file
  """
  return file_lines(arg1)

def get_temperature_py(filecal, fileares):
  """
  Derive the temperature from the EW line-ratio calibrations 
  reading a ares files and a the calibration file

  call like:
  get_temperature_py('ratios_list.dat', 'HD20619.ares')

  returns a tuple like:
  (teff,erteff,erteff2,erteff3,nout,nindout)
  """

  teffa = np.array([0],dtype=float)
  erteffa = np.array([0],dtype=float)
  erteff2a = np.array([0],dtype=float)
  erteff3a = np.array([0],dtype=float)
  nouta = np.array([0],dtype=int)
  nindouta = np.array([0],dtype=int)

  get_temperature(filecal,
                  fileares,
                  <double*> np.PyArray_DATA(teffa), 
                  <double*> np.PyArray_DATA(erteffa),
                  <double*> np.PyArray_DATA(erteff2a),
                  <double*> np.PyArray_DATA(erteff3a),
                  <long*> np.PyArray_DATA(nouta),
                  <long*> np.PyArray_DATA(nindouta))

  teff=teffa[0]
  erteff=erteffa[0]
  erteff2=erteff2a[0]
  erteff3=erteff3a[0]
  nout=nouta[0]
  nindout=nindouta[0]

  return (teff,erteff,erteff2,erteff3,nout,nindout)


def get_feh_py(filecal, fileares, teff, erteff, erteff2, erteff3):
  """
  Derive the feh from the iron lines calibrations 
  reading a ares files and a the calibration file and given a teff

  call like:
  get_feh_py('feh_calib_lines.dat', 'HD20619.ares', teff, erteff, erteff2, erteff3)

  returns a tuple like:
  (fehout, erfehout, nout)
  """

  fehouta = np.array([0],dtype=float)
  erfehouta = np.array([0],dtype=float)
  nouta = np.array([0],dtype=int)

  get_feh(filecal,
          fileares,
          teff, 
          erteff,
          erteff2,
          erteff3,
          <double*> np.PyArray_DATA(fehouta),
          <double*> np.PyArray_DATA(erfehouta),
          <long*> np.PyArray_DATA(nouta))

  fehout=fehouta[0]
  erfehout=erfehouta[0]
  nout=nouta[0]
  return (fehout, erfehout, nout)


