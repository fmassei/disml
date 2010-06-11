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
#include "disgtk_treeview.h"

enum { COL_NAME = 0, COL_VALUE, NUM_COLS };

static void fill_w_diskv(GtkTreeStore *store, GtkTreeIter *parent,
                                                                t_diskv_s *kv)
{
    GtkTreeIter iter;
    gtk_tree_store_append(store, &iter, parent);
    gtk_tree_store_set(store, &iter, COL_NAME, kv->key, COL_VALUE, kv->val, -1);
}
static void fill_w_disobj(GtkTreeStore *store, GtkTreeIter *parent,
                                                                t_disobj_s *obj)
{
    GtkTreeIter iter;
    t_mmp_listelem_s *le;
    t_diselem_s *de;
    gtk_tree_store_append(store, &iter, parent);
    gtk_tree_store_set(store, &iter, COL_NAME, obj->name, -1);
    for (le=obj->elemlist->head; le!=NULL; le=le->next) {
        de = (t_diselem_s *)le->data;
        if (de->type==OT_OBJ) {
            fill_w_disobj(store, &iter, de->elem.obj);
        } else {
            fill_w_diskv(store, &iter, de->elem.kv);
        }
    }
}

static GtkTreeModel *create_and_fill_model(t_disobj_s *obj)
{
  GtkTreeStore *store;
  store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
  fill_w_disobj(store, NULL, obj);
  return GTK_TREE_MODEL(store);
}

static GtkWidget *create_view_and_model(t_disobj_s *obj)
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;

  view = gtk_tree_view_new();

  /* --- Column #1 --- */
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (view),
                                               -1,      
                                               "Name",  
                                               renderer,
                                               "text", COL_NAME,
                                               NULL);
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (view),
                                               -1,      
                                               "Value",  
                                               renderer,
                                               "text", COL_VALUE,
                                               NULL);
  model = create_and_fill_model(obj);
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */
  g_object_unref(model);
  return view;
}

int disgtk_gtktreeview(t_disobj_s *obj, int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *view;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "delete_event", gtk_main_quit, NULL);
  view = create_view_and_model(obj);
  gtk_container_add(GTK_CONTAINER (window), view);
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}

