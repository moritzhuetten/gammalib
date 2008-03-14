#! /usr/bin/env python

from gammalib import *
from math import *
import os

#================#
# Test FITS file #
#================#
def test_fits():
    """
    Test GFits interface.
    """
    # Remove test files
    try:
        os.remove("test_python.fits")
        os.remove("test_python_2.fits")
    except:
        pass

    # Create FITS file
    fits = GFits("test_python.fits")

    # Print number of HDUs
    print "Number of HDUs ..: " + str(fits.num_hdus())

    # Allocate HDU "Testing"
    hdu = GFitsHDU()
    hdu.extname("Testing")
    print "HDU name ........: " + hdu.extname()
    print "HDU number ......: " + str(hdu.extno())
    print "HDU type ........: " + str(hdu.exttype())

    # Set header keywords
    hdr = hdu.header()
    hdr.update(GFitsHeaderCard("test", "test-value", "this is for testing"))
    hdr.update(GFitsHeaderCard("real", 3.1415, "a real value"))
    hdr.update(GFitsHeaderCard("int", 41, "an integer value"))
    

    # Append HDU to FITS file
    fits.append_hdu(hdu)

    # Set double image
    img1 = GFitsDblImage(100)
    img2 = GFitsDblImage(10, 20)
    img3 = GFitsDblImage(10, 10, 10)
    img4 = GFitsDblImage(5, 9, 20, 20)
    print img1
    print img2
    print img3
    print img4
    print img1.bitpix()
    print img3.naxis()
    print img2.naxes(0),  img2.naxes(1)
    print img4.num_pixels()
    fits.append_hdu(GFitsHDU(img1))
    fits.append_hdu(GFitsHDU(img2))
    fits.append_hdu(GFitsHDU(img3))
    fits.append_hdu(GFitsHDU(img4))
    
    # Set ASCII table
    tbl1 = GFitsAsciiTable(10)
    col1 = GFitsTableDblCol("DOUBLE", 10)
    col2 = GFitsTableFltCol("FLOAT", 10)
    col3 = GFitsTableLngCol("LONG", 10)
    col4 = GFitsTableShtCol("SHORT", 10)
    col5 = GFitsTableStrCol("STRING", 10, 20)
    col6 = GFitsTableLogCol("LOGICAL", 10)
    for i in range(10):
        col1.set(i, i*0.01)
        col2.set(i, i*0.01)
        col3.set(i, i*100)
        col4.set(i, i*100)
        col5.set(i, str(i*100))
        col6.set(i, i % 2)
    tbl1.append_column(col1)
    #tbl1.append_column(col2) # FLOAT: datatype conversion overflow (status=412)
    #tbl1.append_column(col3) # LONG: unknown TFORM datatype code (status=262)
    #tbl1.append_column(col4) # SHORT: datatype conversion overflow (status=412)
    tbl1.append_column(col5)
    #tbl1.append_column(col6) # LOGICAL: unknown TFORM datatype code (status=262)
    fits.append_hdu(GFitsHDU(tbl1))

    # Set binary table
    tbl2 = GFitsBinTable(10)
    col1 = GFitsTableDblCol("DOUBLE", 10)
    col2 = GFitsTableFltCol("FLOAT", 10)
    col3 = GFitsTableLngCol("LONG", 10)
    col4 = GFitsTableShtCol("SHORT", 10)
    col5 = GFitsTableStrCol("STRING", 10, 20)
    col6 = GFitsTableLogCol("LOGICAL", 10)
    for i in range(10):
        col1.set(i, i*0.01)
        col2.set(i, i*0.01)
        col3.set(i, i*100)
        col4.set(i, i*100)
        col5.set(i, str(i*100))
        col6.set(i, i % 2)
    tbl2.append_column(col1)
    tbl2.append_column(col2)
    tbl2.append_column(col3)
    tbl2.append_column(col4)
    tbl2.append_column(col5)
    tbl2.append_column(col6)
    fits.append_hdu(GFitsHDU(tbl2))

    #print fits
    
    # Save FITS file
    fits.save()

    # Save into another FITS file
    fits.saveto("test_python_2.fits")


#=================#
# Test Node array #
#=================#
def test_node_array():
    """
    Test GNodeArray interface.
    """
    # Set-up vector and data array
    vector = GVector(20)
    data   = GVector(20)
    for i in range(20):
        vector[i] = 10.0 + i*5.0
        data[i]   = sin(0.15*(vector[i]-10.0))
    
    # Set-up node array
    array = GNodeArray()
    array.nodes(vector)
    
    # Get values
    x_val = []
    y_val = []
    for i in range(100):
        x = i-10
        array.set_value(x)
        inx_left  = array.inx_left()
        inx_right = array.inx_right()
        wgt_left  = array.wgt_left()
        wgt_right = array.wgt_right()
        y         = wgt_left*data[inx_left] + wgt_right*data[inx_right]
        x_val.append(x)
        y_val.append(y)


#===================#
# Test LAT response #
#===================#
def test_lat_response():
    """
    Test GLATResponse interface.
    """
    # Remove test file
    try:
        os.remove("test_rsp.fits")
    except:
        pass
    
    # Allocate LAT response
    rsp = GLATResponse()
    
    # Set calibration database
    rsp.set_caldb("irf/lat")
    
    # Load response
    rsp.load("Pass5_v0", "front")
    
    # Save response
    rsp.save("test_rsp.fits")


#======================#
# Test LAT observation #
#======================#
def test_lat_observation():
    """
    Test GLATObservation interface.
    """
    # Allocate LAT Observation
    obs = GLATObservation("data/FT1_253582800.fits.gz", "data/FT2_253582800.fits.gz")

    #
    #print obs.ft1()
    #print obs.ft2()


#==============#
# Test Healpix #
#==============#
def test_healpix():
    """
    Test GHealpix interface.
    """
    # Open exposure cube
    fits = GFits("data/expCube_253582800.fits.gz")
    
    # Lood Healpix table
    pixels = GHealpix(fits.hdu("EXPOSURE"))
    
    # Dump information
    print pixels
    
    print pixels.pix2ang(0)
    print " ",pixels.pix2ang(0).ra()*180/pi, pixels.pix2ang(0).dec()*180/pi
    print pixels.pix2ang(1)
    print pixels.pix2ang(2)
    print pixels.pix2ang(3)
    

#==========================#
# Main routine entry point #
#==========================#
if __name__ == '__main__':
    """
    Perform testing.
    """
    test_fits()
    test_node_array()
    test_lat_response()
    test_lat_observation()
    test_healpix()
    