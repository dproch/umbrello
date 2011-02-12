/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2011                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "umllistviewitem.h"

// app includes
#include "debug_utils.h"
#include "folder.h"
#include "classifier.h"
#include "entity.h"
#include "template.h"
#include "attribute.h"
#include "operation.h"
#include "entityconstraint.h"
#include "umldoc.h"
#include "umllistview.h"
#include "umlobjectlist.h"
#include "umlview.h"
#include "model_utils.h"
#include "uniqueid.h"
#include "uml.h"
#include "cmds.h"
#include "umlscene.h"

// kde includes
#include <klocale.h>
#include <kmessagebox.h>

// qt includes
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtGui/QDrag>

// system includes
#include <cstdlib>

UMLListView* UMLListViewItem::s_pListView = 0;

/**
 * Sets up an instance.
 *
 * @param parent   The parent to this instance.
 * @param name     The name of this instance.
 * @param t        The type of this instance.
 * @param o        The object it represents.
 */
UMLListViewItem::UMLListViewItem(UMLListView * parent, const QString &name,
                                 ListViewType t, UMLObject* o)
  : QTreeWidgetItem(parent)
{
    init(parent);
    m_type = t;
    m_object = o;
    if (o) {
        m_id = o->id();
    }
    setIcon(Icon_Utils::it_Home);
    setText(0, name);
}

/**
 * Sets up an instance for subsequent loadFromXMI().
 *
 * @param parent   The parent to this instance.
 */
UMLListViewItem::UMLListViewItem(UMLListView * parent)
  : QTreeWidgetItem(parent)
{
    init(parent);
    if (parent == 0)
        uDebug() << "UMLListViewItem constructor called with a 0 listview parent";
}

/**
 * Sets up an instance for subsequent loadFromXMI().
 *
 * @param parent   The parent to this instance.
 */
UMLListViewItem::UMLListViewItem(UMLListViewItem * parent)
  : QTreeWidgetItem(parent)
{
    init();
}

/**
 * Sets up an instance.
 *
 * @param parent   The parent to this instance.
 * @param name     The name of this instance.
 * @param t        The type of this instance.
 * @param o        The object it represents.
 */
UMLListViewItem::UMLListViewItem(UMLListViewItem * parent, const QString &name, ListViewType t, UMLObject*o)
  : QTreeWidgetItem(parent)
{
    init();
    m_type = t;
    m_object = o;
    if (!o) {
        m_id = Uml::id_None;
        updateFolder();
    } else {
        UMLClassifierListItem *umlchild = dynamic_cast<UMLClassifierListItem*>(o);
        if (umlchild)
            parent->addClassifierListItem(umlchild, this);
        updateObject();
        m_id = o->id();
    }
    setText(0, name);
    if (!Model_Utils::typeIsRootView(t)) {
        setFlags(flags() | Qt::ItemIsEditable);
    }
}

/**
 * Sets up an instance.
 *
 * @param parent   The parent to this instance.
 * @param name     The name of this instance.
 * @param t        The type of this instance.
 * @param id       The id of this instance.
 */
UMLListViewItem::UMLListViewItem(UMLListViewItem * parent, const QString &name, ListViewType t, Uml::IDType id)
  : QTreeWidgetItem(parent)
{
    init();
    m_type = t;
    m_id = id;
    switch (m_type) {
    case lvt_Collaboration_Diagram:
        setIcon(Icon_Utils::it_Diagram_Collaboration);
        break;
    case lvt_Class_Diagram:
        setIcon(Icon_Utils::it_Diagram_Class);
        break;
    case lvt_State_Diagram:
        setIcon(Icon_Utils::it_Diagram_State);
        break;
    case lvt_Activity_Diagram:
        setIcon(Icon_Utils::it_Diagram_Activity);
        break;
    case lvt_Sequence_Diagram:
        setIcon(Icon_Utils::it_Diagram_Sequence);
        break;
    case lvt_Component_Diagram:
        setIcon(Icon_Utils::it_Diagram_Component);
        break;
    case lvt_Deployment_Diagram:
        setIcon(Icon_Utils::it_Diagram_Deployment);
        break;
    case lvt_UseCase_Diagram:
        setIcon(Icon_Utils::it_Diagram_Usecase);
        break;
    default:
        setIcon(Icon_Utils::it_Diagram);
    }
    //  Constructor also used by folder so just make sure we don't need to
    //  to set pixmap to folder.  doesn't hurt diagrams.
    updateFolder();
    setText(0, name);
    setFlags(flags() | Qt::ItemIsEditable);
}

