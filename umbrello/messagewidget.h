/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "umlwidget.h"

// forward declarations
class FloatingText;
class ObjectWidget;
/**
 *	Used to display a message on a sequence diagram.  The message
 *	could be between two objects or a message that calls itself on
 *	an object.  This class will only display the line that is
 *	required and the text will be setup by the @ref FloatingText
 *	widget that is passed in the constructor.  A message can be
 *	synchronous (calls a method and gains control back on return,
 *	as happens in most programming languages) or asynchronous
 *	(calls a method and gains back control immediately).
 *
 *	@short	Displays a message.
 *	@author Paul Hensgen
 *	@see	UMLWidget
 *	@see	FloatingText
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class MessageWidget : public UMLWidget {
	Q_OBJECT
public:

	/**
	 *	Constructs a MessageWidget.
	 *
	 *	@param	view	The parent to this class.
	 *	@param	a	The role A widget for this message.
	 *	@param	b	The role B	widget for this message.
	 *	@param	ft	The FloatingText widget that is needed to display text.
	 *	@param	id	A unique id used for deleting this object cleanly.
	 *	@param	y	The vertical position to display this message.
	 *	@param sequenceMessageType Whether synchronous or asynchronous
	 */
	MessageWidget(UMLView * view, UMLWidget* a, UMLWidget* b, FloatingText* ft, 
		      int id, int y, Sequence_Message_Type sequenceMessageType);

	/**
	 *	Constructs a MessageWidget.
	 *
	 *	@param	view		The parent to this class.
	 */
	MessageWidget(UMLView * view, Sequence_Message_Type sequenceMessageType);

	/**
	 *	Initializes key variables of the class.
	 */
	void init();

	/**
	 *	Standard deconstructor.
	 */
	virtual ~MessageWidget();

	/**
	 * Write property of int m_nWidgetAID.
	 */
	void setWidgetAID( int widgetAID ) {
		m_nWidgetAID = widgetAID;
	}

	/**
	 * Read property of int m_nWidgetAID.
	 */
	int getWidgetAID() const {
		return m_nWidgetAID;
	}

	/**
	 * Write property of int m_nWidgetBID.
	 */
	void setWidgetBID( int widgetBID ) {
		m_nWidgetBID = widgetBID;
	}

	/**
	 * Read property of int m_nWidgetBID.
	 */
	int getWidgetBID() const {
		return m_nWidgetBID;
	}

	/**
	 * Write property of QString m_SequenceNumber.
	 */
	void setSequenceNumber( QString sequenceNumber ) {
		m_SequenceNumber = sequenceNumber;
	}

	/**
	 * Read property of QString m_SequenceNumber.
	 */
	QString getSequenceNumber() const {
		return m_SequenceNumber;
	}

	/**
	 * Write property of QString m_Operation.
	 */
	void setOperation( QString operation ) {
		m_Operation = operation;
	}

	/**
	 * Read property of QString m_Operation.
	 */
	QString getOperation() const {
		return m_Operation;
	}

	/**
	 * Write property of int m_nTextID.
	 */
	void setTextID( int textID ) {
		m_nTextID = textID;
	}

	/**
	 * Read property of int m_nTextID.
	 */
	int getTextID() const {
		return m_nTextID;
	}

	/**
	 * Returns whether the message is synchronous or asynchronous
	 */
	Sequence_Message_Type getSequenceMessageType() const {
		return m_sequenceMessageType;
	}

	/**
	 *	Check to see if the given UMLWidget is involved in the message.
	 *
	 *	@param	w	The UMLWidget to check for.
	 *	@return	The status of the check.	true - if is contained, false - not contained.
	 */
	bool contains(UMLWidget * w);

	/**
	 *	Returns the A widget it is related to.
	 *
	 *	@return Returns the A widget it is related to.
	 */
	UMLWidget* getWidgetA();

	/**
	 *	Returns the B widget it is related to.
	 *
	 *	@return Returns the B widget it is related to.
	 */
        UMLWidget* getWidgetB();

	/**
	 *	Returns the id of the A widget it is related to.
	 *
	 *	@return Returns the id of the A widget it is related to..
	 */
	int getCopyIDA() const {
		return m_nWidgetAID;
	}

	/**
	 *	Returns the id of the B widget it is related to.
	 *
	 *	@return Returns the id of the B widget it is related to..
	 */
	int getCopyIDB() const {
		return m_nWidgetBID;
	}

	/**
	 *	Sets the A widget it is related to.
	 *
	 *	@param wa the A widget it is related to.
	 */
	void setWidgetA(UMLWidget * wa) ;

	/**
	 *	Sets the B widget it is related to.
	 *
	 *	@param wa the B widget it is related to.
	 */
	void setWidgetB(UMLWidget * wb);

	/**
	 *	Returns the text widget it is related to.
	 *
	 *	@return Returns the text widget it is related to.
	 */
	FloatingText * getFloatingText() {
		return m_pFText;
	}

	/**
	 *	Sets the text widget it is related to.
	 *
	 *	@param f	The text widget it is related to.
	 */
	void setFloatingText(FloatingText * f) {
		m_pFText = f;
	}

	/**
	 *	Calculate the geometry of the widget.
	 */
	void calculateWidget();

	/**
	 * Activates a MessageWidget, Connects its m_pWA and m_pWB pointers to UMLObjects
	 * and also send signals about its floatingText
	 */
	bool activate(IDChangeLog * Log = 0);

	/**
	 * Synchronizes the WidgetData members with the properties of the corresponding
	 * UMLObject, for example:
	 * the X and Y position of the widget, etc
	 */
	void synchronizeData();

	/**
	 * Calculates the size of the widget by calling
	 * calculateDimenstionsSynchronous() or
	 * calculateDimenstionsAsynchronous()
	 */
	void calculateDimensions();

	/**
	 * Calculates and sets the size of the widget for a synchronous message
	 */
	void calculateDimensionsSynchronous();

	/**
	 * Calculates and sets the size of the widget for an asynchronous message
	 */
	void calculateDimensionsAsynchronous();

	/** 
	 * calls drawSynchronous() or drawAsynchronous()
	 */
	void draw(QPainter& p, int offsetX, int offsetY);

	/**
	 * Draws the calling arrow with filled in arrowhead, the
	 * timeline box and the returning arrow with a dashed line and
	 * stick arrowhead
	 */
	void drawSynchronous(QPainter& p, int offsetX, int offsetY);

	/**
	 * Draws a solid arrow line and a stick arrow head
	 */
	void drawAsynchronous(QPainter& p, int offsetX, int offsetY);


	/**
	 *   Used to cleanup any other widget it may need to delete.
	 */
	void cleanup();

	/**
	 *	Overrides the standard operation.
	 */
	void mouseMoveEvent(QMouseEvent *me);

	/**
	 *	Sets the state of whether the widget is selected.
	 *
	 *	@param _select The state of whether the widget is selected.
	 */
	void setSelected(bool _select);

	/**
	 *		Returns the minimum height this widget should be set at on
	 *		a sequence diagrams.  Takes into account the widget positions
	 *		it is related to.
	 */
	int getMinHeight();

	/**
	 *		Returns the maximum height this widget should be set at on
	 *		a sequence diagrams.  Takes into account the widget positions
	 *		it is related to.
	 */
	int getMaxHeight();

	/**
	 *	Overrides the standard operation.
	 */
	virtual void mouseReleaseEvent(QMouseEvent * me);

	/**
	 *	Overrides the standard operation.
	 */
	virtual void mousePressEvent(QMouseEvent *me);

	/**
	 * saves properties for the supplied <UML:MessageWidget> tag
	 */
	bool saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

	/**
	 * loads properties from a supplied <UML:MessageWidget> tag
	 */
	bool loadFromXMI( QDomElement & qElement );

protected:
	// Data loaded/saved
	int m_nWidgetAID;
	int m_nWidgetBID;
	QString m_SequenceNumber;
	QString m_Operation;
	int m_nTextID;
	/**
	 *	Whether the message is synchronous or asynchronous
	 */
        Sequence_Message_Type m_sequenceMessageType;

private:
	void moveEvent(QMoveEvent */*m*/);
	void resizeEvent(QResizeEvent */*re*/);
	void mouseDoubleClickEvent(QMouseEvent */* me*/);

	UMLWidget * m_pWA, * m_pWB;
	FloatingText * m_pFText;
	int m_nY;
public slots:
	void slotWidgetMoved(int id);
	void slotMenuSelection(int sel);
signals:
	/**
	 * emitted when the message widget is moved up or down
	 * slots into ObjectWidget::slotMessageMoved()
	 */
	void sigMessageMoved();
};

#endif
