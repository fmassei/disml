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
#include "dis_print.h"

#define INDCHAR '\t'

static void nest_print(FILE *out, int nest)
{
    while(nest--) fprintf(out, "%c", INDCHAR);
}

static void print_first_line(FILE *out)
{
    fprintf(out, "%s%c\n", DISML_MAGIC, INDCHAR);
}

static int diskv_print(FILE *out, t_diskv_s *kv, int nest)
{
    if (kv==NULL) return -1;
    if (kv->key==NULL || kv->val==NULL) return -1;
    nest_print(out, nest);
    fprintf(out, "%s=\"%s\"\n", kv->key, kv->val);
    return 0;
}

static int disobj_print(FILE *out, t_disobj_s *obj, int nest)
{
    t_mmp_listelem_s *p;
    t_diselem_s *elem;
    if (obj==NULL) return -1;
    if (obj->name == NULL || obj->elemlist==NULL) return -1;
    nest_print(out, nest);
    fprintf(out, "%s\n", obj->name);
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        elem = (t_diselem_s *)p->data;
        if (elem->type==OT_OBJ) {
            if (disobj_print(out, elem->elem.obj, nest+1)!=0)
                return -1;
        } else {
            if (diskv_print(out, elem->elem.kv, nest+1)!=0)
                return -1;
        }
    }
    return 0;
}

MMP_CEXTERN_BEGIN

DISML_API ret_t dis_print_file(char *fname, t_disobj_s *obj)
{
    FILE *out;
    ret_t ret;
    ret = MMP_ERR_OK;
    if (obj==NULL || fname==NULL || *fname=='\0') {
        mmp_setError(MMP_ERR_PARAMS);
        return MMP_ERR_PARAMS;
    }
    if ((out = fopen(fname, "wb"))==NULL) {
        mmp_setError(MMP_ERR_FILE);
        return MMP_ERR_FILE;
    }
    print_first_line(out);
    if (disobj_print(out, obj, 0)!=0) {
        mmp_setError(MMP_ERR_GENERIC);
        fclose(out);
        return MMP_ERR_GENERIC;
    }
    if (fclose(out)!=0) {
        mmp_setError(MMP_ERR_FILE);
        return MMP_ERR_FILE;
    }
    return ret;
}

MMP_CEXTERN_END