/**
 * Standard destructor.
 */
UMLListViewItem::~UMLListViewItem()
{
}

/**
 * Initializes key variables of the class.
 */
void UMLListViewItem::init(UMLListView * parent)
{
    m_type = lvt_Unknown;
    m_bCreating = false;
    m_object = 0;
    m_id = Uml::id_None;
    if (s_pListView == 0 && parent != 0) {
        uDebug() << "s_pListView still 0, setting it now ";
        s_pListView = parent;
    }
}

/**
 * Returns the signature of items that are operations.
 * @return signature of an operation item, else an empty string
 */
QString UMLListViewItem::toolTip()
{
    UMLObject *obj = umlObject();
    if (obj && obj->baseType() == UMLObject::ot_Operation) {
        UMLOperation *op = static_cast<UMLOperation*>(obj);
        return op->toString(Uml::SignatureType::ShowSig);
    }
    else {
        return QString();
    }
}

/**
 * Returns the type this instance represents.
 *
 * @return  The type this instance represents.
 */
UMLListViewItem::ListViewType UMLListViewItem::type() const
{
    return m_type;
}

/**
 * Adds the child listview item representing the given UMLClassifierListItem.
 */
void UMLListViewItem::addClassifierListItem(UMLClassifierListItem *child, UMLListViewItem *childItem)
{
    m_comap[child] = childItem;
}

/**
 * Deletes the child listview item representing the given UMLClassifierListItem.
 */
void UMLListViewItem::deleteChildItem(UMLClassifierListItem *child)
{
    UMLListViewItem *childItem = findChildObject(child);
    if (childItem == 0) {
        uError() << child->name() << ": child listview item not found";
        return;
    }
    m_comap.remove(child);
    delete childItem;
}

void UMLListViewItem::setVisible(bool state)
{
    setHidden(!state);
}

/**
 * Returns the id this class represents.
 *
 * @return  The id this class represents.
 */
Uml::IDType UMLListViewItem::getID() const
{
    if (m_object) {
        uDebug() << "name=" << m_object->name();  //:TODO:
        return m_object->id();
    }
    return m_id;
}

/**
 * Sets the id this class represents.
 * This only sets the ID locally, not at the UMLObject that is perhaps
 * associated to this UMLListViewItem.
 * @param id   the id this class represents
 */
void UMLListViewItem::setID(Uml::IDType id)
{
    if (m_object) {
        Uml::IDType oid = m_object->id();
        if (id != Uml::id_None && oid != id) {
            uDebug() << "new id " << ID2STR(id) << " does not agree with object id "
                << ID2STR(oid);
	}
    }
    m_id = id;
}

/**
 * Set the UMLObject associated with this instance.
 *
 * @param obj  The object this class represents.
 */
void UMLListViewItem::setUMLObject(UMLObject * obj)
{
    m_object = obj;
}

/**
 * Return the UMLObject associated with this instance.
 *
 * @return  The object this class represents.
 */
UMLObject * UMLListViewItem::umlObject() const
{
    return m_object;
}

/**
 * Returns true if the UMLListViewItem of the given ID is a parent of
 * this UMLListViewItem.
 */
bool UMLListViewItem::isOwnParent(Uml::IDType listViewItemID)
{
    QTreeWidgetItem *lvi = (QTreeWidgetItem*)s_pListView->findItem(listViewItemID);
    if (lvi == 0) {
        uError() << "ListView->findItem(" << ID2STR(listViewItemID) << ") returns 0";
        return true;
    }
    for (QTreeWidgetItem *self = (QTreeWidgetItem*)this; self; self = self->parent()) {
        if (lvi == self)
            return true;
    }
    return false;
}

