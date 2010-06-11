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
#include <disml.h>
#include <mmp_trace.h>
#include "disgtk_treeview.h"

int main(int argc, char *argv[])
{
    t_disobj_s *obj;
    mmp_trace_reset();
    if (argc<2) {
        fprintf(stderr, "missing file name\n");
        return EXIT_FAILURE;
    }
    obj = dis_parse_file(argv[1]);
    if (obj==NULL) {
        mmp_trace_print(stdout);
        return EXIT_FAILURE;
    }
    disobj_smart_sort(obj);
    disgtk_gtktreeview(obj, argc, argv);
    printf("done.\n");
    return 0;
}

