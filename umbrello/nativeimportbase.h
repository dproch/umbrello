#ifndef NATIVEIMPORTBASE_H
#define NATIVEIMPORTBASE_H

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2005                                                     *
 *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                  *
 ***************************************************************************/

#include <qstring.h>
#include <qstringlist.h>
#include "classimport.h"
#include "umlnamespace.h"

class UMLPackage;
class UMLClassifier;

/**
 * Intermediate base class for native Umbrello implementations of
 * programming language import
 * @author Oliver Kellogg <okellogg@users.sourceforge.net>
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class NativeImportBase : public ClassImport {
public:
    /**
     * Constructor
     * @param singleLineCommentIntro  "//" for IDL and Java, "--" for Ada
     */
    NativeImportBase(QString singleLineCommentIntro);
    virtual ~NativeImportBase();

    /**
     * Implement abstract operation from ClassImport.
     */
    void importFiles(QStringList files);

protected:
    /**
     * Import a single file.
     * To be provided by the programming language specific code import
     * implementation class.
     *
     * @param file  The file to import.
     */
    virtual void parseFile(QString file) = 0;

    /**
     * Scan a single line, advancing `m_srcIndex' and filling `m_source'.
     * To be provided by the programming language specific code import
     * implementation class.
     *
     * @param line  The line to scan.
     */
    virtual void scan(QString line) = 0;

    /**
     * Advance m_srcIndex until m_souce[m_srcIndex] contains the lexeme
     * given by `until'.
     */
    void skipStmt(QString until = ";");

    /**
     * How to start a single line comment in this programming language.
     */
    QString m_singleLineCommentIntro;

    /**
     * The scanned lexemes.
     */
    QStringList m_source;
    /**
     * Used for indexing m_source.
     */
    int m_srcIndex;

    /**
     * Stack of scopes.
     */
    UMLPackage *m_scope[32];
    /**
     * Indexes m_scope. Index 0 is reserved for global scope.
     */
    int m_scopeIndex;

    /**
     * The class currently being processed.
     */
    UMLClassifier *m_klass;
    /**
     * The current access (public/protected/private)
     */
    Uml::Scope m_currentAccess;
    /**
     * Intermediate accumulator for comment text.
     */
    QString m_comment;

};

#endif

