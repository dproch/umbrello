

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Sep 1 2003
 */

#ifndef CPPHEADERCLASSDECLARATIONBLOCK_H
#define CPPHEADERCLASSDECLARATIONBLOCK_H

#include <qstring.h>

#include "cppheadercodedocument.h"
#include "../ownedhierarchicalcodeblock.h"

class CPPHeaderClassDeclarationBlock : public OwnedHierarchicalCodeBlock
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //

    /**
     * Empty Constructor
     */
    explicit CPPHeaderClassDeclarationBlock ( CPPHeaderCodeDocument * parentDoc, const QString &start = "", const QString &endText = "}", const QString &comment = "");

    /**
     * Empty Destructor
     */
    virtual ~CPPHeaderClassDeclarationBlock ( );

    /**
     * Update the start/end text of this codeblock.
     */
    void updateContent ( );

    /**
     * Save the XMI representation of this object
     */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    virtual void loadFromXMI ( QDomElement & root );

protected:

    /** set the class attributes from a passed object
     */
    virtual void setAttributesFromObject (TextBlock * obj);

private:

    void init (CPPHeaderCodeDocument * parent, const QString &comment);

};

#endif // CPPHEADERCLASSDECLARATIONBLOCK_H
