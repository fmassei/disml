/*
    Copyright 2010 Francesco Massei

    This file is part of the DISML parser library.

        DISML parser is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DISML parser is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DISML parser.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef H_DISML_EXTRA_H
#define H_DISML_EXTRA_H

#include <mmp/mmp_h_utils.h>

#ifndef _WIN32
#   define DISML_EXTRA_API    extern
#else
#   ifdef DISML_EXPORTS
#       define DISML_EXTRA_API __declspec(dllexport)
#   else
#       define DISML_EXTRA_API __declspec(dllimport)
#   endif
#endif

#include <disml/disml.h>
#include "dis_to_xml.h"
#include "dis_to_json.h"

MMP_CEXTERN_BEGIN

/* convert from a "dis" object to an XML formatted file */
DISML_EXTRA_API ret_t disobj_to_xml(const char *fname, t_disobj_s *obj);
/* convert from a "dis" object to an JSON formatted file */
DISML_EXTRA_API ret_t disobj_to_json(const char *fname, t_disobj_s *obj);

MMP_CEXTERN_END

#endif /* H_DISML_EXTRA_H */
