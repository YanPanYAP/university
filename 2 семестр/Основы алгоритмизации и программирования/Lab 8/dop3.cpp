#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct String
{
	string info;
	String* next;
};

void create(String** begin, String** end, string str)
{
	String* t = new String;
	t->info = str;
	t->next = nullptr;
	if (*begin == nullptr)
		*begin = *end = t;
	else
	{
		(*end)->next = t;
		*end = t;
	}
}

void fromFile(ifstream& f, String** begin, String** end)
{
	f.open("f.txt");
	if (!f)
	{
		cout << "Ошибка открытия файла";
		return;
	}

	string line;
	while (getline(f, line))
	{
		string symbols, numbers;
		bool lastWasDigit = false;
		for (int i = 0; i < line.length(); i++)
		{
			if (isdigit(line[i]))
			{
				numbers += line[i];
				lastWasDigit = true;
			}
			else
			{
				if (line[i] == ' ' && lastWasDigit)
				{
					lastWasDigit = false;
					continue;
				}
				symbols += line[i];
				lastWasDigit = false;
			}
		}
		line = symbols + " " + numbers;
		create(begin, end, line);
	}

	f.close();
}

void toFile(ofstream& f, String* begin)
{
	f.open("g.txt");
	if (!f)
	{
		cout << "Ошибка открытия файла";
		return;
	}

	while (begin != nullptr)
	{
		f << begin->info << endl;
		begin = begin->next;
	}

	f.close();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream inFile;
	ofstream outFile;
	String* begin = nullptr, * end, * t;
	fromFile(inFile, &begin, &end);
	toFile(outFile, begin);
	cout << "Данные считаны из f.txt и записаны в g.txt" << endl;

	return 0;
}