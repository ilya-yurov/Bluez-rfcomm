#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 }; //структура адресации определяет детали исходящего соединения или прослушивающего сокета
    int s, status;
    char dest[18] = "00:19:86:00:31:22"; //BD адрес сервера. в данном приложении клиент запрограммирован исключительно для подключения по этому адресу.

    // выделяем сокет
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM); //системный вызов socket основывается на информации о коммуникационном домене и типе сокета s = socket(domain, type, protocol)
    if (s < 0)
	printf("Can't open socket");

    // устанавливаем параметры подключения (к кому подключаться)
    addr.rc_family = AF_BLUETOOTH; //семейство адресов сокета. всегда будет AF_BLUETOOTH
    addr.rc_channel = (uint8_t) 1; //любой локальный адаптер Bluetooth является приемлемым
    str2ba( dest, &addr.rc_bdaddr ); //указывается номер порта для прослушивания. функция str2ba преобразует строку вида «XX: XX: XX: XX: XX: XX» и упаковывает ее в 6-байтовый bdaddr_t 

    // подключаемся к серверу
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr)); //функция connect()  (connect(s, serveraddr, serveraddrlen)) инициирует установление связи на сокете

    // отправляем сообщение 
    if( status == 0 ) {
        status = write(s, "hello!", 6); //пишем при помощи write() (write(s, buf, sizeof(buf))) сообщение в сокет
    }

    if( status < 0 ) perror("uh oh");
    //закрываем сокет	
    close(s);
    return 0;
}
