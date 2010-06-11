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
#include "dis_parse.h"

/* set the last error in the static error tracing array */
static void set_last_err(char *desc, int line_n)
{
    char buf[0xff];
    sprintf(buf, "line: %d, %s", line_n, desc);
    mmp_setError_ext(MMP_ERR_PARSE, buf);
}

/* get the indentation on the line pointed by str */
static int get_str_indent(char *str, char indchar)
{
    int i = 0;
    if (str==NULL || *str=='\0')
        return -1;
    while (str[i]==indchar && str[i]!='\0')
        ++i;
    return i;
}

/* strip the indentation */
static char *strip_str_indent(char *str, char indchar)
{
    if (str==NULL || *str=='\0')
        return NULL;
    while (*str==indchar && *str!='\0')
        ++str;
    return str;
}

/* remove the quotation marks from str, if they are present */
static char *kv_valesc(char *str)
{
    if (((str = mmp_str_trim(str))==NULL) || *str=='\0') return NULL;
    if (*str=='"' && str[strlen(str)-1]=='"') {
        ++str;
        str[strlen(str)-1] = '\0';
    }
    return str;
}

/* parse a "dis" key-value */
static t_diskv_s *diskv_parse(char *str)
{
    t_diskv_s *pair;
    char *eq, *k, *v;
    if ((eq = xindex(str, '='))==NULL) return NULL;
    eq[0] = '\0'; eq++;
    if (    ((k = mmp_str_trim(str))==NULL || *k=='\0') ||
            ((v = kv_valesc(eq))==NULL)    )
        return NULL;
    if (    ((pair = xmalloc(sizeof(*pair)))==NULL) ||
            ((pair->key = xstrdup(k))==NULL) ||
            ((pair->val = xstrdup(v))==NULL)   )
        diskv_delete(&pair);
    return pair;
}

/* parse a "dis" object */
static t_disobj_s *disobj_parse(t_disobj_s *par, char *str)
{
    t_disobj_s *obj;
    if ((str = mmp_str_trim(str))==NULL || *str=='\0') return NULL;
    if (    ((obj = xmalloc(sizeof(*obj)))==NULL) ||
            ((obj->name = xstrdup(str))==NULL) ||
            ((obj->elemlist = mmp_list_create())==NULL) ) {
        disobj_delete(&obj);
    }
    if (obj) obj->parent = par;
    return obj;
}

/* add a "dis" object as a child */
static t_disobj_s *add_to_disobj(t_disobj_s *obj, char *str)
{
    t_disobj_s *ret;
    t_diselem_s *elem=NULL;
    if (str==NULL || *str=='\0') return NULL;
    if (xindex(str, '=')) {
        t_diskv_s *kv;
        if ((kv = diskv_parse(str))==NULL) return NULL;
        if ((elem = kv_to_elem(kv))==NULL) {
            diskv_delete(&kv); return NULL; }
        ret = obj;
    } else {
        if ((ret = disobj_parse(obj, str))==NULL) return NULL;
        if (obj) {
            if ((elem = obj_to_elem(ret))==NULL) {
                disobj_delete(&obj); return NULL; }
        }
    }
    if (obj) {
        if (mmp_list_add_data(obj->elemlist, elem)!=0) {
            diselem_delete(&elem);
            return NULL;
        }
    }
    return ret;
}

/* semi-standard fgets way of correctly taking a line */
static ret_t get_and_validate_line(char *buf, size_t bufsize, FILE *in,
                                                                    int *line)
{
    ++(*line);
    if (fgets(buf, bufsize, in)==NULL) {
        if (ferror(in)!=0) {
            set_last_err("Error reading file", *line);
            return MMP_ERR_FILE;
        }
        return MMP_ERR_OK;
    }
    if (strlen(buf)>=bufsize-1) {
        set_last_err("Line too long", *line);
        return MMP_ERR_PARSE;
    }
    return MMP_ERR_OK;
}

#define MAXBUFSIZE  150

