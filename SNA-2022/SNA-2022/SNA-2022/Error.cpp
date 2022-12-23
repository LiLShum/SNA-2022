#include "Libraries.h"
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "|IN|: Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "|IN|: Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(102),	ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "|IN|: Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "|IN|: Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "|IN|: Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY(113, "|IN|: Превышен максимальный размер исходного кода"),
		ERROR_ENTRY(114, "|IN|: Превышен максимальный размер строкового литерала "),
		ERROR_ENTRY_NODEF(115), ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120, "|LA|: Цепочка символов не разобрана"),
		ERROR_ENTRY(121, "|LA|: Таблица лексем переполнена"),
		ERROR_ENTRY(122, "|LA|: Таблица идентификаторов переполнена"),
		ERROR_ENTRY(123, "|LA|: Дублирование идентификатора"),
		ERROR_ENTRY(124, "|LA|: Дублирование арифметических операций"),
		ERROR_ENTRY(125, "|LA|: превышение допустимого значения типа"),
		ERROR_ENTRY_NODEF(126),ERROR_ENTRY_NODEF(127),ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),

		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "|SA|: Неверная структура программы"),
		ERROR_ENTRY(601, "|SA|: Ошибочный оператор"),
		ERROR_ENTRY(602, "|SA|: Неверное выражение"),
		ERROR_ENTRY(603, "|SA|: Ошибка в параметрах функции или операторе объявления"),
		ERROR_ENTRY(604, "|SA|: Ошибка в параметре вызываемой функции strtonum стандартной библиотеки"),
		ERROR_ENTRY(605, "|SA|: Блок содержит неверное подвыражение"),
		ERROR_ENTRY(606, "|SA|: Ошибка в параметре вызываемой функции strl стандартной библиотеки"),
		ERROR_ENTRY(607, "|SA|: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(608, "|SA|: Ошибка арифметического оператора"),

		ERROR_ENTRY(609, "|SA|: В языке не предусмотрено объявление переменных вне функций"), ERROR_ENTRY_NODEF(610),

		ERROR_ENTRY(611, "|SA|: Не найден конец правила"),
		ERROR_ENTRY(612, "|SA|: Цепочка разобрана не полностью (стек не пустой)"),
		ERROR_ENTRY(613, "|SA|: Точка входа в программу не задана"),
		ERROR_ENTRY_NODEF(614),	ERROR_ENTRY_NODEF(615), ERROR_ENTRY_NODEF(616),
		ERROR_ENTRY_NODEF(617), ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY(700, "|SMA|: Повторное объявление идентификатора"),
		ERROR_ENTRY(701, "|SMA|: Ошибка в типе идентификатора"),
		ERROR_ENTRY(702, "|SMA|: Ошибка в передаваемых значениях в функцию"),
		ERROR_ENTRY(703, "|SMA|: В функцию не переданы параметры"),
		ERROR_ENTRY(704, "|SMA|: Тип данных результата выражения не соответствует присваиваемому идентификатору"),
		ERROR_ENTRY(705, "|SMA|: Ошибка в параметре вызываемой функции strtonum стандартной библиотеки"),
		ERROR_ENTRY(706, "|SMA|: Необъявленный идентификатор"),
		ERROR_ENTRY(707, "|SMA|: Несоответствие типов в операторе присваивания"),
		ERROR_ENTRY(708, "|SMA|: Неверная структура программы"),
		ERROR_ENTRY(709, "|SMA|: Превышен максимальный размер идентификатора"),
		ERROR_ENTRY(710,"|SMA|: Повторное объявление точки входа"),ERROR_ENTRY_NODEF10(720),ERROR_ENTRY_NODEF10(730),ERROR_ENTRY_NODEF10(740),
		ERROR_ENTRY_NODEF10(750),ERROR_ENTRY_NODEF10(760),ERROR_ENTRY_NODEF10(770),ERROR_ENTRY_NODEF10(780),
		ERROR_ENTRY_NODEF10(790),
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR getError(int id)
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		return errors[id];
	}

	ERROR getErrorIn(int id, int line = -1, int col = -1)
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		errors[id].inText.line = line;
		errors[id].inText.positionInTheString = col;
		return errors[id];
	}
}