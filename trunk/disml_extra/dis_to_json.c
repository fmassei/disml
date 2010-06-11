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
#include "dis_to_json.h"

static ret_t print_to_json(FILE *out, t_disobj_s *obj)
{
    t_diselem_s *de;
    t_mmp_listelem_s *p;
    int dd = 0;
    fprintf(out, "{%s:", obj->name);
    if (obj->elemlist->head==NULL)
        fprintf(out, "null");
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        de = (t_diselem_s *)p->data;
        if (de->type==OT_KV) {
            fprintf(out, ((dd++<=0) ? "{" : ","));
            fprintf(out, "%s=\"%s\"", de->elem.kv->key, de->elem.kv->val);
        } else {
            fprintf(out, ((dd++<=0) ? "{" : ","));
            print_to_json(out, de->elem.obj);
        }
    }
    fprintf(out, "}");
    return MMP_ERR_OK;
}

DISML_EXTRA_API ret_t disobj_to_json(const char *fname, t_disobj_s *obj)
{
    FILE *out;
    int ret = MMP_ERR_OK;
    if ((out = fopen(fname, "wb"))==NULL) {
        mmp_setError(MMP_ERR_FILE);
        return MMP_ERR_FILE;
    }
    ret = print_to_json(out, obj);
    fclose(out);
    return ret;
}

