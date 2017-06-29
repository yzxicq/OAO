#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");	//
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);	//app内窗口新建
  gtk_window_set_title (GTK_WINDOW (window), "Window");	//窗口内标题设置
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);	//窗口内设置默认大小

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);	//新建按钮箱
  gtk_container_add (GTK_CONTAINER (window), button_box);	//添加到容器

  button = gtk_button_new_with_label ("Hello World");	//按钮内文本新建
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);	//行为链接,点击则，调用print_hello
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window); //行为链接，点击则，关闭app
  gtk_container_add (GTK_CONTAINER (button_box), button);	//添加到容器

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);	//app新建
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);	//行为链接
  status = g_application_run (G_APPLICATION (app), argc, argv);		//app运行
  g_object_unref (app);

  return status;
}
