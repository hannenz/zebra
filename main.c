#include <gtk/gtk.h>
#include <clutter-gtk/clutter-gtk.h>
#include <clutter/clutter.h>
#include <cairo.h>
#include <stdio.h>
#include <webkit/webkit.h>

typedef struct {
	GtkWidget *window;
	GtkWidget *toolbar;
	GtkWidget *menubar;
	GtkWidget *statusbar;
	GtkWidget *stage;
	GtkWidget *colorpicker;
	GtkWidget *fileselector;
	GtkWidget *background_repeat_select;
	GtkWidget *notebook;
	ClutterActor *mainstage;
} APP;

APP app;

typedef enum {
	TOOL_SELECT,
	TOOL_RECTANGLE,
} TOOL;

TOOL tool = TOOL_SELECT;

ClutterActor *stage;
ClutterActor *tmpRect = NULL;

gchar *background_image_file;

guint x_0, y_0, x_1, y_1;

GList *actors = NULL;

static void on_file_set(GtkFileChooserButton *fcb, gpointer data){
	background_image_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fcb));
	if (background_image_file != NULL){
		g_print("File has been set:	%s\n", background_image_file);
	}
	else{
		g_print("Failed to set file\n");
	}
}

static void on_stage_dim_changed(GtkSpinButton *sbut, ClutterActor *stage){

	gchar *type = g_object_get_data(G_OBJECT(sbut), "type");
	gfloat w,h;
	clutter_actor_get_size(stage, &w, &h);

	if (!g_strcmp0(type, "width")){
		w = gtk_spin_button_get_value_as_int(sbut);
	}
	else if (!g_strcmp0(type, "height")){
		h = gtk_spin_button_get_value_as_int(sbut);
	}
	clutter_actor_set_size(stage, w, h);
}

static void on_new_button_clicked(GtkWidget *button){
	tool = TOOL_RECTANGLE;
	GdkCursor *crsr;
	crsr = gdk_cursor_new(GDK_CROSSHAIR);

	GdkWindow *gdk_window;
	gdk_window = gtk_widget_get_window(app.stage);
	gdk_window_set_cursor(gdk_window, crsr);
}

