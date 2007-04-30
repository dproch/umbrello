/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef MESSAGEWIDGETLIST_H
#define MESSAGEWIDGETLIST_H

#include <q3ptrlist.h>

class MessageWidget;

typedef Q3PtrList<MessageWidget> MessageWidgetList;
typedef Q3PtrListIterator<MessageWidget> MessageWidgetListIt;

#endif