/**
 * Updates the representation of the object.
 */
void UMLListViewItem::updateObject()
{
    if (m_object == 0)
        return;

    Uml::Visibility scope = m_object->visibility();
    UMLObject::Object_Type ot = m_object->baseType();
    QString modelObjText = m_object->name();
    if (Model_Utils::isClassifierListitem(ot)) {
        UMLClassifierListItem *pNarrowed = static_cast<UMLClassifierListItem*>(m_object);
        modelObjText = pNarrowed->toString(Uml::SignatureType::SigNoVis);
    }
    setText(0, modelObjText);

    Icon_Utils::Icon_Type icon = Icon_Utils::it_Home;
    switch (ot) {
    case UMLObject::ot_Package:
        if (m_object->stereotype() == "subsystem")
            icon = Icon_Utils::it_Subsystem;
        else
            icon = Icon_Utils::it_Package;
        break;
        /*
            case UMLObject::ot_Folder:
                {
                    ListViewType lvt = Model_Utils::convert_OT_LVT(m_object);
                    icon = Model_Utils::convert_LVT_IT(lvt);
                }
                break;
         */
    case UMLObject::ot_Operation:
        if (scope == Uml::Visibility::Public)
            icon = Icon_Utils::it_Public_Method;
        else if (scope == Uml::Visibility::Private)
            icon = Icon_Utils::it_Private_Method;
        else if (scope == Uml::Visibility::Implementation)
            icon = Icon_Utils::it_Private_Method;
        else
            icon = Icon_Utils::it_Protected_Method;
        break;

    case UMLObject::ot_Attribute:
    case UMLObject::ot_EntityAttribute:
        if (scope == Uml::Visibility::Public)
            icon = Icon_Utils::it_Public_Attribute;
        else if (scope == Uml::Visibility::Private)
            icon = Icon_Utils::it_Private_Attribute;
        else if (scope == Uml::Visibility::Implementation)
            icon = Icon_Utils::it_Private_Attribute;
        else
            icon = Icon_Utils::it_Protected_Attribute;
        break;
    case UMLObject::ot_UniqueConstraint:
        m_type = Model_Utils::convert_OT_LVT(umlObject());
        icon = Model_Utils::convert_LVT_IT(m_type);
        break;

    default:
        icon = Model_Utils::convert_LVT_IT(m_type);
        break;
    }//end switch
    if (icon)
        setIcon(icon);
}

/**
 * Updates the icon on a folder.
 */
void UMLListViewItem::updateFolder()
{
    Icon_Utils::Icon_Type icon = Model_Utils::convert_LVT_IT(m_type);
    if (icon) {
        if (Model_Utils::typeIsFolder(m_type))
            icon = (Icon_Utils::Icon_Type)((int)icon + (int)isExpanded());
        setIcon(icon);
    }
}

/**
 * Overrides default method.
 * Will call default method but also makes sure correct icon is shown.
 */
void UMLListViewItem::setOpen(bool expand)
{
    setExpanded(expand);
    updateFolder();
}

/**
 * Sets if the item is in the middle of being created.
 */
void UMLListViewItem::setCreating( bool creating )
{
    m_bCreating = creating;
}

/**
 * Set the pixmap corresponding to the given Icon_Type.
 */
void UMLListViewItem::setIcon(Icon_Utils::Icon_Type iconType)
{
    QPixmap p = Icon_Utils::SmallIcon(iconType);
    QTreeWidgetItem::setIcon(0, QIcon(p));
}

void UMLListViewItem::startRename(int col)
{
    uDebug() << "column=" << col << ", text=" << text(col);
    m_oldText = text(col);  // keep the old text
    if (m_bCreating) {
        s_pListView->cancelRename(this);
    }
}

/**
 * This function is called if the user presses Enter during in-place renaming
 * of the item in column col.
 */
