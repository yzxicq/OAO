#include <gtk/gtk.h>

#define EXAMPLE_APP_TYPE (example_app_get_type ())
GType           example_app_get_type    (void);
typedef struct _ExampleApp       ExampleApp;

struct _ExampleApp
{
  GtkApplication parent;
};







ExampleApp *
example_app_new (void)
{
  return g_object_new (EXAMPLE_APP_TYPE,
                       "application-id", "org.gtk.exampleapp",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}







int
main (int argc, char *argv[])
{
  return g_application_run (G_APPLICATION (example_app_new ()), argc, argv);
}


