/***************************************************************************
 *                   GModelPar.cpp  -  Model parameter class               *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GModelPar.cpp
 * @brief GModelPar class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GModelPar.hpp"
#include "GTools.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_VALUE                             "GModelPar::value(const double&)"
#define G_MIN                                 "GModelPar::min(const double&)"
#define G_MAX                                 "GModelPar::max(const double&)"
#define G_RANGE               "GModelPar::range(const double&,const double&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                     GModelPar constructors/destructors                  =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GModelPar::GModelPar(void)
{
    // Initialise private members for clean destruction
    init_members();
  
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] par Parameter from which the instance should be built.
 ***************************************************************************/
GModelPar::GModelPar(const GModelPar& par)
{ 
    // Initialise private members for clean destruction
    init_members();

    // Copy members
    copy_members(par);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelPar::~GModelPar()
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                           GModelPar operators                           =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] par Parameter which should be assigned.
 ***************************************************************************/
GModelPar& GModelPar::operator= (const GModelPar& par)
{ 
    // Execute only if object is not identical
    if (this != &par) {

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(par);

    } // endif: object was not identical
  
    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                         GModelPar public methods                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Set parameter value
 *
 * @param[in] value Parameter value.
 ***************************************************************************/
void GModelPar::value(const double& value)
{
    // If there is a minimum boundary and if value is below this boundary
    // then throw an error
    if (m_hasmin && value < m_min)
        throw GException::out_of_range(G_VALUE, value, m_min, m_max);

    // If there is a maximum boundary and if value is above this boundary
    // then throw an error
    if (m_hasmax && value > m_max)
        throw GException::out_of_range(G_VALUE, value, m_min, m_max);

    // Assign value
    m_value = value;
	
    // Return
    return;
}


/***********************************************************************//**
 * @brief Set parameter minimum
 *
 * @param[in] min Parameter minimum.
 ***************************************************************************/
void GModelPar::min(const double& min)
{
    // If minimum is above actual value then throw error
    if (m_value < min)
        throw GException::out_of_range(G_MIN, m_value, min, m_max);

    // Assign minimum
    m_min = min;
    
    // Flag that minimum was set
    m_hasmin = true;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Set parameter maximum
 *
 * @param[in] max Parameter maximum.
 ***************************************************************************/
void GModelPar::max(const double& max)
{
    // If maximum is below value then throw error
    if (m_value > max)
        throw GException::out_of_range(G_MAX, m_value, m_min, max);

    // Assign maximum
    m_max = max;
    
    // Flag that maximum was set
    m_hasmax = true;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Set parameter minimum and maximum
 *
 * @param[in] min Parameter minimum.
 * @param[in] max Parameter maximum.
 ***************************************************************************/
void GModelPar::range(const double& min, const double& max)
{
    // If maximum is below value then throw error
    if (m_value < min || m_value > max || min > max)
        throw GException::out_of_range(G_RANGE, m_value, min, max);

    // Assign range
    m_min = min;
    m_max = max;
    
    // Flag that range was set
    m_hasmin = true;
    m_hasmax = true;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Extract parameter attributes from XML element
 *
 * @param[in] xml XML element containing parameter attributes.
 *
 * The following parameter attributes may exist (they take the default
 * values when they are not found in the XML element):
 * 'value' (default=0.0), 'error' (default=0.0), 'scale' (default=1.0),
 * 'min' (default=INDEF), 'max' (default=INDEF), 'free' (default=0).
 ***************************************************************************/
void GModelPar::read(const GXmlElement& xml)
{
    // Get value
    std::string arg = xml.attribute("value");
    if (arg != "")
        m_value = todouble(arg);
    else
        m_value = 0.0;

    // Get error
    arg = xml.attribute("error");
    if (arg != "")
        m_error = todouble(arg);
    else
        m_error = 0.0;

    // Get scale factor
    arg = xml.attribute("scale");
    if (arg != "")
        m_scale = todouble(arg);
    else
        m_scale = 1.0;

    // Get min
    arg = xml.attribute("min");
    if (arg != "")
        min(todouble(arg));

    // Get max
    arg = xml.attribute("max");
    if (arg != "")
        max(todouble(arg));

    // Get free
    if (xml.attribute("free") == "1" || 
        tolower(xml.attribute("free")) == "true")
        free();
    else
        fix();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set or update parameter attributes in XML element
 *
 * @param[in] xml XML element containing parameter attributes.
 *
 * The following parameter attributes may exist (they take the default
 * values when they are not found in the XML element):
 * 'value' (default=0.0), 'error' (default=0.0), 'scale' (default=1.0),
 * 'min' (default=INDEF), 'max' (default=INDEF), 'free' (default=0).
 ***************************************************************************/
void GModelPar::write(GXmlElement& xml) const
{
    // Set value
    xml.attribute("value", str(m_value));

    // Set error (only if parameter is free)
    if (isfree())
        xml.attribute("error", str(m_error));

    // Set scale
    xml.attribute("scale", str(m_scale));

    // Set minimum
    if (hasmin())
        xml.attribute("min", str(m_min));

    // Set maximum
    if (hasmax())
        xml.attribute("max", str(m_max));

    // Set free/fix flag
    if (isfree())
        xml.attribute("free", "1");
    else
        xml.attribute("free", "0");

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                        GModelPar private methods                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelPar::init_members(void)
{
    // Initialise members
    m_name.clear();
    m_unit.clear();
    m_value    = 0.0;
    m_error    = 0.0;
    m_gradient = 0.0;
    m_min      = 0.0;
    m_max      = 0.0;
    m_scale    = 1.0;
    m_hasmin   = false;
    m_hasmax   = false;
    m_free     = true;
  
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] par GModelPar members which should be copied.
 ***************************************************************************/
void GModelPar::copy_members(const GModelPar& par)
{
    // Copy members
    m_name    = par.m_name;
    m_unit    = par.m_unit;
    m_value   = par.m_value;
    m_error   = par.m_error;
    m_gradient = par.m_gradient;
    m_min     = par.m_min;
    m_max     = par.m_max;
    m_scale   = par.m_scale;
    m_hasmin  = par.m_hasmin;
    m_hasmax  = par.m_hasmax;
    m_free    = par.m_free;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelPar::free_members(void)
{
  
    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             GModelPar friends                           =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Put parameter in output stream
 *
 * @param[in] os Output stream into which the parameter will be dumped.
 * @param[in] par Parameter to be dumped.
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GModelPar& par)
{
    // Put parameter in stream
    os << std::scientific;
    os << par.m_name << ": ";
    os << par.real_value();
    if (par.m_free) {
        os << " +/- ";
        os << par.real_error();
    }
    if (par.m_hasmin && par.m_hasmax)
        os << " [" << par.real_min() << "," << par.real_max() << "]";
    else if (par.m_hasmin)
        os << " [" << par.real_min() << ",infty[";
    else if (par.m_hasmax)
        os << " ]-infty," << par.real_max() << "]";
    os << " " << par.m_unit;
    if (par.m_free)
        os << " (free,";
    else
        os << " (fixed,";
    os << "scale=" << par.m_scale << ")";

    // Return output stream
    return os;
}


/***********************************************************************//**
 * @brief Write parameter in logger
 *
 * @param[in] log Logger.
 * @param[in] par Parameter to be written.
 ***************************************************************************/
GLog& operator<<(GLog& log, const GModelPar& par)
{
    // Write parameter name
    log << par.m_name << ": ";

    // Write value
    log << par.real_value();

    // For free parameters, write statistical uncertainty
    if (par.m_free) {
        log << " +/- ";
        log << par.real_error();
    }

    // Write parameter limites if they exist
    if (par.m_hasmin && par.m_hasmax)
        log << " [" << par.real_min() << "," << par.real_max() << "]";
    else if (par.m_hasmin)
        log << " [" << par.real_min() << ",infty[";
    else if (par.m_hasmax)
        log << " ]-infty," << par.real_max() << "]";

    // Write parameter unit
    log << " " << par.m_unit;

    // Signal if parameter was free or fixed
    if (par.m_free)
        log << " (free,";
    else
        log << " (fixed,";

    // Write parameter scale
    log << "scale=" << par.m_scale << ")";

    // Return logger
    return log;
}
