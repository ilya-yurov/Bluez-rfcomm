#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:19:86:00:31:22";

    // выделяем сокет
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s < 0)
	printf("Can't open socket");

    // устанавливаем параметры подключения (к кому подключаться)
    addr.rc_family = AF_BLUETOOTH; //семейство адресов сокета. всегда будет AF_BLUETOOTH
    addr.rc_channel = (uint8_t) 0; //любой локальный адаптер Bluetooth является приемлемым
    str2ba( dest, &addr.rc_bdaddr ); //указывается номер порта для прослушивания

    // подключаемся к серверу
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // отправляем сообщение 
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");
    //закрываем сокет	
    close(s);
    return 0;
}