GtkWidget *main_window(){
	GtkWidget *vbox, *hpane, *vpane, *swin, *vbox2, *stage_width_entry, *stage_height_entry, *rectangle_button;
	WebKitWebView *web_view;
	ClutterActor *stage;
	ClutterColor
		stage_bg_color = { 128, 128, 128, 255 },
		white = { 255, 255, 255, 255 }
	;

	// Create widgets
	app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	app.menubar = gtk_menu_bar_new();
	app.toolbar = gtk_toolbar_new();
	hpane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	vpane = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	app.statusbar = gtk_statusbar_new();
	app.stage = gtk_clutter_embed_new();
	app.notebook = gtk_notebook_new();
	web_view= WEBKIT_WEB_VIEW(webkit_web_view_new());
	swin = gtk_scrolled_window_new(NULL, NULL);
	vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	stage_width_entry = gtk_spin_button_new_with_range(0, 9999, 1);
	stage_height_entry = gtk_spin_button_new_with_range(0, 9999, 1);
	app.colorpicker = gtk_color_button_new();
	app.fileselector = gtk_file_chooser_button_new("Background image", GTK_FILE_CHOOSER_ACTION_OPEN);
	app.background_repeat_select = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app.background_repeat_select), "no-repeat");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app.background_repeat_select), "repeat-x");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app.background_repeat_select), "repeat-y");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app.background_repeat_select), "repeat-xy");
	stage = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(app.stage));
	rectangle_button = gtk_button_new_from_stock(GTK_STOCK_NEW);
	app.mainstage = clutter_rectangle_new_with_color(&white);

	// Pack widgets
	gtk_container_add(GTK_CONTAINER(app.window), vbox);

	gtk_box_pack_start(GTK_BOX(vbox), app.menubar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), app.toolbar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), app.notebook, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), app.statusbar, FALSE, FALSE, 0);

	gtk_notebook_append_page(GTK_NOTEBOOK(app.notebook), vpane, gtk_label_new("Animator"));
	gtk_notebook_append_page(GTK_NOTEBOOK(app.notebook), swin, gtk_label_new("Browser"));
	gtk_notebook_append_page(GTK_NOTEBOOK(app.notebook), gtk_label_new("tbc"), gtk_label_new("Source"));

	gtk_container_add(GTK_CONTAINER(swin), GTK_WIDGET(web_view));

	gtk_paned_pack1(GTK_PANED(vpane), hpane, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(vpane), gtk_label_new("Timeline"), FALSE, TRUE);

	gtk_paned_pack1(GTK_PANED(hpane), app.stage,TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(hpane), vbox2, FALSE, TRUE);

	gtk_box_pack_start(GTK_BOX(vbox2), stage_width_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), stage_height_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), app.colorpicker, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), app.fileselector, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), app.background_repeat_select, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), rectangle_button, FALSE, FALSE, 0);



	//Setup widgets
	gtk_paned_set_position(GTK_PANED(hpane), 600);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	webkit_web_view_load_uri(web_view, "http://www.webkitgtk.org/");
	gtk_widget_set_size_request(app.stage, 800, 600);
	gtk_paned_set_position(GTK_PANED(hpane), 600);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(stage_width_entry), 300);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(stage_height_entry), 200);
	g_object_set_data(G_OBJECT(stage_width_entry), "type", "width");
	g_object_set_data(G_OBJECT(stage_height_entry), "type", "height");
	clutter_stage_set_user_resizable(CLUTTER_STAGE(stage), FALSE);
	gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(app.colorpicker), TRUE);
	gtk_combo_box_set_active(GTK_COMBO_BOX(app.background_repeat_select), 0);
	clutter_actor_set_background_color(stage, &stage_bg_color);
	clutter_actor_set_size(app.mainstage, 400, 300);
	clutter_actor_set_position(app.mainstage, 100, 100);
	clutter_actor_add_child(stage, app.mainstage);
	clutter_actor_show(app.mainstage);
	clutter_actor_show(stage);

	// Connect signals
	g_signal_connect(app.fileselector, "file-set", G_CALLBACK(on_file_set), NULL);
	g_signal_connect(app.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(stage_width_entry, "value-changed", G_CALLBACK(on_stage_dim_changed), stage);
	g_signal_connect(stage_height_entry, "value-changed", G_CALLBACK(on_stage_dim_changed), stage);
	g_signal_connect(rectangle_button, "clicked", G_CALLBACK(on_new_button_clicked), NULL);

	return (app.window);
}

static void on_gesture_begin(ClutterGestureAction *action, ClutterActor *stage, gpointer data) {
	gfloat x, y;
	ClutterColor transp = {0, 0, 0, 0};
	ClutterColor grey = {30, 30, 30, 128};

	switch (tool){
		case TOOL_RECTANGLE:

			clutter_gesture_action_get_press_coords(action, 0, &x, &y);

			tmpRect = clutter_rectangle_new_with_color(&transp);
			clutter_rectangle_set_border_width(CLUTTER_RECTANGLE(tmpRect), 1);
			clutter_rectangle_set_border_color(CLUTTER_RECTANGLE(tmpRect), &grey);

			clutter_actor_set_position(tmpRect, x, y);
			clutter_actor_set_size(tmpRect, 0, 0);
			clutter_actor_add_child(CLUTTER_ACTOR(stage), tmpRect);
			clutter_actor_show(tmpRect);

			x_0 = x;
			y_0 = y;
			break;
		default:
			break;
	}
}

static void on_gesture_progress(ClutterGestureAction *action, ClutterActor *stage, gpointer data) {
	gfloat x, y;
	gint dx, dy;
	clutter_gesture_action_get_motion_coords(action, 0, &x, &y);

	dx = (guint)x - (guint)x_0;
	dy = (guint)y - (guint)y_0;

	if (dx < 0 && dy > 0){
		clutter_actor_set_position(tmpRect, x, y_0);
	}
	if (dy < 0 && dx > 0){
		clutter_actor_set_position(tmpRect, x_0, y);
	}
	if (dx < 0 && dy < 0){
		clutter_actor_set_position(tmpRect, x, y);
	}

	clutter_actor_set_size(tmpRect, ABS(dx), ABS(dy));
}

