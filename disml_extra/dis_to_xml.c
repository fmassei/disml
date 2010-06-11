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
#include "dis_to_xml.h"

static ret_t print_to_xml(FILE *out, t_disobj_s *obj)
{
    t_diselem_s *de;
    t_mmp_listelem_s *p;
    int closed = 0;
    fprintf(out, "<%s", obj->name);
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        de = (t_diselem_s *)p->data;
        if (de->type==OT_KV) {
            fprintf(out, " %s=\"%s\"", de->elem.kv->key, de->elem.kv->val);
        } else {
            if (!closed) {
                fprintf(out, ">\n");
                closed = 1;
            }
            print_to_xml(out, de->elem.obj);
        }
    }
    if (!closed) fprintf(out, ">\n");
    fprintf(out, "</%s>", obj->name);
    return MMP_ERR_OK;
}

DISML_EXTRA_API ret_t disobj_to_xml(const char *fname, t_disobj_s *obj)
{
    FILE *out;
    int ret = MMP_ERR_OK;
    if ((out = fopen(fname, "wb"))==NULL) {
        mmp_setError(MMP_ERR_FILE);
        return MMP_ERR_FILE;
    }
    disobj_smart_sort(obj);     /* smart sort is necessary to have kv first */
    ret = print_to_xml(out, obj);
    fclose(out);
    return ret;
}

