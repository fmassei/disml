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
#include "dis_ops.h"

/* delete a "dis" key-value pair" */
void diskv_delete(t_diskv_s **kv)
{
    if (kv==NULL || *kv==NULL) return;
    if ((*kv)->key) xfree((*kv)->key);
    if ((*kv)->val) xfree((*kv)->val);
    xfree(*kv);
    *kv = NULL;
}

/* delete a "dis" element */
void diselem_delete(t_diselem_s **elem)
{
    if (elem==NULL || *elem==NULL) return;
    if ((*elem)->type==OT_KV && (*elem)->elem.kv)
        diskv_delete(&((*elem)->elem.kv));
    if ((*elem)->type==OT_OBJ && (*elem)->elem.obj) {
        disobj_delete(&((*elem)->elem.obj));
    }
    xfree(*elem);
    *elem = NULL;
}

/* void version of the "dis" element delete for lambda-like deletions */
static void diselem_delete_v(void **ptr)
{
    diselem_delete((t_diselem_s **)ptr);
}

/* delete a "dis" object */
void disobj_delete(t_disobj_s **obj)
{
    if (obj==NULL || *obj==NULL) return;
    if ((*obj)->name) xfree((*obj)->name);
    if ((*obj)->elemlist) {
        mmp_list_delete_withdata(&(*obj)->elemlist, diselem_delete_v);
    }
    xfree(*obj);
    *obj = NULL;
}

/* create a "dis" element from a "dis" key value */
t_diselem_s *kv_to_elem(t_diskv_s *kv)
{
    t_diselem_s *elem;
    if (kv==NULL) return NULL;
    if ((elem = xmalloc(sizeof(*elem)))==NULL) {
        mmp_setError(MMP_ERR_ENOMEM);
        return NULL;
    }
    elem->type = OT_KV;
    elem->elem.kv = kv;
    return elem;
}

/* create a "dis" element from a "dis" object */
t_diselem_s *obj_to_elem(t_disobj_s *obj)
{
    t_diselem_s *elem;
    if (obj==NULL) return NULL;
    if ((elem = xmalloc(sizeof(*elem)))==NULL) {
        mmp_setError(MMP_ERR_ENOMEM);
        return NULL;
    }
    elem->type = OT_OBJ;
    elem->elem.obj = obj;
    return elem;
}

/* find the root object */
t_disobj_s *disobj_find_topmost(t_disobj_s *obj)
{
    if (obj->parent==NULL)
        return obj;
    return disobj_find_topmost(obj->parent);
}

/* "dis" element comparer function */
static int diselem_compare_rl(const t_diselem_s *e1, const t_diselem_s *e2)
{
    if (e1->type!=e2->type)
        return (e1->type==OT_KV)?-1:1;
    if (e1->type==OT_KV)
        return strcmp(e1->elem.kv->key, e2->elem.kv->key);
    else
        return strcmp(e1->elem.obj->name, e2->elem.obj->name);
}
/* void version of the comparer function for lambda-like use */
static int diselem_compare(const void *e1,const void *e2)
{
    return diselem_compare_rl((t_diselem_s *)e1, (t_diselem_s *)e2);
}

MMP_CEXTERN_BEGIN

DISML_API void disobj_destroy(t_disobj_s **obj)
{
    disobj_delete(obj);
}

DISML_API void disobj_smart_sort(t_disobj_s *obj)
{
    t_mmp_listelem_s *p;
    t_diselem_s *e;
    mmp_list_sort_by_data(obj->elemlist, diselem_compare);
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        e = (t_diselem_s *)p->data;
        if (e->type==OT_OBJ)
            disobj_smart_sort(e->elem.obj);
    }
}

MMP_CEXTERN_END

