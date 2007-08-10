/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef ASSOCROLEPAGE_H
#define ASSOCROLEPAGE_H

// qt includes
#include <qwidget.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <q3buttongroup.h>
#include <q3multilineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

#include <klineedit.h>
#include <kcombobox.h>

//my class includes
#include "../umlobject.h"
#include "../objectwidget.h"
#include "../umldoc.h"
#include "../associationwidget.h"

/**
 * Displays properties of a UMLObject in a dialog box.  This is not usually directly
 * called.  The class @ref AssocPropDlg will set this up for you.
 *
 * @short Display properties on a UMLObject.
 * @author Paul Hensgen <phensgen@techie.com>
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class AssocRolePage : public QWidget {
    Q_OBJECT
public:

    /**
     *  Sets up the AssocRolePage.
     *
     *  @param  d       The UMLDoc which controls controls object creation.
     *  @param  parent  The parent to the AssocRolePage.
     *  @param  a       The AssociationWidget to display the properties of.
     */
    AssocRolePage(UMLDoc *d, QWidget *parent, AssociationWidget *a);

    /**
     *  Standard deconstructor.
     */
    ~AssocRolePage();

    /**
     *  Will move information from the dialog into the object.
     *  Call when the ok or apply button is pressed.
     */
    void updateObject();

private:
    KLineEdit *m_pRoleALE, *m_pRoleBLE;
    KComboBox *m_pMultiACB, *m_pMultiBCB;
    Q3MultiLineEdit *m_pDocA, *m_pDocB;
    AssociationWidget *m_pAssociationWidget;
    UMLDoc * m_pUmldoc;
    ObjectWidget *m_pWidget;
    QRadioButton *m_PublicARB, *m_ProtectedARB, *m_PrivateARB, *m_ImplementationARB;
    QRadioButton *m_PublicBRB, *m_ProtectedBRB, *m_PrivateBRB, *m_ImplementationBRB;
    QRadioButton *m_ChangeableARB, *m_AddOnlyARB, *m_FrozenARB;
    QRadioButton *m_ChangeableBRB, *m_AddOnlyBRB, *m_FrozenBRB;

    void constructWidget();

public slots:
    /**
     *  When the draw as actor check box is toggled, the draw
     *  as multi instance need to be enabled/disabled.  They
     *  both can't be available at the same time.
     */
    // void slotActorToggled( bool state );
};

#endif
