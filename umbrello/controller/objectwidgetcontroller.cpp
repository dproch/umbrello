/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2013                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "objectwidgetcontroller.h"

// app includes
#include "objectwidget.h"
#include "seqlinewidget.h"
#include "uml.h"
#include "umlscene.h"
#include "umlwidget.h"
#include "umlwidgetlist.h"

// kde includes
#include <kcursor.h>
#include <klocale.h>

/**
 * Constructor for ObjectWidgetController.
 *
 * @param objectWidget The object widget which uses the controller.
 */
ObjectWidgetController::ObjectWidgetController(ObjectWidget* objectWidget)
  : UMLWidgetController(objectWidget)
{
}

/**
 * Destructor for ObjectWidgetController.
 */
ObjectWidgetController::~ObjectWidgetController()
{
}

/**
 * Overridden from UMLWidgetController.
 * Returns the cursor to be shown when resizing the widget.
 * The cursor shown is KCursor::sizeHorCursor().
 *
 * @return The cursor to be shown when resizing the widget.
 */
QCursor ObjectWidgetController::getResizeCursor()
{
    return Qt::SizeHorCursor;
}

/**
 * Overridden from UMLWidgetController.
 * Resizes the width of the object widget.
 * Object widgets can only be resized horizontally, so height isn't modified.
 *
 * @param newW The new width for the widget.
 * @param newH The new height for the widget (isn't used).
 */
void ObjectWidgetController::resizeWidget(UMLSceneValue newW, UMLSceneValue newH)
{
    Q_UNUSED(newH);
    m_widget->setSize(newW, m_widget->height());
}

/**
 * Overrides the standard operation.
 */
void ObjectWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *me)
{
    UMLWidgetController::mousePressEvent(me);
    m_isOnDestructionBox = false;
    SeqLineWidget * pLine = dynamic_cast<ObjectWidget*>(m_widget)->sequentialLine();

    if (pLine->onDestructionBox(me->scenePos())) {
        m_isOnDestructionBox = true;
        qreal oldX = dynamic_cast<ObjectWidget*>(m_widget)->x() + dynamic_cast<ObjectWidget*>(m_widget)->width() / 2;
        qreal oldY = dynamic_cast<ObjectWidget*>(m_widget)->getEndLineY() - 10;
        m_oldPos = QPointF(oldX, oldY);
    }

}

/**
 * Overrides the standard operation.
 */
void ObjectWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent* me)
{
    if (me->button() != Qt::LeftButton) {
        return;
    }

    if (m_inResizeArea) {
        resize(me);
        return;
    }

    int diffY = me->scenePos().y() - m_oldPos.y();

    if (m_isOnDestructionBox) {
        moveDestructionBy (diffY);
    }
    else {
        UMLWidgetController::mouseMoveEvent(me);
    }
}

/**
 * Overridden from UMLWidgetController.
 * Moves the widget to a new position using the difference between the
 * current position and the new position.
 * Y position is ignored, and widget is only moved along X axis.
 *
 * @param diffX The difference between current X position and new X position.
 * @param diffY The difference between current Y position and new Y position
 *                          (isn't used).
 */
void ObjectWidgetController::moveWidgetBy(UMLSceneValue diffX, UMLSceneValue diffY)
{
    Q_UNUSED(diffY);
    m_widget->setX(m_widget->x() + diffX);
}

/**
 * Moves the destruction Box to a new position using the difference between the
 * current position and the new position.
 * The destruction box is only moved along Y axis.
 *
 * @param diffY The difference between current Y position and new Y position
 */
void ObjectWidgetController::moveDestructionBy(UMLSceneValue diffY)
{
    // endLine = length of the life line + diffY - 10 to center on the destruction box
    UMLSceneValue endLine = dynamic_cast<ObjectWidget *>(m_widget)->getEndLineY() + diffY - 10;
    SeqLineWidget * pLine = dynamic_cast<ObjectWidget *>(m_widget)->sequentialLine();
    pLine->setEndOfLine(endLine);
    m_oldPos.setY(endLine);
}

/**
 * Overridden from UMLWidgetController.
 * Modifies the value of the diffX and diffY variables used to move the widgets.
 * All the widgets are constrained to be moved only in X axis (diffY is set to 0).
 *
 * @param diffX The difference between current X position and new X position.
 * @param diffY The difference between current Y position and new Y position.
 */
void ObjectWidgetController::constrainMovementForAllWidgets(UMLSceneValue& diffX, UMLSceneValue& diffY)
{
    Q_UNUSED(diffX);
    diffY = 0;
}