static void on_gesture_end(ClutterGestureAction *action, ClutterActor *stage, gpointer data) {
	ClutterActor *new_actor, *texture, *actor;
	gfloat x, y, w, h;
	GError *error = NULL;
	GdkColor color;
	guint16 alpha;
	gint iw = 125;
	gint ih = 126;
	gboolean repeat_x = FALSE;
	gboolean repeat_y = TRUE;
	guint bgr;


	new_actor = tmpRect;

	gtk_color_button_get_color(GTK_COLOR_BUTTON(app.colorpicker), &color);
	alpha = gtk_color_button_get_alpha(GTK_COLOR_BUTTON(app.colorpicker));
	ClutterColor col =  {
		CLAMP(((color.red / 65535.0) * 255), 0, 255),
		CLAMP(((color.green / 65535.0) * 255), 0, 255),
		CLAMP(((color.blue / 65535.0) * 255), 0, 255),
		CLAMP(((alpha / 65535.0) * 255), 0, 255),

	};

	clutter_rectangle_set_color(CLUTTER_RECTANGLE(new_actor), &col);
	clutter_rectangle_set_border_width(CLUTTER_RECTANGLE(new_actor), 0);
	tmpRect = NULL;


	clutter_actor_get_position(new_actor, &x, &y);
	clutter_actor_get_size(new_actor, &w, &h);

	if (background_image_file != NULL){

		texture = clutter_texture_new_from_file(background_image_file, &error);
		if (error != NULL){
			g_print("Loading image failed\n");
			g_error_free(error);
		}
		clutter_actor_set_position(texture, x, y);
		clutter_actor_set_size(texture, w, h);
		clutter_actor_add_child(stage, texture);
		clutter_actor_show(texture);

		bgr = gtk_combo_box_get_active(GTK_COMBO_BOX(app.background_repeat_select));
		switch (bgr){
			case 0:
				repeat_x = repeat_y = FALSE;
				break;
			case 1:
				repeat_x = TRUE; repeat_y = FALSE;
				break;
			case 2:
				repeat_x = FALSE; repeat_y = TRUE;
				break;
			case 3:
				repeat_x = repeat_y = TRUE;
				break;
		}
		clutter_texture_get_base_size(CLUTTER_TEXTURE(texture), &iw, &ih);
		clutter_actor_set_clip(texture, 0, 0, repeat_x ? w : iw, repeat_y ? h : ih);
		clutter_texture_set_sync_size(CLUTTER_TEXTURE(texture), TRUE);
		clutter_texture_set_repeat(CLUTTER_TEXTURE(texture), TRUE, TRUE);
		clutter_texture_set_keep_aspect_ratio(CLUTTER_TEXTURE(texture), TRUE);
		actor = texture;
		clutter_actor_destroy(new_actor);
	}
	else {
		actor = new_actor;
	}
	tool = TOOL_SELECT;
	clutter_actor_add_action(actor, clutter_drag_action_new());
	clutter_actor_set_reactive(actor, TRUE);
	actors = g_list_append(actors, actor);
	GdkWindow *gdk_window;
	gdk_window = gtk_widget_get_window(app.stage);
	gdk_window_set_cursor(gdk_window, NULL);
}

void test_clutter(){
	ClutterActor *stage;
	ClutterAction *action;

	stage = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(app.stage));
	action = clutter_gesture_action_new();

	clutter_actor_add_action(CLUTTER_ACTOR(stage), action);
	g_signal_connect(action, "gesture-begin", G_CALLBACK(on_gesture_begin), NULL);
	g_signal_connect(action, "gesture-progress", G_CALLBACK(on_gesture_progress), NULL);
	g_signal_connect(action, "gesture-end", G_CALLBACK(on_gesture_end), NULL);
	//~ g_signal_connect(stage, "button-press-event", G_CALLBACK(on_button_press), NULL);
	tool = TOOL_RECTANGLE;
}

int main(int argc, char **argv){
	GtkWidget *window;


	gtk_init(&argc, &argv);

	if (gtk_clutter_init(&argc, &argv)){

		window = main_window();

		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
		gtk_widget_set_app_paintable(window, TRUE);

		gtk_widget_show_all(window);
		test_clutter();
		gtk_main();
	}
	return (0);
}
