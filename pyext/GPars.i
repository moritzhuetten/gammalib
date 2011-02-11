/***************************************************************************
 *            GPars.i - Application parameters Python interface            *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GPars.i
 * @brief Application parameter container class Python interface definition
 * @author Jurgen Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GPars.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GPars
 *
 * @brief Application parameter container class
 ***************************************************************************/
class GPars {

public:
    // Constructors and destructors
    GPars(void);
    explicit GPars(const std::string& filename);
    explicit GPars(const std::string& filename, const std::vector<std::string>& args);
    GPars(const GPars& pars);
    virtual ~GPars(void);
 
    // Methods
    void        clear(void);
    int         size(void) const;
    void        load(const std::string& filename);
    void        load(const std::string& filename, const std::vector<std::string>& args);
    void        save(const std::string& filename);
    bool        haspar(const std::string& name) const;
};


/***********************************************************************//**
 * @brief GPars class extension
 ***************************************************************************/
%extend GPars {
    char *__str__() {
        return tochar(self->print());
    }
    GPar& __getitem__(const int& index) {
        if (index >= 0 && index < self->size())
            return (*self)[index];
        else
            throw GException::out_of_range("__getitem__(int)", index, self->size());
    }
    GPar& __getitem__(const std::string& name) {
        return (*self)[name];
    }
    void __setitem__(const int& index, const GPar& par) {
        if (index>=0 && index < self->size()) {
            (*self)[index] = par;
            return;
        }
        else
            throw GException::out_of_range("__setitem__(int)", index, self->size());
    }
    void __setitem__(const std::string& name, const GPar& par) {
        (*self)[name] = par;
        return;
    }
    GPars copy() {
        return (*self);
    }
}