/* semi-generic fgets, with reallocs, trimming and stuff */
static char *get_and_validate_wholeline(FILE *in, int *line, char ccont)
{
    char *buf=NULL, *tmp;
    int pline;
    pline = *line - 1;
    for(;;) {
        if ((tmp = xrealloc(buf, (*line-pline)*MAXBUFSIZE))==NULL) {
            if (buf!=NULL) xfree(buf);
            mmp_setError(MMP_ERR_ENOMEM);
            return NULL;
        }
        if (buf==NULL)
            tmp[0] = '\0';
        buf = tmp;
        tmp = buf + strlen(buf);
        if (get_and_validate_line(tmp, MAXBUFSIZE, in, line)!=MMP_ERR_OK) {
            xfree(buf);
            return NULL;
        }
        mmp_str_rtrim(tmp);
        if (*tmp=='\0' || tmp[strlen(tmp)-1]!=ccont)
            break;
        else
            tmp[strlen(tmp)-1]='\0';
    }
    return buf;
}

/* parse the DISML header */
static ret_t parse_first_line(FILE *in, int *line, char *indchar,
                                                char *brkchar, char *cmnchar)
{
    char *buf;
    size_t blen, mlen;
    if ((buf = get_and_validate_wholeline(in, line, '\0'))==NULL)
        return MMP_ERR_PARSE;
    blen = strlen(buf);
    mlen = strlen(DISML_MAGIC);
    if (strncmp(buf, DISML_MAGIC, mlen)) {
        set_last_err("Magic header not found", *line);
        xfree(buf);
        return MMP_ERR_PARSE;
    }
    if (blen>mlen) {
        *indchar = buf[strlen(DISML_MAGIC)];
    } else {
        *indchar = '\t';
    }
    if (blen>mlen+1) {
        *brkchar = buf[strlen(DISML_MAGIC)+1];
    } else {
        *brkchar = '\\';
    }
    if (blen>mlen+2) {
        *cmnchar = buf[strlen(DISML_MAGIC)+2];
    } else {
        *cmnchar = '#';
    }
    xfree(buf);
    return MMP_ERR_OK;
}

/* parse a file */
static t_disobj_s *parse_file(FILE *in)
{
    char *buf=NULL;
    char *parsable;
    t_disobj_s *act_obj, *last_obj;
    int act_ind, n_ind, line;
    char indchar, brkchar, cmnchar;
    act_obj = last_obj = NULL;
    act_ind = 0;
    line = 0;
    if (parse_first_line(in, &line, &indchar, &brkchar, &cmnchar)!=MMP_ERR_OK) {
        goto baderror;
    }
    while ((buf = get_and_validate_wholeline(in, &line, brkchar))!=NULL) {
        if (feof(in)) break;
        if (mmp_str_ltrim(buf)[0]==cmnchar) {
            xfree(buf);
            continue;
        }
        if (    ((n_ind = get_str_indent(buf, indchar))<0) ||
                ((parsable = strip_str_indent(buf, indchar))==NULL) ) {
            set_last_err("Incorrect indentation", line);
            goto baderror;
        }
        if (n_ind>(act_ind+1)) {
            set_last_err("Incorrect indentation", line);
            goto baderror;
        }
        if (n_ind<act_ind) {
            while(n_ind!=act_ind) {
                act_obj = act_obj->parent;
                --act_ind;
            }
        }
        if (n_ind>act_ind)
            act_obj = last_obj;
        if ((last_obj = add_to_disobj(act_obj, parsable))==NULL) {
            set_last_err("Error parsing", line);
            goto baderror;
        }
        act_ind = n_ind;
        xfree(buf);
    }
    if (buf!=NULL) xfree(buf);
    return disobj_find_topmost(act_obj);
baderror:
    if (act_obj) {
        act_obj = disobj_find_topmost(act_obj);
        disobj_delete(&act_obj);
    }
    if (buf!=NULL) xfree(buf);
    return NULL;
}

MMP_CEXTERN_BEGIN

DISML_API t_disobj_s *dis_parse_file(const char *fname)
{
    FILE *in;
    t_disobj_s *ret;
    if ((in = fopen(fname, "rb"))==NULL) {
        mmp_setError(MMP_ERR_FILE);
        return NULL;
    }
    if ((ret = parse_file(in))==NULL) {
        mmp_setError(MMP_ERR_PARSE);
        return NULL;
    }
    if (fclose(in)!=0) {
        mmp_setError(MMP_ERR_FILE);
        return ret;
    }
    return ret;
}

DISML_API void dis_reset_error_tracing(void) { mmp_trace_reset(); }
DISML_API void dis_print_error_tracing(void) { mmp_trace_print(stderr); }

MMP_CEXTERN_END

