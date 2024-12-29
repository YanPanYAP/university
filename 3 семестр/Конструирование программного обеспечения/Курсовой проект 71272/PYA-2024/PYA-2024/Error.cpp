#include"ERROR.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "[ERROR]# Недопустимый код ошибки"),
		ERROR_ENTRY(1, "[ERROR]# Системный сбой"),
		ERROR_ENTRY_NODEF(2),ERROR_ENTRY_NODEF(3),ERROR_ENTRY_NODEF(4),ERROR_ENTRY_NODEF(5),ERROR_ENTRY_NODEF(6),ERROR_ENTRY_NODEF(7),ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"[PARM]# Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101),ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104,"[PARM]# Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105),ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110,"[IN]# Ошибка при открытии файла с иходным кодом(-in)"),
		ERROR_ENTRY(111,"[LEX]# Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112,"[LOG]# Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY_NODEF(113),ERROR_ENTRY_NODEF(114),ERROR_ENTRY_NODEF(115),ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),ERROR_ENTRY_NODEF(118),ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120,"[LEX]# Превышен максимльный размер таблицы лексем"),
		ERROR_ENTRY(121,"[LEX]# Таблица лексем переполнена"),
		ERROR_ENTRY_NODEF(122), ERROR_ENTRY_NODEF(123), ERROR_ENTRY_NODEF(124),
		ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128),
		ERROR_ENTRY_NODEF(129),  ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY(160,"[LEX]# Превышено максимальное количество строк в таблице идентификаторов"),
		ERROR_ENTRY(161,"[LEX]# Таблица идентификаторов переполнена"),
		ERROR_ENTRY_NODEF(162),
		ERROR_ENTRY(163,"[LEX]# Не удалось распознать цепочку"),
		ERROR_ENTRY(164,"[LEX]# Размер литерала больше максимально допустимого"),
		ERROR_ENTRY(165,"[LEX]# Отсутствует точка входа main"),
		ERROR_ENTRY_NODEF(166),
		ERROR_ENTRY_NODEF(167), ERROR_ENTRY_NODEF(168),ERROR_ENTRY_NODEF(169),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY(300,"[SEM]# Превышен максимальный размер number типа"),
		ERROR_ENTRY(301,"[SEM]# В объявлении не указан тип идентификатора"),
		ERROR_ENTRY(302,"[SEM]# В объявлении отсутствует ключевое слово new"),
		ERROR_ENTRY(303,"[SEM]# Возвращаемый функцией тип не соответствует типу функции"),
		ERROR_ENTRY(304,"[SEM]# Ошибка в условии выражения"),
		ERROR_ENTRY(305,"[SEM]# Типы данных в выражении не совпадают"),
		ERROR_ENTRY(306,"[SEM]# Деление на ноль"),
		ERROR_ENTRY(307,"[SEM]# Необъявленный идентификатор"),
		ERROR_ENTRY(308,"[SEM]# Обнаружено несколько точек входа в main"),
		ERROR_ENTRY(309,"[SEM]# Обнаружен \' . Возможно, не закрыт строковый литерал"),
		ERROR_ENTRY(310,"[SEM]# В Symbol типе не может быть более 1 символа"),
		ERROR_ENTRY(311,"[SEM]# Повторное объявление идентификатора"),
		ERROR_ENTRY(312,"[SEM]# Ошибка в вызове стандартной функции "),
		ERROR_ENTRY(313,"[SEM]# Эта операция не применима к типу symbol/stroke"),
		ERROR_ENTRY(314,"[SEM]# В стандартную функцию передан необъявленный идентификатор"),
		ERROR_ENTRY(315,"[SEM]# Ожидается тип stroke/symbol"),
		ERROR_ENTRY(316,"[SEM]# Превышен максимальный размер типа stroke"),
		ERROR_ENTRY(317,"[SEM]# Ошибка в объявление идентификатора (указан неправильный тип)"),
		ERROR_ENTRY(318,"[SIN]# Ожидается ;"),
		ERROR_ENTRY(319,"[SIN]# Ожидается только одна ;"),
		ERROR_ENTRY(320,"[SEM]# Неверное количество параметров в вызове функции"),
		ERROR_ENTRY(321,"[SEM]# Незакрытый строковый литерал"),
		ERROR_ENTRY(322, "[SEM]# Некорректный контекст для $"),
		ERROR_ENTRY_NODEF10(330),
		ERROR_ENTRY(331,"# Превышено максимальное кол-во параметров"),
		ERROR_ENTRY_NODEF(331),ERROR_ENTRY_NODEF(332),ERROR_ENTRY_NODEF(333),ERROR_ENTRY_NODEF(334),ERROR_ENTRY_NODEF(335),ERROR_ENTRY_NODEF(336),
		ERROR_ENTRY_NODEF(337),ERROR_ENTRY_NODEF(338),ERROR_ENTRY_NODEF(339),
		ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"[SIN]# Неверная структура программы"),
		ERROR_ENTRY(601,"[SIN]# Неверная структура функции"),
		ERROR_ENTRY(602,"[SIN]# Ошибка в параметрах функции"),
		ERROR_ENTRY(603,"[SIN]# Ошибка в арифметической операции"),
		ERROR_ENTRY(604,"[SIN]# Ошибка в логической операции"),
		ERROR_ENTRY(605,"[SIN]# Ошибка в параметрах функции статической библиотеки"),
		ERROR_ENTRY(606,"[SIN]# Ошибка в вызове стандартной функции"),
		ERROR_ENTRY(607,"[SIN]# Ошибка при инициализацие переменной"),
		ERROR_ENTRY(608,"[SIN]# Ошибка присвоения значение идентификатору"),
		ERROR_ENTRY(609,"[SIN]# Ошибка в выражении"),
		ERROR_ENTRY(610,"[SIN]# Ожидается идентификатор или литерал"),
		ERROR_ENTRY(611,"[SIN]# Ошибка в структуре условного выражения"),
		ERROR_ENTRY(612,"[SIN]# Ошибка построения условного выражения"),
		ERROR_ENTRY(613,"[SIN]# Ошибка тела"),
		ERROR_ENTRY(614,"[SIN]# Ошибка в объявлении переменной"),
		ERROR_ENTRY(615,"[SIN]# Функция определена после main"),

		ERROR_ENTRY_NODEF(616),ERROR_ENTRY_NODEF(617),ERROR_ENTRY_NODEF(618),ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900),
	};
	ERROR geterror(int id) {
		if (0 > id || id >= ERROR_MAX_ENTRY)
			return errors[0];
		else
			return errors[id];
	};
	ERROR geterrorin(int id, int line = -1, int col = -1) {
		ERROR e;
		if (id < 0 || id >= ERROR_MAX_ENTRY)
			e = errors[0];
		else
		{
			e = errors[id];
			e.inext.line = line;
			e.inext.column = col;
		}
		return e;
	};
};