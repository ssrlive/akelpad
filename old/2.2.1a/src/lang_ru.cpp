#include <windows.h>
#include <tchar.h>

char APP_SHORT_TITLE[]="AkelPad";
char APP_FULL_TITLE[]="AkelPad2";

char STR_DUMMY_NAME[]="AkelPad Synchronizing Class";

char STR_MODE_INSERT[]="Вставка";
char STR_MODE_OVERLAP[]="Замена";
char STR_MODIFIED[]="Изменён";

char STR_CP_WINDOWS[]="Windows-1251 (ANSI)";
char STR_CP_DOS[]="DOS-866 (OEM)";
char STR_CP_KOI[]="КОИ8-Р";
char STR_CP_UNICODE_UCS2_LE[]="Unicode (UCS-2 little endian)";
char STR_CP_UNICODE_UCS2_BE[]="Unicode (UCS-2 big endian)";
char STR_CP_UNICODE_UTF8[]="Unicode (UTF-8)";

char STR_FILE_FILTER[]="Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0\0";

char STR_DOCNAME[]="AkelPad Document";

char STR_HOMEPAGE[]="http://akelpad.sourceforge.net";

//akelpad.cpp messages
char MSG_ERROR_LOADING_DLL[]="Невозможно загрузить RICHED20.DLL";
char MSG_ERROR_IN_MESSAGE_QUEUE[]="Ошибка в очереди сообщений";
char MSG_ERROR_LIMIT_REACHED[]="Операция не может быть совершена всвязи с достижением лимита текста.\nЛимит будет увеличен.";
char MSG_ERROR_NOT_ENOUGH_MEMORY_FOR_RICHEDIT[]="Ошибка выделения памяти для окна редактирования.";
char MSG_ERROR_NOT_ENOUGH_MEMORY[]="Не хватает памяти";


//edit.cpp messages
char MSG_DOCUMENT_CHANGED[]="Документ был изменён. Сохранить?";
char MSG_FILE_WILL_BE_REOPENED[]="Файл будет открыт заново, все изменения будут потеряны.";
char MSG_WHETHER_APPEND_HEADER[]="Файлы в кодировке Unicode обычно имеют в своём начале метку, по которой идентифицируется кодировка. В этом файле метки нет. Добавить её?";
char MSG_ERROR_IO[]="Ошибка ввода/вывода.";
char MSG_ERROR_CANNOT_TRANSLATE[]="Ошибка: невозможно осуществить перекодировку.";
char MSG_ERROR_CANNOT_OPEN_FILE[]="Невозможно открыть файл.";
char MSG_ERROR_SYNTAX[]="Синтаксическая ошибка.";
char MSG_TITLE_SAVING_IN_UNICODE[]="Сохранение в Unicode";
char MSG_WRONG_STRING[]="Неправильный номер строки";
char MSG_ERROR_PRINT[]="Ошибка при печати документа";

//codepage.cpp messages
char MSG_WRONG_CODEPAGE[]="Неправильно указана кодовая страница.";
char MSG_CP_UNIMPLEMENTED[]="Данная кодовая страница не поддерживается.";
