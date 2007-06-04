/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Thu Jun 19 2003
 */


#ifndef CPPCODEGENERATOR_H
#define CPPCODEGENERATOR_H

#include <qstring.h>
#include "../codedocumentlist.h"
#include "../codeviewerstate.h"
#include "../codegenerator.h"
#include "../umldoc.h"
#include "cppmakecodedocument.h"

class CodeViewerDialog;
class CPPHeaderCodeDocument;

class CPPCodeGenerator : public CodeGenerator
{
    Q_OBJECT
public:

    static const bool DEFAULT_BUILD_MAKEFILE;

    // Constructors/Destructors
    //

    /**
     * Basic Constructor
     */
    CPPCodeGenerator ();

    /**
     * Empty Destructor
     */
    virtual ~CPPCodeGenerator ( );

    /**
     * Set the value of m_createMakefile
     * @param new_var the new value of m_createMakefile
     */
    void setCreateProjectMakefile ( bool new_var );

    /**
     * Get the value of m_createMakefile
     * @return the value of m_createMakefile
     */
    bool getCreateProjectMakefile ( );

    // Public attribute accessor methods
    //

    QString fixTypeName(const QString &string);

    /**
     * Add a header CodeDocument object from m_headercodedocumentVector List
     */
    bool addHeaderCodeDocument ( CPPHeaderCodeDocument * doc );

    /**
     * Remove a header CodeDocument object from m_headercodedocumentVector List
     */
    bool removeHeaderCodeDocument ( CPPHeaderCodeDocument * remove_object );

    /**
     * returns "Cpp"
     */
    Uml::Programming_Language getLanguage();

    // generate 2 different types of classifier code documents.
    CodeDocument * newClassifierCodeDocument (UMLClassifier * classifier);
    CPPHeaderCodeDocument * newHeaderClassifierCodeDocument (UMLClassifier * classifier);

    /** Get the editing dialog for this code document
     */
    virtual CodeViewerDialog * getCodeViewerDialog( QWidget* parent, CodeDocument * doc,
            Settings::CodeViewerState state);

    /**
     * Write out all code documents to file as appropriate.
     */
    virtual void writeCodeToFile ( );

    // this method is here to provide class wizard the
    // ability to write out only those classes which
    // are selected by the user.
    virtual void writeCodeToFile(UMLClassifierList &list);

    /**
     * Add C++ primitives as datatypes
     */
    QStringList defaultDatatypes();

    /**
     * Save the XMI representation of this object
     */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    /**
     * get list of reserved keywords
     */
    virtual const QStringList reservedKeywords() const;

    /**
     * Add the default stereotypes for c++ (constructor, int etc)
     */
    virtual void createDefaultStereotypes ();

    void initFromParentDocument( );

protected:

    /**
     * @return  CPPMakeCodeDocument
     * @param   this
     */
    CPPMakefileCodeDocument * newMakefileCodeDocument ( );

    /**
     * Find a cppheadercodedocument by the given classifier.
     * @return      CPPHeaderCodeDocument
     * @param       classifier
     */
    CPPHeaderCodeDocument * findHeaderCodeDocumentByClassifier (UMLClassifier * classifier );

private:

    bool m_createMakefile;

    // a separate list for recording the header documents
    CodeDocumentList m_headercodedocumentVector;

    void initAttributes ( ) ;

public slots:

    /** These 2 functions check for adding or removing objects to the UMLDocument
     * they are need to be overridden here because unlike in the Java (or most other lang)
     * we add 2 types of classifiercodedocument per classifier,
     * e.g. a "source" and a "header" document.
     */
    virtual void checkAddUMLObject (UMLObject * obj);
    virtual void checkRemoveUMLObject (UMLObject * obj);

    /**
     * Force a synchronize of this code generator, and its present contents, to that of the parent UMLDocument.
     * "UserGenerated" code will be preserved, but Autogenerated contents will be updated/replaced
     * or removed as is apppropriate.
     */
    virtual void syncCodeToDocument ( );
};

#endif // CPPCODEGENERATOR_H
