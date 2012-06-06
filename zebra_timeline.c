#include <gtk/gtk.h>
#include "zebra_timeline.h"

#define ZEBRA_TIMELINE_GET_PRIVATE(obj)		(G_TYPE_INSTANCE_GET_PRIVATE((obj), ZEBRA_TIMELINE_TYPE, ZebraTimelinePrivate))


typedef struct _ZebraTimelinePrivate ZebraTimelinePrivate;

struct _ZebraTimelinePrivate {
	// Add private properties
};

enum {
	FOO_SIGNAL,
	// Add signals
	LAST_SIGNAL
};

enum {
	PROP_0,
	PROP_BAR
	// Add Properties
};

GType zebra_timeline_get_type(void){
	static GType timeline_type = 0;
	if (!timeline_type){
		static const GTypeInfo timeline_info = {
			sizeof(ZebraTimelineClass),
			NULL,
			NULL,
			(GClassInitFunc) zebra_timeline_class_init,
			NULL,
			NULL,
			sizeof(ZebraTimeline),
			0,
			(GInstanceInitFunc) zebra_timeline_init
		};
		timeline_type = g_type_register_static(GTK_TYPE_WIDGET, "ZebraTimeline", &timeline_info, 0);
	}
	return timeline_type;
}

static void zebra_timeline_class_init(ZebraTimelineClass *klass){
	GObjectClass *gobject_class;
	GtkWidgetClass *widget_class;

	gobject_class = (GObjectClass*)klass;
	widget_class = (GtkWidgetClass*)klass;

	gobject_class->set_property = zebra_timeline_set_property;
	gobject_class->get_property = zebra_timeline_get_property;

	widget_class->realize = zebra_timeline_realize;
	widget_class->expose = zebra_timeline_expose;
	widget_class->size_request = zebra_timeline_size_request;
	widget_class->size_allocate = zebra_timeline_size_allocate;

	g_type_class_add_private(klass, sizeof(ZebraTimelinePrivate));

	g_object_class_install_property(gobject_class, PROP_BAR,
		g_param_spec_string("bar", "Bar message", "The bar message", "Foobar", G_PARAM_READWRITE)
	);
}

static void zebra_timeline_init(ZebraTimeline *timeline){
	//ZebraTimelinePrivate *priv = ZEBRA_TIMELINE_GET_PRIVATE(timeline);
	// Init private data
}

static void zebra_timeline_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec){
	ZebraTimeline *timeline = ZEBRA_TIMELINE(object);

	switch (prop_id){
		case PROP_BAR:
			zebra_timeline_set_bar(timeline, (gchar*)g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

static void zebra_timeline_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec){
	ZebraTimeline *timeline = ZEBRA_TIMELINE(object);
//	ZebraTimelinePrivate *priv = ZEBRA_TIMELINE_GET_PRIVATE(object);

	switch (prop_id){
		case PROP_BAR:
			g_value_set_string(value, zebra_timeline_get_bar(timeline));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

GtkWidget *zebra_timeline_new(){
	return GTK_WIDGET(g_object_new(zebra_timeline_get_type(), NULL));
}

static void zebra_timeline_realize(GtkWidget *widget){
	ZebraTimeline *timeline;
	GdkWindowAttr attributes;
	gint attr_mask;

	g_return_if_fail(widget != NULL);
	g_return_if_fail(IS_ZEBRA_TIMELINE(widget));

	gtk_widget_set_realized(widget, TRUE);
	timeline = ZEBRA_TIMELINE(widget);

	GtkAllocation allocation;

	gtk_widget_get_allocation(widget, &allocation);

	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.event_mask = gtk_widget_get_events(widget);
	attributes.event_mask |= (GDK_EXPOSURE_MASK);
	attributes.visual = gtk_widget_get_visual(widget);
	//~ attributes.colormap = gtk_widget_get_colormap(widget);

	attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

	GtkWidget *parent = gtk_widget_get_parent(widget);
	GdkWindow *gdk_window = gdk_window_new(gtk_widget_get_window(parent), &attributes, attr_mask);

	gtk_widget_set_window(widget, gdk_window);
	gdk_window_set_user_data(gdk_window, timeline);

	//~ widget->style = gtk_style_attach(widget->style, widget->window);
	//~ gtk_style_set_background(widget->style, widget->window, GTK_STATE_NORMAL);
	gdk_window_show(gdk_window);
}

void zebra_timeline_set_bar(ZebraTimeline *timeline, gchar *bar){
}

gchar *zebra_timeline_get_bar(ZebraTimeline *timeline){
	return NULL;
}

static void zebra_timeline_size_request(GtkWidget *widget, GtkRequisition *requisition){
}

static void zebra_timeline_size_allocate(GtkWidget *widget, GtkAllocation *allocation){
}

static gint zebra_timeline_expose(GtkWidget *widget, GdkEventExpose *event){
}
