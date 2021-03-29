#include "includes.h"

class Polinom : TeX_convertible
{
private:
	list<monom> _polinom;
public:
	friend bool uniformity(const Polinom& obj); // однородность
	friend bool harmony(Polinom& obj); // гармоничность
	friend ostream& operator<<(ostream& out, const Polinom& obj);
	friend istream& operator>>(istream& in, Polinom& obj);

	Polinom();
	Polinom(char* str);
	Polinom(const Polinom& obj);
	Polinom operator+(const Polinom& obj);
	Polinom& operator+=(const Polinom& obj);
	Polinom operator-(const Polinom& obj);
	Polinom& operator-=(const Polinom& obj);
	Polinom operator*(const Polinom& obj);
	Polinom& operator*=(const Polinom& obj);
	Polinom operator/(char* arr);
	Polinom& operator/=(char* arr);
	bool operator==(const Polinom& obj);
	bool operator!=(const Polinom& obj);

	~Polinom();
	string convert() const override;
};

bool uniformity(const Polinom& obj)
{
	Polinom copy(obj);
	int degree = 0, degree_new = 0;
	int flag = 0;
	// 0 - не инициализированно 
	// 1 - открыто
	// -1 - закрыто

	for (auto& iter1 : copy._polinom)
	{
		if (!flag && iter1 == copy._polinom.front()) { flag = 1; }

		for (auto& iter2 : iter1._monom)
		{
			if (flag == 1) { degree += iter2.second; } // если первый элемент инициализируем переменную степени 
			else { degree_new += iter2.second; }
		}

		if (degree_new != degree && flag != 1) { return false; }
		if (flag == 1) { flag = -1; } // закрыто
		degree_new = 0;
	}

	return true;
}

bool harmony(Polinom& obj)
{
	char* str = new char[SIZE];
	int i = 0;

	if (!obj._polinom.empty())
	{
		list<char> ls(SIZE);
		list<char> ls_new(ls);
		for (auto& iter1 : obj._polinom)
		{
			for (auto& iter2 : iter1._monom) { ls.push_back(iter2.first); }
		}

		stable_sort(ls.begin(), ls.end());
		auto size = unique_copy(ls.begin(), ls.end(), ls_new.begin());
		ls_new.resize(distance(ls_new.begin(), size));
		if (ls_new.front() == '\0') { ls_new.pop_front(); }

		auto iter = ls_new.begin();
		while (iter != ls_new.end()) { str[i++] = *iter++; }
		str[i] = '\0';

		if (!((obj / str) / str)._polinom.empty()) { delete[] str; return false; }
	}

	delete[] str;
	return true;
}

ostream& operator<<(ostream& out, const Polinom& obj)
{
	Polinom copy(obj);

	for (auto& iter : copy._polinom)
	{
		if (iter.getKef() > 0 && iter != obj._polinom.front()) { out << "+"; }
		out << iter;
	}

	return out;
}

istream& operator>>(istream& in, Polinom& obj)
{
	char* str = new char[SIZE];
	char symbol = in.get();
	int i = 0;

	while (symbol != '\n')
	{
		if (symbol != ' ') { str[i++] = symbol; }
		symbol = in.get();
	}
	str[i] = '\0';

	Polinom pol(str);
	obj = pol;

	delete[] str;
	return in;
}