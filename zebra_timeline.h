#ifndef __ZEBRA_TIMELINE_H__
#define __ZEBRA_TIMELINE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define ZEBRA_TIMELINE_TYPE 		(zebra_timeline_get_type())
#define ZEBRA_TIMELINE(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), ZEBRA_TIMELINE_TYPE, ZebraTimeline))
#define ZEBRA_TIMELINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), ZEBRA_TIMELINE_TYPE, ZebraTimelineClass))
#define IS_ZEBRA_TIMELINE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), ZEBRA_TIMELINE_TYPE))
#define IS_ZEBRA_TIMELINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), ZEBRA_TIMELINE_TYPE))

typedef struct _ZebraTimeline ZebraTimeline;
typedef struct _ZebraTimelineClass ZebraTimelineClass;

struct _ZebraTimeline {
	GtkWidget parent;
};

struct _ZebraTimelineClass {
	GtkWidgetClass parent_class;
};

GType 		zebra_timeline_get_type (void) G_GNUC_CONST;
GtkWidget 	*zebra_timeline_new(void);
static void zebra_timeline_class_init(ZebraTimelineClass *klass);
static void zebra_timeline_init(ZebraTimeline *timeline);
static void zebra_timeline_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void zebra_timeline_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static void zebra_timeline_realize(GtkWidget *widget);
static void zebra_timeline_size_request(GtkWidget *widget, GtkRequisition *requisition);
static void zebra_timeline_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gint zebra_timeline_expose(GtkWidget *widget, GdkEventExpose *event);

void zebra_timeline_set_bar(ZebraTimeline *timeline, gchar *bar);
gchar *zebra_timeline_get_bar(ZebraTimeline *timeline);
G_END_DECLS
#endif
