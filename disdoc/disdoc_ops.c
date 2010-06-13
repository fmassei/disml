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
#include "disdoc_ops.h"

static ret_t copy_or_die(char **dest, char *src)
{
    if ((*dest = xstrdup(src))==NULL)
        return MMP_ERR_ENOMEM;
    return MMP_ERR_OK;
}

static ret_t parse_section_kv(t_section_s *sec, t_diskv_s *kv)
{
    if (!strcmp(kv->key, "title"))
        return copy_or_die(&(sec->title), kv->val);
    if (!strcmp(kv->key, "text"))
        return copy_or_die(&(sec->text), kv->val);
    if (!strcmp(kv->key, "type"))
        return copy_or_die(&(sec->type), kv->val);
    return MMP_ERR_PARSE;
}

static t_section_s *disobj_to_section(t_disobj_s *obj);
static ret_t parse_section_obj(t_section_s *sec, t_disobj_s *obj)
{
    if (!strcmp(obj->name, "section")) {
        t_section_s *s;
        if ((s = disobj_to_section(obj))==NULL)
            return MMP_ERR_PARSE;
        return mmp_list_add_data(sec->section, s);
    }
    return MMP_ERR_PARSE;
}

static void destroy_section_v(void **ptr);
static void destroy_section(t_section_s **sec)
{
    if (sec==NULL || *sec==NULL) return;
    if ((*sec)->title!=NULL) xfree((*sec)->title);
    if ((*sec)->text!=NULL) xfree((*sec)->text);
    if ((*sec)->type!=NULL) xfree((*sec)->type);
    if ((*sec)->section) mmp_list_delete_withdata(&(*sec)->section,
                                                        destroy_section_v);
    xfree(*sec);
    *sec = NULL;
}
static void destroy_section_v(void **ptr)
{
    destroy_section((t_section_s **)ptr);
}

static t_section_s *disobj_to_section(t_disobj_s *obj)
{
    t_section_s *ret;
    t_diselem_s *de;
    t_mmp_listelem_s *p;
    if (strcmp(obj->name, "section"))
        return NULL;
    ret = xmalloc(sizeof(*ret));
    ret->title = ret->text = ret->type = NULL;
    ret->section = mmp_list_create();
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        de = (t_diselem_s *)p->data;
        if (de->type==OT_KV) {
            if (parse_section_kv(ret, de->elem.kv)!=MMP_ERR_OK) {
                destroy_section(&ret);
                goto done;
            }
        } else {
            if (parse_section_obj(ret, de->elem.obj)!=MMP_ERR_OK) {
                destroy_section(&ret);
                goto done;
            }
        }
    }
done:
    return ret;
}

static ret_t parse_document_kv(t_document_s *doc, t_diskv_s *kv)
{
    if (!strcmp(kv->key, "title"))
        return copy_or_die(&(doc->title), kv->val);
    if (!strcmp(kv->key, "subtitle"))
        return copy_or_die(&(doc->subtitle), kv->val);
    if (!strcmp(kv->key, "author1"))
        return copy_or_die(&(doc->author1), kv->val);
    if (!strcmp(kv->key, "author1mail"))
        return copy_or_die(&(doc->author1mail), kv->val);
    if (!strcmp(kv->key, "url"))
        return copy_or_die(&(doc->url), kv->val);
    return MMP_ERR_PARSE;
}

static ret_t parse_document_obj(t_document_s *doc, t_disobj_s *obj)
{
    if (!strcmp(obj->name, "section")) {
        t_section_s *s;
        if ((s = disobj_to_section(obj))==NULL)
            return MMP_ERR_PARSE;
        return mmp_list_add_data(doc->section, s);
    }
    return MMP_ERR_PARSE;
}

void destroy_document(t_document_s **doc)
{
    if (doc==NULL || *doc==NULL) return;
    if ((*doc)->title!=NULL) xfree((*doc)->title);
    if ((*doc)->subtitle!=NULL) xfree((*doc)->subtitle);
    if ((*doc)->author1!=NULL) xfree((*doc)->author1);
    if ((*doc)->author1mail!=NULL) xfree((*doc)->author1mail);
    if ((*doc)->url!=NULL) xfree((*doc)->url);
    mmp_list_delete_withdata(&((*doc)->section), destroy_section_v);
    xfree(*doc);
    *doc = NULL;
}

t_document_s *disobj_to_document(t_disobj_s *obj)
{
    t_document_s *ret;
    t_diselem_s *de;
    t_mmp_listelem_s *p;
    if (strcmp(obj->name, "document"))
        return NULL;
    ret = xmalloc(sizeof(*ret));
    ret->section = mmp_list_create();
    ret->title = ret->subtitle = ret->author1 = ret->author1mail = ret->url =
        NULL;
    for (p=obj->elemlist->head; p!=NULL; p=p->next) {
        de = (t_diselem_s *)p->data;
        if (de->type==OT_KV) {
            if (parse_document_kv(ret, de->elem.kv)!=MMP_ERR_OK) {
                destroy_document(&ret);
                goto done;
            }
        } else {
            if (parse_document_obj(ret, de->elem.obj)!=MMP_ERR_OK) {
                destroy_document(&ret);
                goto done;
            }
        }
    }
done:
    return ret;
}

