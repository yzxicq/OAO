#include <stdlib.h>
#include <gtk/gtk.h>

typedef GtkApplication Hellostruct;				//need
typedef GtkApplicationClass HellostructClass;			//need
G_DEFINE_TYPE (Hellostruct, hello, GTK_TYPE_APPLICATION)	//need


static void
start (GApplication *app)
{
  GtkWidget *window;
  window = gtk_application_window_new (GTK_APPLICATION (app));	//窗口
  gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (window), FALSE);
  gtk_window_set_default_size ((GtkWindow*)window, 640, 480);
  gtk_window_set_title (GTK_WINDOW (window), "Sunny");
  gtk_window_set_icon_name (GTK_WINDOW (window), "sunny");
  gtk_widget_show_all (GTK_WIDGET (window));
}

static void hello_init (GtkApplication *app) 			//need
{
}

static void hello_class_init (GtkApplicationClass *class)	//need
{
  GApplicationClass *application_class = G_APPLICATION_CLASS (class);
  application_class->activate = start;
  //application_class->startup = startup;
  //application_class->open = open;
}


int main (int argc, char **argv)				//need
{
  GtkApplication *hello;
  int status;

  hello = g_object_new (hello_get_type (),
                       "application-id", "org.gtk.Test.Sunny",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
  status = g_application_run (G_APPLICATION (hello), argc, argv);
  g_object_unref (hello);

  return status;
}
