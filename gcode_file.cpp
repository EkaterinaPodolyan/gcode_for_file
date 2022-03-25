#include <fstream>
#include<iostream>
#include<string>
using namespace std;
/*Есть GCODE Данного формата X(value);Y(value);Z(value).
Он используется для отдачи команд микроконтроллеры, который в свою очередь управляет устройством управления.
Зачастую GCODE выполняется построчно, поэтому если в скрипте оказалась ошибка,
то только на ошибочной строке он остановится.
Дан  GCODE - выведете конечные координаты устройства.
*/
//Насколько я поняла задачу, дают нам строку, а работать надо с числами, до тех пор, пока формат записи не нарушен
//Предположим, что исходные координаты нам известны, а  остальные данные подаются из файла  file.txt построчно:
class pre_point
{
	int x_prev, y_prev, z_prev;
public:
	pre_point();
	pre_point(int, int, int);
	void SetX(int x) { this->x_prev += x; }
	void SetY(int y) { this->y_prev += y; }
	void SetZ(int z) { this->z_prev += z; }
	void print() { cout << x_prev << ' ' << y_prev << ' ' << z_prev << endl; }
};

pre_point::pre_point()
{
	x_prev = 0;
	y_prev = 0;
	z_prev = 0;
}

pre_point::pre_point(int x_prev, int y_prev, int z_prev)
{
	this->x_prev = x_prev;
	this->y_prev = y_prev;
	this->z_prev = z_prev;
}


bool prevraschalka(string s, int& x, int& y, int& z)
{
	x = 0; y = 0; z = 0;
	//выковыриваем Х
	int koef = 1;
	int index = 0;
	if (s[index] != 'X') { return false; }
	else { index++; }
	if (s[index] == '-') { index++; koef = -1; }
	while (s[index] < 58 && s[index]>47)
	{
		x = x * 10 + int(s[index]) - int('0');
		index++;
	}
	x *= koef;
	if (s[index] != ';') { return false; }
	else { index++; }
	//теперь Y
	if (s[index] != 'Y') { return false; }
	else { index++; }
	koef = 1;
	if (s[index] == '-') { index++; koef = -1; }
	while (s[index] < 58 && s[index]>47)
	{
		y = y * 10 + int(s[index]) - int('0');
		++index;
	}
	y *= koef;
	if (s[index] != ';') { return false; }
	else { index++; }
	//и, наконец, Z
	if (s[index] != 'Z') { return false; }
	else { index++; }
	koef = 1;
	if (s[index] == '-') { index++;  koef = -1; }
	while (s[index] < 58 && s[index]>47)
	{
		z = z * 10 + int(s[index]) - int('0');
		index++;
	}
	z *= koef;
	if (s[index] != ';') { return false; }
	else { return true; }
}


int main()
{
	setlocale(LC_ALL, "rus");
	int x = 0, y = 0, z = 0;
	string msg;
	ifstream fs;
	string path = "file.txt";
	/*вводим стартовые координаты*/
	int x_prev = 10, y_prev = 20, z_prev = 0;
	pre_point prev(x_prev, y_prev, z_prev);

	fs.open(path);
	if (!fs.is_open()) { cout << "файл открыть не удалось" << endl; }
	else {
		cout << "файл открыт" << endl;
		while (!fs.eof()) {
			msg = "";
			fs >> msg;
			cout << msg << endl;
			
			if (prevraschalka(msg, x, y, z))
			{
				prev.SetX(x);
				prev.SetY(y);
				prev.SetZ(z);
			}
			else
			{
				cout << endl << endl;
				prev.print();
				cout << "ERROR SCRIPT" << endl;
				fs.close();
				return 0;
			}
		}
	}
	prev.print();
}


		
