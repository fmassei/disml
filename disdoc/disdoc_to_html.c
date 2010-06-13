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
#include "disdoc_to_html.h"

#define HTML_HEADER_FMT \
    "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"" \
    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n" \
    "<html xmlns=\"http://www.w3.org/1999/xhtml\"><head>\n" \
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\" />" \
    "<title>%s</title>" \
    "<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" />" \
    "<meta name=\"generator\" content=\"DISML DSS2 0.1\" />" \
    "</head><body>\n"
#define HTML_FOOTER_FMT \
    "</body></html>"

static int ss[10], ssn;
static char ssbuf[0xff];

static char *getssbuf()
{
    char tmp[10];
    int i;
    ssbuf[0] = '\0';
    for (i=0; i<ssn+1; ++i) {
        sprintf(tmp, "%d.", ss[i]);
        strcat(ssbuf, tmp);
    }
    return ssbuf;
}
static int getHn(int n)
{
    ++n; /* start from 1 */
    if (n<4) return n;
    return 4;
}

static void print_section_v(void *ptr, void *fptr);
static void print_section(FILE *out, t_section_s *sec)
{
    ss[ssn]++;
    memset(ss+ssn+1, 0, sizeof(int)*10-ssn-1);
    if (sec->title)
        fprintf(out, "<h%d>%s %s</h%d>\n", getHn(ssn), getssbuf(), sec->title, getHn(ssn));
    if (sec->text)
        fprintf(out, "<p>%s</p>\n", sec->text);
    ++ssn;
    mmp_list_lambda_data_ext(sec->section, print_section_v, out);
    --ssn;
}
static void print_section_v(void *ptr, void *fptr)
{
    print_section((FILE*)fptr, (t_section_s *)ptr);
}

static void print_document(FILE *out, t_document_s *doc)
{
    fprintf(out, HTML_HEADER_FMT, ((doc->title!=NULL) ? doc->title : ""));
    if (doc->title)
        fprintf(out, "<h1>%s</h1>\n", doc->title);
    if (doc->subtitle)
        fprintf(out, "<h2>%s</h2>\n", doc->subtitle);
    if (doc->author1)
        fprintf(out, "<p><b>%s</b></p>\n", doc->author1);
    if (doc->author1mail)
        fprintf(out, "<p>&lt;<a href=\"mailto:%s\">%s</a>&gt;</p>\n",
                    doc->author1mail, doc->author1mail);
    if (doc->url)
        fprintf(out, "<p><b>This version</b>: <a href=\"%s\">%s</a></p>\n", doc->url, doc->url);
    memset(ss, 0, sizeof(ss)/sizeof(ss[0]));
    ssn = 0;
    mmp_list_lambda_data_ext(doc->section, print_section_v, out);
    fprintf(out, HTML_FOOTER_FMT);
}

ret_t disdoc_to_html(const char *fname, t_document_s *doc)
{
    FILE *out;
    if ((out = fopen(fname, "wb"))==NULL) {
        mmp_setError(MMP_ERR_FILE);
        return MMP_ERR_FILE;
    }
    print_document(out, doc);
    fclose(out);
    return MMP_ERR_OK;
}
