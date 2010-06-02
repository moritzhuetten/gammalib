/***************************************************************************
 *                GLATEventBin.cpp  -  LAT event bin class                 *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
/**
 * @file GLATEventBin.cpp
 * @brief GLATEventBin class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include "GException.hpp"
#include "GLATEventBin.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_MODEL                     "GLATEventBin::model(GModels&, GVector*)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GLATEventBin::GLATEventBin(void) : GEventBin()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] bin Event bin from which the instance should be built.
 ***************************************************************************/
GLATEventBin::GLATEventBin(const GLATEventBin& bin) : GEventBin(bin)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(bin);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GLATEventBin::~GLATEventBin(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] bin Event bin which should be assigned.
 ***************************************************************************/
GLATEventBin& GLATEventBin::operator= (const GLATEventBin& bin)
{
    // Execute only if object is not identical
    if (this != &bin) {

        // Copy base class members
        this->GEventBin::operator=(bin);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(bin);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Return model value and (optionally) gradient
 *
 * @param[in] models Model descriptor.
 * @param[out] gradient Pointer to gradient vector (NULL=not computed).
 *
 * @exception GException::gradient_par_mismatch
 *            Dimension of gradient vector mismatches number of parameters.
 *
 * Implements generic model and gradient evaluation for the LAT instrument.
 *
 * @todo Requires multiplication of model with solid angle, duration, and
 *       energy binsize. Current method not correct.
 * @todo Requires implementation of all model types (not only factorized
 *       point sources which are currently the only type that is
 *       supported)
 ***************************************************************************/
double GLATEventBin::model(GModels& models, GVector* gradient) const
{
    // Verify that gradients vector has the same dimension than the
    // model has parameters
    #if defined(G_RANGE_CHECK)
    if (models.npars() != gradient->size())
        throw GException::gradient_par_mismatch(G_MODEL, gradient->size(), 
                                                models.npars());
    #endif

    // Integral over source direction, energy and time
    //for (srcTime = ...
    //for (srcEng = ...
    //for (srcDir = ...
    GTime   srcTime = *time();    // Assume no time dispersion
    GEnergy srcEng  = *energy();  // Assume no energy dispersion
    GSkyDir srcDir;               // Needs to be implemented

    // Get source term
    double source = models.eval_gradients(srcDir, srcEng, srcTime);
    
    // Get IRF
    double irf = rsp()->irf(*dir(), *energy(), *time(), 
                            srcDir, srcEng, srcTime, *pnt());
    
    // Evaluate model
    double model = source * irf;

    // Set gradient vector
    if (gradient != NULL) {
        for (int i = 0; i < gradient->size(); ++i)
            (*gradient)(i) = irf * models.par(i)->gradient();
    }

    //}
    //}
    //}

    // Return
    return model;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GLATEventBin::init_members(void)
{
    // Initialise attributes
    m_counts = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] bin GLATEventBin members which should be copied.
 ***************************************************************************/
void GLATEventBin::copy_members(const GLATEventBin& bin)
{
    // Copy attributes
    m_counts = bin.m_counts;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GLATEventBin::free_members(void)
{
    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone class
***************************************************************************/
GLATEventBin* GLATEventBin::clone(void) const
{
    return new GLATEventBin(*this);
}


/*==========================================================================
 =                                                                         =
 =                                Friends                                  =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Put bin into output stream
 *
 * @param[in] os Output stream into which the bin will be dumped
 * @param[in] bin Bin to be dumped
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GLATEventBin& bin)
{
    // Put bin in output stream
    os << bin.m_counts << " ";
        
    // Return output stream
    return os;
}
