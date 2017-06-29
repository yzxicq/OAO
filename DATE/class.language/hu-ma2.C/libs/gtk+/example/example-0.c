#include <gtk/gtk.h>

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);		//app内窗口新建
  gtk_window_set_title (GTK_WINDOW (window), "Window");    //窗口内设置标题
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);	//窗口内设置默认大小
  gtk_widget_show_all (window);				//小部件内显示所有	
}

int
main (int    argc,
      char **argv)	//开始
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE); //app新建
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL); //
  status = g_application_run (G_APPLICATION (app), argc, argv);	//app运行
  g_object_unref (app);

  return status;
}
