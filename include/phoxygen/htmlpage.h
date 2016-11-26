/*
 * phoxygen -- PHP documentation tool. (C) 2015--2016 Baubadil GmbH.
 * 
 * phoxygen is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, in version 2 as it comes
 * in the "LICENSE" file of the phoxygen main distribution. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LICENSE file for more details.
 */

#ifndef HTMLPAGE_H
#define HTMLPAGE_H

#include "xwp/basetypes.h"

#include <memory>

class HTMLPage;
typedef shared_ptr<HTMLPage> PHTMLPage;

class HTMLPage
{
public:
    HTMLPage(const string &dirHTMLOut,
             const string &strFilename,
             const string &strTitle,
             const string &strBody);
};

#endif