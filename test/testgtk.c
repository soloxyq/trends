//gcc -o gtk testgtk.c `/d/build_tools/gtk3/bin/pkg-config.exe --cflags --libs gtk+-3.0` -mwindows
//pkg-config --cflags gtk+-3.0
//pkg-config --libs gtk+-3.0
#include <gtk/gtk.h>
#include <stdio.h>
gint delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)                // 回调函数，当关闭窗体时执行
{
   g_print("程序已退出\n");
   return TRUE;                                                // 返回值为TRUE时，会调用destroy()函数
}
void destroy(GtkWidget *widget,gpointer data)// 回调函数
{
   gtk_main_quit ();                                // 结束主循环
}
int main(int argc, char *argv[])
{
   GtkWidget *window;
   GtkWidget *button;
   gtk_init(&argc, &argv);                                                        // 初始化GTK+函数库
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);        // 创建一个新窗口
   g_signal_connect (G_OBJECT(window), "delete_event",G_CALLBACK(delete_event), NULL);
                    // 关闭窗体时执行delete_event()回调函数
   g_signal_connect (G_OBJECT(window), "destroy",G_CALLBACK(destroy), NULL);
   button = gtk_button_new_with_label("Hello World");
   // 创建一个标签为“Hello World”的新按钮
   g_signal_connect_swapped(G_OBJECT(button), "clicked",G_CALLBACK(gtk_widget_destroy),window);
   // 当单击按钮时调用gtk_widget_destroy(window)函数关闭窗口
   gtk_container_add (GTK_CONTAINER(window), button);
                                                                        //把按钮放入窗体容器中
   gtk_widget_show (button);                // 显示按钮
   gtk_widget_show (window);                // 显示窗体
   gtk_main();                                                // 开始GTK+主循环
   return 0;
}