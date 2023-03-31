Чат 3
Процесс-сервер:
1) создает "слушающий" сокет и ждет запросов на соединение от клиентов;
2) при поступлении запроса, устанавливается соединение с очередным клиентом, от клиента
сервер получает имя (ник) вошедшего в "комнату для разговоров", клиент заносится в список
присутствующих;
3) всем присутствующим рассылается сообщение, что в комнату вошел такой-то (имя);
4) от разных клиентов могут поступать реплики – получив реплику от клиента, сервер рассылает
ее всем "присутствующим" (включая самого автора) с указанием автора реплики;
5) при разрыве связи (команда \quit) с клиентом сервер сообщает всем, что-такой-то (имя) нас
покинул (ушел) и выводит его прощальное сообщение.

Один из типов пользователей – администраторы, для того чтобы получить доступ к
функционалу администраторов пользователь должен выполнить команду \admin, после чего сервер
запрашивает пароль, при вводе корректного пароля пользователь получает доступ к
дополнительным командам. Администраторы не могут влиять на других администраторов.

Реализованы команды:
1) \users получить от сервера список всех пользователей (имена), которые сейчас онлайн;

2) \quit <message> – выход из чата с прощальным сообщением.

3)\private <nickname> <message> приватное сообщение пользователю с именем <nickname>,
если пользователя-адресата на сервере нет, то выдается сообщение об ошибке;

4) \privates – получить от сервера имена пользователей которым вы отправляли приватные
сообщения;

5) \help – вывод допустимых команд.

6) \ban <nickname> <message> – отключает текущего пользователя с заданным именем и запрещает
новые подключения к серверу с таким именем, при этом пользователю выводится причина его
блокировки, указанная в <message>;

7) \kick <nickname> <message> – отключает пользователя с заданным именем, при этом
пользователю выводится причина его блокировки, указанная в <message> ;

8) \nick <oldnickname> <newnickname> – принудительно меняет имя пользователю;

9) \shutdown <meassage> – завершает работу сервера, а всем пользователям отправляется заданное
сообщение.

10) \nick <новое_имя> - позволяет любому пользователю поменять свой ник.

Файлы:
client.cpp - Подключает клиента к серверу
lgraph.cpp - Разбивает сообщения на слова и специальные символы
main.cpp - Запускает сервер и обрабатывает сообщения и команды

Функции обрабатывающие слова:

int lex(char ***mass, char *buf, int *sizelist, int *curlist, int *sizebuf, int *curbuf, int socket)()
-Разбивает сообщения на слова и заполняет ими массив mass.

void null_list(char **mass, int *sizelist, int *curlist)
-инициализирует список mass
-обнуляет размер списка sizelist
-обнуляет счетчик текущего свободного элемента curlist

void clearlist(char **mass, int *sizelist, int *curlist)
-Очищает список mass
-Обнуляет счетчики sizelist и  curlist

void termlist(char ***mass, int *sizelist, int *curlist)
-Корректирует размер списка mass
-Корректирует счетчики списка sizelist и curlist

void printlist(char ***mass, int *sizelist)
-Печатает список mass

void null_buf(char *buf, int *sizebuf, int *curbuf)
-Очищает буфер buf

void addsym(int c, char **buf, int *sizebuf, int *curbuf)
-Добавляет символ с в буфер buf
-Меняет счетчики буфера sizebuf и curbuf

int symset(int c)
-проверяет символ с на непечатающиеся символы

void addword(char **buf, int *sizebuf, int *curbuf, char ***mass, int *sizelist, int *curlist)
-Добавляет в список mass, слово из buf
-Обновляет счетчики буфера и списка

class client
    char *concatenation(const char *buf, char *name) // функция конкантенирующая строку
    char **privates; // массив пользователей, которым были отправлены приватные сообщения
    int cur_privates; //текущее количество "приватных" пользователей
    int size_privates; // размер массива privates
    typedef enum {Nickname, Normal, Password} States; // состояния пользователя
    States state; //текущее состояние пользователя
    char admin_password[strlen("12345")+1] = {"12345"}; //админский пароль
public:
    int admin; // флаг админа
    int socket; // номер клиентского сокета
    char *name; //никнейм пользователя
    int send_message; // количество отправленных сообщений пользователем
    
    int accept_client(int ls, client *desk, int *cur_desk)
    -Подключает пользователя к серверу и выдает номер сокета
    
    int check_state()
    -Проверяет состояние клиента
    
    int read_client(char ***mass, char *buf, int *sizelist, int *curlist, int *sizebuf, int *curbuf)
    -Обрабатывает массив слов mass и ищет команды
    
    int write_client(char ***mass, int *sizelist, int *curlist, char *name)
    -Печатает сообщение mass пользователю
    
    int add_privates(char *name)
    -Добавляет пользователя с именем name в массив privates

    int print_privates()
    -Печатает содержимое массива privates

    int private_message(char **mass, int sizelist, int curlist, char *name, client *address)
    -Отправляет пользователю adress приватное сообщение

    int qiut_client(char **mass, int sizelist, int curlist, char *name)
    -Рассылает последнее сообщение от пользователя name

    int entry_client(char *name)
    -Оповещает всех пользователей о новом участнике беседы

    int change_nickname(char ***mass, int *sizelist, int *curlist, client *user = NULL)
    -Меняет никнейм пользователю user либо самому себе

    int ban_client(char **mass, int sizelist, int curlist, client *ban_client, int flag_kick)
    -Кикает либо банит клиента ban_client

    int set_admin()
    -Просит ввести пользователя пароль, если пользователь хочет получить доступ к админским командам
    
    int get_messages()
    -Возвращает количество напечатанных пользователем сообщений

    int clear()
    -Очищает структуры клиента

class channel
    char *name; // Имя канала
    char *password; //Пароль канала
    client *desk; //Массив пользователей канала
    int size_desk; // Размер массива desk
    char **ban_nick; // Массив забаненых пользователей
    int size_ban; // Размер массива ban_nick
    int cur_ban;  // Текущее количество забаненых пользователей
public:
    int cur_desk; // Текущее количество активных пользователей канала
    
    int add_client(int socket)
    -Добавляет нового пользователя в канал

    int update(client **a, int *cur)
    -Обновляет информацию о состоянии канала

    int delit_client(client *a)
    -Удаляет клиента из массива desk
    
    int check_nick(client *user, char *name)
    -Проверяет есть ли такой ник на сервере

    int check_ban(client *user, char *name)
    -Проверяет забанен ли такой ник

    int add_ban_nick(char *nick)
    -Добавляет забаненный ник

class server 
    channel *chat; //массив чатов
    int size_chat; //размер массива *chat
    int cur_chat; //текущее количество чатов на сервере
public:
    int main_socket; //номер сокета сервера
    
    int add_channel(channel *ch)
    -Добавляет канал на сервер
    
    int get_socket()
    -Возвращает номер сокета сервера