void UMLListViewItem::okRename(int col)
{
    uDebug() << "column=" << col << ", text=" << text(col);
    UMLDoc* doc = s_pListView->document();
    if (m_bCreating) {
        m_bCreating = false;
        if (s_pListView->itemRenamed(this, col)) {
//:TODO:            s_pListView->ensureItemVisible(this);
            doc->setModified(true);
        } else {
            delete this;
        }
        return;
    }
    QString newText = text(col);
    if (newText == m_oldText) {
        return;
    }
    if (newText.isEmpty()) {
        cancelRenameWithMsg();
        return;
    }
    switch (m_type) {
    case lvt_UseCase:
    case lvt_Actor:
    case lvt_Class:
    case lvt_Package:
    case lvt_UseCase_Folder:
    case lvt_Logical_Folder:
    case lvt_Component_Folder:
    case lvt_Deployment_Folder:
    case lvt_EntityRelationship_Folder:
    case lvt_Interface:
    case lvt_Datatype:
    case lvt_Enum:
    case lvt_EnumLiteral:
    case lvt_Subsystem:
    case lvt_Component:
    case lvt_Node:
    case lvt_Category:
        if (m_object == 0 || !doc->isUnique(newText)) {
            cancelRenameWithMsg();
            return;
        }
        UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(m_object, newText));
        doc->setModified(true);
        break;

    case lvt_Operation: {
        if (m_object == 0) {
            cancelRenameWithMsg();
            return;
        }
        UMLOperation *op = static_cast<UMLOperation*>(m_object);
        UMLClassifier *parent = static_cast<UMLClassifier *>(op->parent());
        Model_Utils::OpDescriptor od;
        Model_Utils::Parse_Status st = Model_Utils::parseOperation(newText, od, parent);
        if (st == Model_Utils::PS_OK) {
            // TODO: Check that no operation with the exact same profile exists.
            UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(op, od.m_name));
            op->setType(od.m_pReturnType);
            UMLAttributeList parmList = op->getParmList();
            const int newParmListCount = parmList.count();
            if (newParmListCount > od.m_args.count()) {
                // Remove parameters at end of of list that no longer exist.
                for (int i = od.m_args.count(); i < newParmListCount; i++) {
                    UMLAttribute *a = parmList.at(i);
                    op->removeParm(a, false);
                }
            }
            Model_Utils::NameAndType_ListIt lit = od.m_args.begin();
            for (int i = 0; lit != od.m_args.end(); ++lit, ++i) {
                const Model_Utils::NameAndType& nm_tp = *lit;
                UMLAttribute *a;
                if (i < newParmListCount) {
                    a = parmList.at(i);
                } else {
                    a = new UMLAttribute(op);
                    a->setID(UniqueID::gen());
                }
                UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(a, nm_tp.m_name));
                a->setType(nm_tp.m_type);
                a->setParmKind(nm_tp.m_direction);
                a->setInitialValue(nm_tp.m_initialValue);
                if (i >= newParmListCount) {
                    op->addParm(a);
                }
            }
            newText = op->toString(Uml::SignatureType::SigNoVis);
        } else {
            KMessageBox::error(0,
                               Model_Utils::psText(st),
                               i18n("Rename canceled"));
        }
        setText(0, m_oldText);
        break;
    }

    case lvt_Attribute:
    case lvt_EntityAttribute: {
        if (m_object == 0) {
            cancelRenameWithMsg();
            return;
        }
        UMLClassifier *parent = static_cast<UMLClassifier*>(m_object->parent());
        Model_Utils::NameAndType nt;
        Uml::Visibility vis;
        Model_Utils::Parse_Status st;
        st = Model_Utils::parseAttribute(newText, nt, parent, &vis);
        if (st == Model_Utils::PS_OK) {
            UMLObject *exists = parent->findChildObject(newText);
            if (exists) {
                cancelRenameWithMsg();
                return;
            }
            UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(m_object, nt.m_name));
            UMLAttribute *pAtt = static_cast<UMLAttribute*>(m_object);
            pAtt->setType(nt.m_type);
            pAtt->setVisibility(vis);
            pAtt->setParmKind(nt.m_direction);
            pAtt->setInitialValue(nt.m_initialValue);
            newText = pAtt->toString(Uml::SignatureType::SigNoVis);
        } else {
            KMessageBox::error(0,
                               Model_Utils::psText(st),
                               i18n("Rename canceled"));
        }
        setText(0, newText);
        break;
    }

    case lvt_PrimaryKeyConstraint:
    case lvt_UniqueConstraint:
    case lvt_ForeignKeyConstraint:
    case lvt_CheckConstraint: {
        if (m_object == 0) {
            cancelRenameWithMsg();
            return;
        }
        UMLEntity *parent = static_cast<UMLEntity*>(m_object->parent());
        QString name;
        Model_Utils::Parse_Status st;
        st = Model_Utils::parseConstraint(newText, name,  parent);
        if (st == Model_Utils::PS_OK) {
            UMLObject *exists = parent->findChildObject(name);
            if (exists) {
                cancelRenameWithMsg();
                return;
            }
            UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(m_object, name));

            UMLEntityConstraint* uec = static_cast<UMLEntityConstraint*>(m_object);
            newText = uec->toString(Uml::SignatureType::SigNoVis);
        } else {
            KMessageBox::error(0,
                               Model_Utils::psText(st),
                               i18n("Rename canceled"));
        }
        setText(0, newText);
        break;
    }

    case lvt_Template: {
        if (m_object == 0) {
            cancelRenameWithMsg();
            return;
        }
        UMLClassifier *parent = static_cast<UMLClassifier*>(m_object->parent());
        Model_Utils::NameAndType nt;
        Model_Utils::Parse_Status st = Model_Utils::parseTemplate(newText, nt, parent);
        if (st == Model_Utils::PS_OK) {
            UMLObject *exists = parent->findChildObject(newText);
            if (exists) {
                cancelRenameWithMsg();
                return;
            }
            UMLApp::app()->executeCommand(new Uml::CmdRenameUMLObject(m_object, nt.m_name));
            UMLTemplate *tmpl = static_cast<UMLTemplate*>(m_object);
            tmpl->setType(nt.m_type);
            newText = tmpl->toString(Uml::SignatureType::SigNoVis);
        } else {
            KMessageBox::error(0,
                               Model_Utils::psText(st),
                               i18n("Rename canceled"));
        }
        setText(0, newText);
        break;
    }

    case lvt_UseCase_Diagram:
    case lvt_Class_Diagram:
    case lvt_Sequence_Diagram:
    case lvt_Collaboration_Diagram:
    case lvt_State_Diagram:
    case lvt_Activity_Diagram:
    case lvt_Component_Diagram:
    case lvt_Deployment_Diagram: {
        UMLView *view = doc->findView(getID());
        if (view == 0) {
            cancelRenameWithMsg();
            return;
        }
        UMLView *anotherView = doc->findView(view->umlScene()->type(), newText);
        if (anotherView && anotherView->umlScene()->getID() == getID()) {
            anotherView = 0;
        }
        if (anotherView) {
            cancelRenameWithMsg();
            return;
        }
        view->umlScene()->setName(newText);
        setText(0, newText);
        doc->signalDiagramRenamed(view);
        break;
    }
    default:
        KMessageBox::error(0,
                           i18n("Renaming an item of listview type %1 is not yet implemented.", m_type),
                           i18n("Function Not Implemented"));
        setText(0, m_oldText);
        break;
    }
    doc->setModified(true);
}

