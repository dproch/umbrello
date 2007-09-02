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
 *      Date   : Wed Jul 16 2003
 */
#include "javaclassdeclarationblock.h"
#include "javacodedocumentation.h"
#include "../codegenerator.h"
#include "../codegenerationpolicy.h"
#include "../uml.h"

// Constructors/Destructors
//

JavaClassDeclarationBlock::JavaClassDeclarationBlock
 ( JavaClassifierCodeDocument * parentDoc, const QString &startText, const QString &endText, const QString &comment)
        : OwnedHierarchicalCodeBlock(parentDoc->getParentClassifier(), parentDoc, startText, endText, comment)
{
    init(parentDoc, comment);
}

JavaClassDeclarationBlock::~JavaClassDeclarationBlock ( ) { }

//
// Methods
//

/**
 * Save the XMI representation of this object
 */
void JavaClassDeclarationBlock::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
    QDomElement blockElement = doc.createElement( "javaclassdeclarationblock" );

    setAttributesOnNode(doc, blockElement);

    root.appendChild( blockElement );
}

/**
 * load params from the appropriate XMI element node.
 */
void JavaClassDeclarationBlock::loadFromXMI ( QDomElement & root )
{
    setAttributesFromNode(root);
}

// Accessor methods
//

// Other methods
//

/**
 * update the start and end text for this ownedhierarchicalcodeblock.
 */
void JavaClassDeclarationBlock::updateContent ( )
{

    JavaClassifierCodeDocument *parentDoc = dynamic_cast<JavaClassifierCodeDocument*>(getParentDocument());
    UMLClassifier *c = parentDoc->getParentClassifier();
    CodeGenerationPolicy *commonPolicy = UMLApp::app()->getCommonPolicy();
    QString endLine = commonPolicy->getNewLineEndingChars();
    bool isInterface = parentDoc->parentIsInterface(); // a little shortcut
    QString JavaClassName = parentDoc->getJavaClassName(c->getName());

    // COMMENT
    if(isInterface)
        getComment()->setText("Interface "+JavaClassName+endLine+c->getDoc());
    else
        getComment()->setText("Class "+JavaClassName+endLine+c->getDoc());

    bool forceDoc = UMLApp::app()->getCommonPolicy()->getCodeVerboseDocumentComments();
    if(forceDoc || !c->getDoc().isEmpty())
        getComment()->setWriteOutText(true);
    else
        getComment()->setWriteOutText(false);


    // Now set START/ENDING Text
    QString startText = "";
    // In Java, we need declare abstract only on classes
    if (c->getAbstract() && !isInterface)
        startText.append("abstract ");

    if (c->getVisibility() != Uml::Visibility::Public) {
        // We should probably emit a warning in here .. java doesn't like to allow
        // private/protected classes. The best we can do (I believe)
        // is to let these declarations default to "package visibility"
        // which is a level between traditional "private" and "protected"
        // scopes. To get this visibility level we just print nothing..
    } else
        startText.append("public ");

    if(parentDoc->parentIsInterface())
        startText.append("interface ");
    else
        startText.append("class ");

    startText.append(JavaClassName);

    // write inheritances out
    UMLClassifierList superclasses =
        c->findSuperClassConcepts(UMLClassifier::CLASS);
    UMLClassifierList superinterfaces =
        c->findSuperClassConcepts(UMLClassifier::INTERFACE);
    int nrof_superclasses = superclasses.count();
    int nrof_superinterfaces = superinterfaces.count();

    // write out inheritance
    int i = 0;
    if(nrof_superclasses >0)
        startText.append(" extends ");
    foreach (UMLClassifier* concept, superclasses ) {
        startText.append(parentDoc->cleanName(concept->getName()));
        if(i != (nrof_superclasses-1))
            startText.append(", ");
        i++;
    }

    // write out what we 'implement'
    i = 0;
    if(nrof_superinterfaces >0)
    {
        // In Java interfaces "extend" other interfaces. Classes "implement" interfaces
        if(isInterface)
            startText.append(" extends ");
        else
            startText.append(" implements ");
    }
    foreach (UMLClassifier* concept, superinterfaces ) {
        startText.append(parentDoc->cleanName(concept->getName()));
        if(i != (nrof_superinterfaces-1))
            startText.append(", ");
        i++;
    }

    // Set the header and end text for the hier.codeblock
    setStartText(startText+" {");

    // setEndText("}"); // not needed

}

void JavaClassDeclarationBlock::init (JavaClassifierCodeDocument *parentDoc, const QString &comment)
{

    setComment(new JavaCodeDocumentation(parentDoc));
    getComment()->setText(comment);

    setEndText("}");
}


#include "javaclassdeclarationblock.moc"
