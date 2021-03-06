/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2006-2020                                                *
 *  Umbrello UML Modeller Authors <umbrello-devel@kde.org>                 *
 ***************************************************************************/

#ifndef OBJECT_FACTORY__H
#define OBJECT_FACTORY__H

#include "umlobject.h"

#include <QString>

class UMLPackage;
class UMLClassifier;
class UMLClassifierListItem;
class UMLAttribute;
class UMLOperation;

namespace Object_Factory {

UMLObject* createUMLObject(UMLObject::ObjectType type,
                           const QString &n = QString(),
                           UMLPackage *parentPkg = 0,
                           bool solicitNewName = true);

UMLObject* createNewUMLObject(UMLObject::ObjectType type,
                              const QString &n,
                              UMLPackage *parentPkg,
                              bool undoable = true);

UMLClassifierListItem* createChildObject(UMLClassifier *parent,
                                         UMLObject::ObjectType type,
                                         const QString& name = QString());

UMLAttribute *createAttribute(UMLObject *parent, const QString& name,
                              UMLObject *type = 0);

UMLOperation *createOperation(UMLClassifier *parent, const QString& name);

void assignUniqueIdOnCreation(bool yesno);
bool assignUniqueIdOnCreation();

UMLObject* makeObjectFromXMI(const QString& xmiTag,
                             const QString& stereoID = QString());

}

#endif
