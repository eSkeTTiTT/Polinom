#include "includes.h"

class monom : TeX_convertible
{
private:
	map<char, int> _monom;
	int kef;
	
public:
	friend bool similarity(const monom& obj, const monom& other); // проверка на подобие мономов
	friend bool uniformity(const Polinom& obj); // однородность
	friend bool harmony(Polinom& obj);
	friend ostream& operator<<(ostream& out, const monom& obj);
	friend istream& operator>>(istream& out, monom& obj);

	monom();
	monom(char* str);
	monom(string str);
	int getKef() const; // получить коэффициент
	void setKef(int value); // установить коэффициент
	monom(const monom& obj); // копирование 
	monom operator+(const monom& obj);
	monom& operator+=(const monom& obj);
	monom operator-(const monom& obj);
	monom& operator-=(const monom& obj);
	monom operator*(const monom& obj);
	monom& operator*=(const monom& obj);
	monom operator/(char symbol);
	monom& operator/=(char symbol);
	bool operator==(const monom& obj);
	bool operator!=(const monom& obj);

	~monom();
	string convert() const override;
};

bool similarity(const monom& obj, const monom& other)
{
	map<char, int>::const_iterator iter_this = obj._monom.begin(), iter_other = other._monom.begin();
	if (obj._monom.size() != other._monom.size()) { return false; } // не равны размеры

	while (iter_this != obj._monom.end())
	{
		if (((iter_other = other._monom.find(iter_this->first)) == other._monom.end()) || (iter_this->second != iter_other->second))
			return false;
		else
			iter_this++;
	}

	return true;
}

ostream& operator<<(ostream& out, const monom& obj)
{
	if (obj.kef == 0) { return out; }
	else if (obj.kef == -1) { out << '-'; }
	else if (obj.kef != 1) { out << obj.kef; }

	map<char, int>::const_iterator iter = obj._monom.begin();
	while (iter != obj._monom.end())
	{
		if (iter->second != 0)
		{
			out << iter->first;
			if (iter->second != 1) { out << "^" << iter->second; }
		}

		iter++;
	}

	return out;
}

istream& operator>>(istream& in, monom& obj)
{
	string str;
	getline(in, str);
	monom new_obj(str);
	obj = new_obj;

	return in;
}
