/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2005-2006                                                *
 *  Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                   *
 ***************************************************************************/

#ifndef IMPORT_UTILS_H
#define IMPORT_UTILS_H

#include <qstringlist.h>
#include "../umlnamespace.h"
#include "../umlattributelist.h"

class UMLObject;
class UMLClassifier;
class UMLPackage;
class UMLOperation;
class UMLEnum;

/**
 * Utilities for code import
 * @author Oliver Kellogg
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */

namespace Import_Utils {

    /**
     * Find or create a document object.
     */
    UMLObject* createUMLObject(Uml::Object_Type type,
                               const QString& name,
                               UMLPackage *parentPkg = NULL,
                               const QString& comment = QString::null,
                               const QString& stereotype = QString::null);
    /**
     * Control whether an object which is newly created by createUMLObject()
     * is put at the global scope.
     *
     * @param yesno  When set to false, the object is created at the scope
     *               given by the parentPkg argument of createUMLObject().
     */
    void putAtGlobalScope(bool yesno);

    /**
     * Set a related classifier for creation of dependencies on template
     * parameters in createUMLObject().
     */
    void setRelatedClassifier(UMLClassifier *c);

    /**
     * Control whether the creation methods solicit a new unique ID for the
     * created object.
     * By default, unique ID generation is turned on.
     *
     * @param yesno   False turns UID generation off, true turns it on.
     */
    void assignUniqueIdOnCreation(bool yesno);

    /**
     * Create a UMLAttribute and insert it into the document.
     */
    UMLObject* insertAttribute(UMLClassifier *klass, Uml::Visibility scope,
                               const QString& name,
                               const QString& type,
                               const QString& comment = QString::null,
                               bool isStatic = false);
    /**
     * Create a UMLAttribute and insert it into the document.
     * Use the specified existing attrType.
     */
    UMLObject* insertAttribute(UMLClassifier *klass, Uml::Visibility scope,
                               const QString& name,
                               UMLClassifier *attrType,
                               const QString& comment /* ="" */,
                               bool isStatic /* =false */);
    /**
     * Create a UMLOperation.
     * The reason for this method is to not generate any Qt signals.
     * Instead, these are generated by insertMethod().
     * (If we generated a creation signal prematurely, i.e. without
     * the method parameters being known yet, then that would lead to
     * a conflict with a pre-existing parameterless method of the same
     * name.)
     */
    UMLOperation* makeOperation(UMLClassifier *parent, const QString &name);

    /**
     * Insert the UMLOperation into the given classifier.
     *
     * @param klass  The classifier into which the operation shall be added.
     * @param op     Reference to pointer to the temporary UMLOperation
     *               for insertion.  The caller relinquishes ownership of the
     *               object pointed to.  If an UMLOperation of same signature
     *               already exists  at the classifier then the incoming
     *               UMLOperation is deleted and the pointer is set to the
     *               existing UMLOperation.
     */
    void insertMethod(UMLClassifier *klass, UMLOperation* &op,
                      Uml::Visibility scope, const QString& type,
                      bool isStatic, bool isAbstract,
                      bool isFriend = false, bool isConstructor = false,
                      const QString& comment = QString::null);

    /**
     * Add an argument to a UMLOperation.
     * The parentPkg arg is only used for resolving possible scope
     * prefixes in the `type'.
     */
    UMLAttribute* addMethodParameter(UMLOperation *method,
                                     const QString& type,
                                     const QString& name);

    /**
     * Add an enum literal to an UMLEnum.
     */
    void addEnumLiteral(UMLEnum *enumType, const QString &literal,
                        const QString &comment = QString());

    /**
     * Create a generalization from the given child classifier to the given
     * parent classifier.
     */
    void createGeneralization(UMLClassifier *child, UMLClassifier *parent);

    /**
     * Create a generalization from the existing child UMLObject to the given
     * parent class name.
     */
    void createGeneralization(UMLClassifier *child, const QString &parentName);

    /**
     * Strip comment lines of leading whitespace and stars.
     */
    QString formatComment(const QString &comment);

    /**
     * Return the list of paths set by previous calls to addIncludePath()
     * and the environment variable UMBRELLO_INCPATH.
     * This list can be used for finding #included (or Ada with'ed or...)
     * files.
     */
    QStringList includePathList();

    /**
     * Add a path to the include path list.
     */
    void addIncludePath(const QString& path);

    /**
     * Returns whether the last createUMLObject() actually created
     * a new object or just returned an existing one.
     */
    bool newUMLObjectWasCreated();

    /**
     * Returns true if a type is an actual Datatype
     */
    bool isDatatype(const QString& name, UMLPackage *parentPkg = NULL);

} // end namespace Import_Utils

#endif