/**
 * Auxiliary method for okRename().
 */
void UMLListViewItem::cancelRenameWithMsg()
{
    uDebug() << "column=" << ":TODO:col" << ", text=" << text(0);
    KMessageBox::error(0,
                       i18n("The name you entered was invalid.\nRenaming process has been canceled."),
                       i18n("Name Not Valid"));
    setText(0, m_oldText);
}

/**
 * Overrides default method to make public.
 */
void UMLListViewItem::cancelRename(int col)
{
    uDebug() << "column=" << col << ", text=" << text(col);
    Q_UNUSED(col);
    if (m_bCreating) {
        s_pListView->cancelRename(this);
    }
}

/**
 * Overrides the default sorting to sort by item type.
 * Sort the listview items by type and position within the corresponding list
 * of UMLObjects. If the item does not have an UMLObject then place it last.
 */
#if 0
int UMLListViewItem::compare(QTreeWidgetItem *other, int col, bool ascending) const
{
    UMLListViewItem *ulvi = static_cast<UMLListViewItem*>(other);
    ListViewType ourType = type();
    ListViewType otherType = ulvi->type();

    if (ourType < otherType)
        return -1;
    if (ourType > otherType)
        return 1;
    // ourType == otherType
    const bool subItem = Model_Utils::typeIsClassifierList(ourType);
    const int alphaOrder = key(col, ascending).compare(other->key(col, ascending));
    int retval = 0;
    QString dbgPfx = "compare(type=" + QString::number((int)ourType)
                     + ", self=" + text() + ", other=" + ulvi->text()
                     + "): return ";
    UMLObject *otherObj = ulvi->umlObject();
    if (m_object == 0) {
        retval = (subItem ? 1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (m_object==0)";
#endif
        return retval;
    }
    if (otherObj == 0) {
        retval = (subItem ? -1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (otherObj==0)";
#endif
        return retval;
    }
    UMLClassifier *ourParent = dynamic_cast<UMLClassifier*>(m_object->parent());
    UMLClassifier *otherParent = dynamic_cast<UMLClassifier*>(otherObj->parent());
    if (ourParent == 0) {
        retval = (subItem ? 1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (ourParent==0)";
#endif
        return retval;
    }
    if (otherParent == 0) {
        retval = (subItem ? -1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (otherParent==0)";
#endif
        return retval;
    }
    if (ourParent != otherParent) {
        retval = (subItem ? 0 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (ourParent != otherParent)";
#endif
        return retval;
    }
    UMLClassifierListItem *thisUmlItem = dynamic_cast<UMLClassifierListItem*>(m_object);
    UMLClassifierListItem *otherUmlItem = dynamic_cast<UMLClassifierListItem*>(otherObj);
    if (thisUmlItem == 0) {
        retval = (subItem ? 1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (thisUmlItem==0)";
#endif
        return retval;
    }
    if (otherUmlItem == 0) {
        retval = (subItem ? -1 : alphaOrder);
#ifdef DEBUG_LVITEM_INSERTION_ORDER
        uDebug() << dbgPfx << retval << " because (otherUmlItem==0)";
#endif
        return retval;
    }
    UMLClassifierListItemList items = ourParent->getFilteredList(thisUmlItem->getBaseType());
    int myIndex = items.indexOf(thisUmlItem);
    int otherIndex = items.indexOf(otherUmlItem);
    if (myIndex < 0) {
        retval = (subItem ? -1 : alphaOrder);
        uError() << dbgPfx << retval << " because (myIndex < 0)";
        return retval;
    }
    if (otherIndex < 0) {
        retval = (subItem ? 1 : alphaOrder);
        uError() << dbgPfx << retval << " because (otherIndex < 0)";
        return retval;
    }
    return (myIndex < otherIndex ? -1 : myIndex > otherIndex ? 1 : 0);
}
#endif

/**
 * Create a deep copy of this UMLListViewItem, but using the
 * given parent instead of the parent of this UMLListViewItem.
 * Return the new UMLListViewItem created.
 */
UMLListViewItem* UMLListViewItem::deepCopy(UMLListViewItem *newParent)
{
    QString nm = text(0);
    ListViewType t = type();
    UMLObject *o = umlObject();
    UMLListViewItem* newItem;
    if (o)
        newItem = new UMLListViewItem(newParent, nm, t, o);
    else
        newItem = new UMLListViewItem(newParent, nm, t, m_id);
    for (int i=0; i < childCount(); i++) {
        UMLListViewItem *childItem = static_cast<UMLListViewItem*>(child(i));
        childItem->deepCopy(newItem);
    }
    return newItem;
}

/**
 * Find the UMLListViewItem that is related to the given UMLObject
 * in the tree rooted at the current UMLListViewItem.
 * Return a pointer to the item or 0 if not found.
 */
UMLListViewItem* UMLListViewItem::findUMLObject(const UMLObject *o)
{
    if (m_object == o)
        return this;
    for (int i = 0; i < childCount(); i++) {
        UMLListViewItem *item = static_cast<UMLListViewItem*>(child(i));
        UMLListViewItem *testItem = item->findUMLObject(o);
        if (testItem)
            return testItem;
    }
    return 0;
}

/**
 * Find the UMLListViewItem that represents the given UMLClassifierListItem
 * in the children of the current UMLListViewItem.  (Only makes sense if
 * the current UMLListViewItem represents a UMLClassifier.)
 * Return a pointer to the item or 0 if not found.
 */
UMLListViewItem* UMLListViewItem::findChildObject(UMLClassifierListItem *cli)
{
    ChildObjectMap::iterator it = m_comap.find(cli);
    if (it != m_comap.end()) {
        return *it;
    }
    return 0;
}

/**
 * Find the UMLListViewItem of the given ID in the tree rooted at
 * the current UMLListViewItem.
 * Return a pointer to the item or 0 if not found.
 *
 * @param id   The ID to search for.
 * @return The item with the given ID or 0 if not found.
 */
UMLListViewItem * UMLListViewItem::findItem(Uml::IDType id)
{
    if (getID() == id) {
        return this;
    }
    for (int i = 0; i < childCount(); ++i) {
        UMLListViewItem *childItem = static_cast<UMLListViewItem*>(child(i));
        UMLListViewItem *inner = childItem->findItem(id);
        if (inner) {
            return inner;
	}
    }
    return 0;
}

/**
 * Saves the listview item to a "listitem" tag.
 */
void UMLListViewItem::saveToXMI(QDomDocument & qDoc, QDomElement & qElement)
{
    QDomElement itemElement = qDoc.createElement("listitem");
    Uml::IDType id = getID();
    QString idStr = ID2STR(id);
    //uDebug() << "id = " << idStr << ", type = " << m_type;
    if (id != Uml::id_None)
        itemElement.setAttribute("id", idStr);
    itemElement.setAttribute("type", m_type);
    UMLFolder *extFolder = 0;
    if (m_object == 0) {
        if (! Model_Utils::typeIsDiagram(m_type) && m_type != lvt_View)
            uError() << text(0) << ": m_object is 0";
        if (m_type != lvt_View)
            itemElement.setAttribute("label", text(0));
    } else if (m_object->id() == Uml::id_None) {
        if (text(0).isEmpty()) {
            uDebug() << "Skipping empty item";
            return;
        }
        uDebug() << "saving local label " << text(0) << " because umlobject ID is not set";
        if (m_type != lvt_View)
            itemElement.setAttribute("label", text(0));
    } else if (m_object->baseType() == UMLObject::ot_Folder) {
        extFolder = static_cast<UMLFolder*>(m_object);
        if (!extFolder->folderFile().isEmpty()) {
            itemElement.setAttribute("open", "0");
            qElement.appendChild(itemElement);
            return;
        }
    }
    itemElement.setAttribute("open", isExpanded());
    QDomElement folderRoot;
    for (int i=0; i < childCount(); i++) {
        UMLListViewItem *childItem = static_cast<UMLListViewItem*>(child(i));
        childItem->saveToXMI(qDoc, itemElement);
    }
    qElement.appendChild(itemElement);
}

/**
 * Loads a "listitem" tag, this is only used by the clipboard currently.
 */
bool UMLListViewItem::loadFromXMI(QDomElement& qElement)
{
    QString id = qElement.attribute("id", "-1");
    QString type = qElement.attribute("type", "-1");
    QString label = qElement.attribute("label", "");
    QString open = qElement.attribute("open", "1");
    if (!label.isEmpty())
        setText(0, label);
    else if (id == "-1") {
        uError() << "Item of type " << type << " has neither ID nor label";
        return false;
    }

    m_id = STR2ID(id);
    if (m_id != Uml::id_None)
        m_object = s_pListView->document()->findObjectById(m_id);
    m_type = (ListViewType)(type.toInt());
    if (m_object)
        updateObject();
    setOpen((bool)open.toInt());
    return true;
}

UMLListViewItem* UMLListViewItem::childItem(int i)
{
    return static_cast<UMLListViewItem *>(child(i));
}
