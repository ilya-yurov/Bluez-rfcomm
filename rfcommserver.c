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
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM); //системный вызов socket основывается на информации о коммуникационном домене и типе сокета s = socket(domain, type, protocol)

    // привязываем сокет к порту 1 первого доступного локального адаптера bluetooth
    loc_addr.rc_family = AF_BLUETOOTH; //семейство адресов сокета AF_BLUETOOTH
    loc_addr.rc_bdaddr = *BDADDR_ANY; //любой локальный адаптер Bluetooth является приемлемым
    loc_addr.rc_channel = (uint8_t) 1; //указывается номер порта для прослушивания
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)); //Для связывания сокета с адресом и номером порта используют системный вызов bind. bind(s, name, namelen);

    // переводим сокет в режим прослушивания
    listen(s, 1); //системный вызов listen связывает сокет с общеизвестным адресом, ассоциирующимся с данным сервисом, и пассивно слушает этот сокет.
                  //listen(s, qlength) где s это дескриптор сокета, а qlength это максимальное количество запросов на установление связи, которые могут стоять в очереди, ожидая обработки сервером
    // принимаем одно соединение
    client = accept(s, (struct sockaddr *)&rem_addr, &opt); //системный вызов accept создает новый сокет и связывает его с ассоциацией, эквивалентной 'слушающему сокету'

    ba2str( &rem_addr.rc_bdaddr, buf ); //ba2str распаковывает  6-байтовый bdaddr_t и выдает строку вида «XX: XX: XX: XX: XX: XX», где каждый XX является шестнадцатеричным числом, определяющим октет 48-битного адреса
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf)); //заполняем массив нулями

    // читаем данные от клиента
    bytes_read = read(client, buf, sizeof(buf)); //читаем полученные от клиента данные при помощи системного вызова read(s, buf, sizeof(buf))
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // закрываем соединение
    close(client);
    close(s);
    return 0;
}
