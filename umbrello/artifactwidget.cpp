/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "artifactwidget.h"
#include "artifact.h"
#include "umlview.h"
#include <kdebug.h>
#include <qpainter.h>
#include <qpointarray.h>

ArtifactWidget::ArtifactWidget(UMLView * view, UMLObject * o) : UMLWidget(view, o) {
	init();
	setSize(100, 30);
	calculateSize();
	UMLWidget::setBaseType( wt_Artifact );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
ArtifactWidget::ArtifactWidget(UMLView * view) : UMLWidget(view) {
	init();
	setSize(100,30);
	UMLWidget::setBaseType( wt_Artifact );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::init() {
	m_pMenu = 0;
	//maybe loading and this may not be set.
	if (m_pObject) {
		calculateSize();
		update();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
ArtifactWidget::~ArtifactWidget() {}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::drawAsNormal(QPainter& p, int offsetX, int offsetY) {
	p.setPen( UMLWidget::getLineColour() );
	if ( UMLWidget::getUseFillColour() ) {
		p.setBrush( UMLWidget::getFillColour() );
	} else {
		p.setBrush( m_pView->viewport()->backgroundColor() );
	}

	int w = width();
	int h = height();
	QFont font = UMLWidget::getFont();
	font.setBold(true);
	QFontMetrics &fm = getFontMetrics(FT_BOLD);
	int fontHeight  = fm.lineSpacing();
	QString name = getName();
	QString stereotype = m_pObject->getStereotype();

	p.drawRect(offsetX, offsetY, w, h);

	p.setPen( QPen(black) );
	p.setFont(font);

	if (stereotype != "") {
		p.drawText(offsetX + ARTIFACT_MARGIN, offsetY + (h/2) - fontHeight,
			   w, fontHeight, AlignCenter, "<< " + stereotype + " >>");
	}

	int lines;
	if (stereotype != "") {
		lines = 2;
	} else {
		lines = 1;
	}

	if (lines == 1) {
		p.drawText(offsetX, offsetY + (h/2) - (fontHeight/2),
			   w, fontHeight, AlignCenter, name);
	} else {
		p.drawText(offsetX, offsetY + (h/2),
			   w, fontHeight, AlignCenter, name);
	}

	if(m_bSelected) {
		drawSelected(&p, offsetX, offsetY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::drawAsFile(QPainter& p, int offsetX, int offsetY) {
	p.setPen( UMLWidget::getLineColour() );
	if ( UMLWidget::getUseFillColour() ) {
		p.setBrush( UMLWidget::getFillColour() );
	} else {
		p.setBrush( m_pView->viewport()->backgroundColor() );
	}

	int w = width();
	int h = height();
	QFont font = UMLWidget::getFont();
	QFontMetrics &fm = getFontMetrics(FT_NORMAL);
	int fontHeight  = fm.lineSpacing();
	QString name = getName();

	int startX = offsetX + (w/2) - 25;
	int iconHeight = h - fontHeight;
	QPointArray pointArray(5);
	pointArray.setPoint(0, startX, offsetY);
	pointArray.setPoint(1, startX + 40, offsetY);
	pointArray.setPoint(2, startX + 50, offsetY + 10);
	pointArray.setPoint(3, startX + 50, offsetY + iconHeight);
	pointArray.setPoint(4, startX, offsetY + iconHeight);
	p.drawPolygon(pointArray);

	p.drawLine(startX + 40, offsetY, startX + 40, offsetY + 10);
	p.drawLine(startX + 40, offsetY + 10, startX + 50, offsetY + 10);
	p.drawLine(startX + 40, offsetY, startX + 50, offsetY + 10);

	p.setPen( QPen(black) );
	p.setFont(font);

	p.drawText(offsetX, offsetY + h - fontHeight,
		   w, fontHeight, AlignCenter, name);

	if(m_bSelected) {
		drawSelected(&p, offsetX, offsetY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::drawAsLibrary(QPainter& p, int offsetX, int offsetY) {
	//FIXME this should have gears on it
	p.setPen( UMLWidget::getLineColour() );
	if ( UMLWidget::getUseFillColour() ) {
		p.setBrush( UMLWidget::getFillColour() );
	} else {
		p.setBrush( m_pView->viewport()->backgroundColor() );
	}

	int w = width();
	int h = height();
	QFont font = UMLWidget::getFont();
	QFontMetrics &fm = getFontMetrics(FT_NORMAL);
	int fontHeight  = fm.lineSpacing();
	QString name = getName();

	int startX = offsetX + (w/2) - 25;
	int iconHeight = h - fontHeight;
	QPointArray pointArray(5);
	pointArray.setPoint(0, startX, offsetY);
	pointArray.setPoint(1, startX + 40, offsetY);
	pointArray.setPoint(2, startX + 50, offsetY + 10);
	pointArray.setPoint(3, startX + 50, offsetY + iconHeight);
	pointArray.setPoint(4, startX, offsetY + iconHeight);
	p.drawPolygon(pointArray);

	p.drawLine(startX + 40, offsetY, startX + 40, offsetY + 10);
	p.drawLine(startX + 40, offsetY + 10, startX + 50, offsetY + 10);
	p.drawLine(startX + 40, offsetY, startX + 50, offsetY + 10);

	p.setPen( QPen(black) );
	p.setFont(font);

	p.drawText(offsetX, offsetY + h - fontHeight,
		   w, fontHeight, AlignCenter, name);

	if(m_bSelected) {
		drawSelected(&p, offsetX, offsetY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::drawAsTable(QPainter& p, int offsetX, int offsetY) {
	p.setPen( UMLWidget::getLineColour() );
	if ( UMLWidget::getUseFillColour() ) {
		p.setBrush( UMLWidget::getFillColour() );
	} else {
		p.setBrush( m_pView->viewport()->backgroundColor() );
	}

	int w = width();
	int h = height();
	QFont font = UMLWidget::getFont();
	QFontMetrics &fm = getFontMetrics(FT_NORMAL);
	int fontHeight  = fm.lineSpacing();
	QString name = getName();

	int startX = offsetX + (w/2) - 25;
	int iconHeight = h - fontHeight;

	p.drawRect(startX, offsetY, 50, h - fontHeight + 1);
	p.drawLine(startX + 20, offsetY, startX + 20, offsetY + iconHeight);
	p.drawLine(startX + 30, offsetY, startX + 30, offsetY + iconHeight);
	p.drawLine(startX + 40, offsetY, startX + 40, offsetY + iconHeight);
	p.drawLine(startX, offsetY + (iconHeight/2), startX + 49, offsetY + (iconHeight/2));
	p.drawLine(startX, offsetY + (iconHeight/2) + (iconHeight/4),
		   startX + 49, offsetY + (iconHeight/2) + (iconHeight/4));

	QPen thickerPen = p.pen();
	thickerPen.setWidth(2);
	p.setPen(thickerPen);
	p.drawLine(startX + 10, offsetY, startX + 10, offsetY + iconHeight);
	p.drawLine(startX, offsetY + (iconHeight/4), startX + 50, offsetY + (iconHeight/4));

	p.setPen( QPen(black) );
	p.setFont(font);

	p.drawText(offsetX, offsetY + h - fontHeight,
		   w, fontHeight, AlignCenter, name);

	if(m_bSelected) {
		drawSelected(&p, offsetX, offsetY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::draw(QPainter& p, int offsetX, int offsetY) {
	if ((static_cast<UMLArtifact*>(m_pObject))->getDrawAsType() == defaultDraw) {
		return drawAsNormal(p, offsetX, offsetY);
	} else if ((static_cast<UMLArtifact*>(m_pObject))->getDrawAsType() == file) {
		return drawAsFile(p, offsetX, offsetY);
	} else if ((static_cast<UMLArtifact*>(m_pObject))->getDrawAsType() == library) {
		return drawAsLibrary(p, offsetX, offsetY);
	} else if ((static_cast<UMLArtifact*>(m_pObject))->getDrawAsType() == table) {
		return drawAsTable(p, offsetX, offsetY);
	} else {
		kdWarning() << "Artifact drawn as unknown type" << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
QSize ArtifactWidget::calculateIconSize() {
	int width, height;

	QFontMetrics &fm = getFontMetrics(FT_BOLD_ITALIC);
	int fontHeight  = fm.lineSpacing();

	width = fm.width( m_pObject->getName() );

	width = width<50 ? 50 : width;

	height = 50 + fontHeight;

	setSize(width, height);
	adjustAssocs( getX(), getY() );//adjust assoc lines
	return QSize(width, height);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
QSize ArtifactWidget::calculateNormalSize() {
	int width, height;

	QFontMetrics &fm = getFontMetrics(FT_BOLD_ITALIC);
	int fontHeight  = fm.lineSpacing();

	width = fm.width( m_pObject->getName() );

	int tempWidth = 0;
	if(m_pObject->getStereotype() != "") {
		tempWidth = fm.width("<< " + m_pObject->getStereotype() + " >>");
	}
	width = tempWidth>width ? tempWidth : width;
	width += ARTIFACT_MARGIN * 2;

	height = (2*fontHeight) + (ARTIFACT_MARGIN * 2);

	setSize(width, height);
	adjustAssocs( getX(), getY() );//adjust assoc lines
	return QSize(width, height);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ArtifactWidget::calculateSize() {
	if ( !m_pObject) {
		return;
	}
	if ((static_cast<UMLArtifact*>(m_pObject))->getDrawAsType() == defaultDraw) {
		calculateNormalSize();
	} else {
		calculateIconSize();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ArtifactWidget::activate(IDChangeLog* ChangeLog /* = 0 */) {
	bool status = UMLWidget::activate(ChangeLog);
	if(status) {
		calculateSize();
	}
	return status;
}

bool ArtifactWidget::saveToXMI(QDomDocument& qDoc, QDomElement& qElement) {
	QDomElement conceptElement = qDoc.createElement("artifactwidget");
	bool status = UMLWidget::saveToXMI(qDoc, conceptElement);
	qElement.appendChild(conceptElement);
	return status;
}

