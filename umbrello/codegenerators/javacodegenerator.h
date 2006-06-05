/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                 *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Thu Jun 19 2003
 */



#ifndef JAVACODEGENERATOR_H
#define JAVACODEGENERATOR_H

#include <qstring.h>
#include "../codeviewerstate.h"
#include "../codegenerator.h"
#include "../codeblockwithcomments.h"
#include "../umldoc.h"

#include "classifierinfo.h"
#include "javaclassifiercodedocument.h"
#include "javaantcodedocument.h"

#include "javacodegenerationpolicy.h"

class CodeViewerDialog;

class JavaCodeGenerator : public CodeGenerator
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //

    static const bool DEFAULT_BUILD_ANT_DOC;

    /**
     * Empty Constructor
     */
    JavaCodeGenerator ();
    JavaCodeGenerator (QDomElement & element);

    /**
     * Empty Destructor
     */
    virtual ~JavaCodeGenerator ( );

    // Public attributes
    //


    // Public attribute accessor methods
    //


    /**
     * Set the value of m_createANTBuildFile
     * @param new_var the new value of m_createANTBuildFile
     */
    void setCreateANTBuildFile ( bool new_var );

    /**
     * Get the value of m_createANTBuildFile
     * @return the value of m_createANTBuildFile
     */
    bool getCreateANTBuildFile ( );

    /**
     * A utility method to get the javaCodeGenerationPolicy()->getAutoGenerateAttribAccessors() value.
     */
    bool getAutoGenerateAttribAccessors( );

    /**
     * A utility method to get the javaCodeGenerationPolicy()->getAutoGenerateAssocAccessors() value.
     */
    bool getAutoGenerateAssocAccessors( );

    /**
     * Get the list variable class name to use. For Java, we have set this to "Vector".
     */
    static QString getListFieldClassName();

    /** Get the editing dialog for this code document
     */
    virtual CodeViewerDialog * getCodeViewerDialog( QWidget* parent, CodeDocument * doc,
            Settings::CodeViewerState state);

    // Other methods
    //

    // general purpose function we may reuse for all types of Java code documents
    static QString capitalizeFirstLetter(QString string);

    /**
     * Utility function for getting the java code generation policy.
     */
    JavaCodeGenerationPolicy * getJavaPolicy();

    /**
     * @return  ClassifierCodeDocument
     * @param   classifier 
     */
    CodeDocument * newClassifierCodeDocument (UMLClassifier * classifier);

    // return "Java"
    Uml::Programming_Language getLanguage();

    /**
     * Adds Java's primitives as datatypes
     */
    virtual QStringList defaultDatatypes();

    /**
     * IF the type is "string" we need to declare it as
     * the Java Object "String" (there is no string primative in Java).
     * Same thing again for "bool" to "boolean". 
     */
    static QString fixTypeName(QString string);

    /**
     * get list of reserved keywords
     */
    virtual const QStringList reservedKeywords() const;

protected:

    /** create the codeblock that will represent the class declaration
     *  for this classifier
     */
    CodeBlockWithComments * createClassDecl ( UMLClassifier *c, ClassifierInfo *info, JavaClassifierCodeDocument * doc);

    /**
     * @return  JavaANTCodeDocument
     */
    JavaANTCodeDocument * newANTCodeDocument ( );

private:

    void initFields( ) ;

    bool m_createANTBuildFile;
};

#endif // JAVACODEGENERATOR_H
