#include <gtk/gtk.h>
/* 콜백 함수 */
static void destroy(GtkWidget *widget,  gpointer data )
{
	gtk_main_quit ();
}
int main(int argc, char *argv[])
{
	/* GtkWidget 은 위젯들에 대한 저장 타입임. */
	GtkWidget *window, *button;
	gtk_init (&argc, &argv);
	/* 새로운 윈도우를 생성하고 테두리 폭 설정 */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	/* “Hello World” 라는 라벨을 가진 버튼 생성 */
	button = gtk_button_new_with_label ("Hello World");
	/* 버튼의 clicked 시그널과 destroy 콜백함수를 연결 */
g_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (destroy), NULL);
	/* 버튼을 gtk 컨테이너 위젯인 윈도우에 패킹함 */
	gtk_container_add (GTK_CONTAINER (window), button);
	/* 생성한 모든 위젯들을 화면에 표시 */
	gtk_widget_show (button); gtk_widget_show (window);
	/* 무한루프를 돌며 시그널과 이벤트를 처리하는 메인 함수 실행 */
	gtk_main ();
	return 0;
}
