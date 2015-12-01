/*************

    SCASE: extendable prototype of an assistive communication tool for people with Locked-in Syndrome

    Copyright (C) 2014  Andrés Aparicio

    This file is part of SCASE.

    SCASE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SCASE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SCASE.  If not, see <http://www.gnu.org/licenses/>.

    **********/

#ifndef SCASE_PLUGIN_WRITER_GLOBAL_H
#define SCASE_PLUGIN_WRITER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCASE_PLUGIN_WRITER_LIBRARY)
#  define SCASE_PLUGIN_WRITERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCASE_PLUGIN_WRITERSHARED_EXPORT Q_DECL_IMPORT
#endif

#define SCASE_PLUGIN_WRITER_NAME "net.adapar.SCASE.plugins.Writer/1.0"
#define SCASE_PLUGIN_WRITER_BROWSER_TREE_FILE "Writer.browserTree"
#define SCASE_PLUGIN_WRITER_SETTINGS_FILE "Writer"

#endif // SCASE_PLUGIN_WRITER_GLOBAL_H
