#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 }; //структура адресации определяет детали исходящего соединения или прослушивающего сокета
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // выделяем сокет
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // привязываем сокет к порту 1 первого доступного локального адаптера bluetooth
    loc_addr.rc_family = AF_BLUETOOTH; //семейство адресов сокета и всегда будет AF_BLUETOOTH
    loc_addr.rc_bdaddr = *BDADDR_ANY; //любой локальный адаптер Bluetooth является приемлемым
    loc_addr.rc_channel = (uint8_t) 0; //указывается номер порта для прослушивания
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // переводим сокет в режим прослушивания
    listen(s, 1);

    // принимаем одно соединение
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // читаем данные от клиента
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // закрываем соединение
    close(client);
    close(s);
    return 0;
}
