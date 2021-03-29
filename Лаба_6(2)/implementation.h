#include "includes.h"
#include "monom.h"
#include "Polinom.h"
#include "MyExceptions.h"

// MONOM
monom::monom()
{
	kef = 0;
}

monom::monom(char* str)
{
	string str_new; // коэф и степень
	str_new.reserve(20);
	char* ptr = str;
	char symbol;

	while (*ptr < 'a' || *ptr > 'z') { str_new.push_back(*ptr++); }
	if (!str_new.empty() && str_new != "+")
	{
		if (str_new == "-") { kef = -1; }
		else { kef = stoi(str_new); }
	}
	else kef = 1;
	str_new.resize(0);

	while (*ptr != '\0')
	{
		symbol = *ptr++;
		if (*ptr == '^')
		{
			ptr++;
			if (*ptr == '-') { throw MyException("Negative degree", 228); } // отрицательная степень

			while (*ptr != '\0' && (*ptr >= '0' && *ptr <= '9')) { str_new.push_back(*ptr++); }

			if (stoi(str_new) != 0)
				_monom.insert(pair<char, int>(symbol, stoi(str_new)));
			str_new.resize(0);
		}
		else { _monom.insert(pair<char, int>(symbol, 1)); }
	}
}

monom::monom(string str)
{
	string str_new; 
	str_new.reserve(20);
	string::iterator iter = str.begin();
	char symbol;

	while (*iter < 'a' || *iter > 'z') { str_new.push_back(*iter++); }
	if (!str_new.empty() && str_new != "+")
	{
		if (str_new == "-") { kef = -1; }
		else { kef = stoi(str_new); }
	}
	else kef = 1;
	str_new.resize(0);

	while (iter != str.end())
	{
		symbol = *iter++;
		if (iter != str.end() && *iter == '^')
		{
			iter++;
			while (iter != str.end() && (*iter >= '0' && *iter <= '9')) { str_new.push_back(*iter++); }

			if (stoi(str_new) != 0)
				_monom.insert(pair<char, int>(symbol, stoi(str_new)));
			str_new.resize(0);
		}
		else { _monom.insert(pair<char, int>(symbol, 1)); }
	}
}

int monom::getKef() const
{
	return this->kef;
}

void monom::setKef(int value)
{
	kef = value;
}

monom::monom(const monom& obj) // копирование
{
	kef = obj.kef;
	_monom = obj._monom;
}

monom monom::operator+(const monom& obj)
{
	return monom(*this) += obj;
}

monom& monom::operator+=(const monom& obj)
{
	if (similarity(*this, obj))
	{
		this->kef += obj.kef;
		return *this;
	}
	else { throw MyException("Subtraction of not similarity monoms", 322); }
}

monom monom::operator-(const monom& obj)
{
	return monom(*this) -= obj;
}

monom& monom::operator-=(const monom& obj)
{
	if (similarity(*this, obj))
	{
		this->kef -= obj.kef;
		return *this;
	}
	else { throw MyException("Addition of not similarity monoms", 322); }
}

monom monom::operator*(const monom& obj)
{
	return monom(*this) *= obj;
}

monom& monom::operator*=(const monom& obj)
{
	map<char, int>::const_iterator iter_1 = this->_monom.begin(), iter_2 = obj._monom.begin();
	this->kef *= obj.kef;

	while (iter_2 != obj._monom.end())
	{
		if ((iter_1 = this->_monom.find(iter_2->first)) == this->_monom.end())
			this->_monom.insert(pair<char, int>(iter_2->first, iter_2->second));
		else
			this->_monom[iter_1->first] = iter_1->second + iter_2->second;

		iter_2++;
	}

	return *this;
}

monom monom::operator/(char symbol)
{
	return monom(*this) /= symbol;
}

monom& monom::operator/=(char symbol)
{
	auto iter = this->_monom.begin();

	iter = this->_monom.find(symbol);
	if (iter == this->_monom.end())
	{
		this->_monom.clear();
		this->kef = 0;
	}
	else
	{
		this->kef *= iter->second--;
		if (iter->second <= 0) { this->_monom.erase(iter->first); }
	}

	return *this;
}

bool monom::operator==(const monom& obj)
{
	if (similarity(*this, obj) && (this->kef == obj.kef)) { return true; }
	else { return false; }
}

bool monom::operator!=(const monom& obj)
{
	return !(*this == obj);
}

monom::~monom()
{
	this->_monom.clear();
}

string monom::convert() const
{
	string str;
	str.reserve(SIZE);

	if (kef == -1) { str.push_back('-'); }
	else if (kef != 1) { str += to_string(kef); }

	auto iter = _monom.begin();
	while (iter != _monom.end())
	{
		if (iter->second != 0)
		{
			str.push_back(iter->first);
			if (iter->second != 1) { str.push_back('^'); str += to_string(iter->second); }
		}

		iter++;
	}

	return str;
}

// POLINOM
Polinom::Polinom()
{
}

Polinom::Polinom(char* str)
{
	string str_new;
	str_new.reserve(sizeof(str));

	char* ptr = str;

	while (*ptr != '\0')
	{
		do { str_new.push_back(*ptr++); } 		
		while (*ptr != '+' && *ptr != '-' && *ptr != '\0');

		if (*ptr == '-' && *(ptr - 1) == '^') { throw MyException("Negative degree", str_new, 228); }

		monom mandem(str_new);
		
		if (mandem.getKef() != 0)
		{
			list<monom>::iterator iter = _polinom.begin();
			while (iter != _polinom.end())
			{
				if (similarity(mandem, *iter)) { *iter += mandem; break; }
				iter++;
			}

			if (iter == _polinom.end()) { _polinom.push_back(mandem); }
			else if (iter->getKef() == 0) { _polinom.erase(iter); } // если подобные дали кеф 0
		}

		str_new.resize(0);
	}
}

Polinom::Polinom(const Polinom& obj)
{
	this->_polinom = obj._polinom;
}

Polinom Polinom::operator+(const Polinom& obj)
{
	return Polinom(*this) += obj;
}

Polinom& Polinom::operator+=(const Polinom& obj)
{
	bool flag = false;

	for (auto& iter2 : obj._polinom)
	{
		for (auto& iter1 : this->_polinom)
		{
			if (similarity(iter1, iter2)) 
			{ 
				if (iter1.getKef() + iter2.getKef() != 0)
				{
					iter1 += iter2;
					flag = true;
				}

				break; 
			}
		}

		if (!flag) { this->_polinom.push_back(iter2); }
		flag = false;
	}

	return *this;
}

Polinom Polinom::operator-(const Polinom& obj)
{
	return Polinom(*this) -= obj;
}

Polinom& Polinom::operator-=(const Polinom& obj)
{
	bool flag = false;

	for (auto& iter2 : obj._polinom)
	{
		for (auto& iter1 : this->_polinom)
		{
			if (similarity(iter1, iter2))
			{
				if (iter1.getKef() - iter2.getKef() != 0)
				{
					iter1 -= iter2;
					flag = true;
				}

				break;
			}
		}

		if (!flag) 
		{
			monom copy(iter2);
			copy.setKef(-1 * copy.getKef());
			this->_polinom.push_back(copy); 
		}
		flag = false;
	}

	return *this;
}

Polinom Polinom::operator*(const Polinom& obj)
{
	return Polinom(*this) *= obj;
}

Polinom& Polinom::operator*=(const Polinom& obj)
{
	Polinom copy(*this);
	monom res;
	this->_polinom.clear();

	for (auto& iter1 : copy._polinom)
	{
		for (auto& iter2 : obj._polinom)
		{
			res = iter1 * iter2;
			auto iter3 = this->_polinom.begin();
			while (iter3 != this->_polinom.end())
			{
				if (similarity(res,*iter3))
				{
					*iter3 += res;
					if (iter3->getKef() == 0) { this->_polinom.erase(iter3); }
					break;
				}
				iter3++;
			}

			if (iter3 == this->_polinom.end()) { this->_polinom.push_back(res); }
		}
	}

	return *this;
}

Polinom Polinom::operator/(char* arr)
{
	return Polinom(*this) /= arr;
}

Polinom& Polinom::operator/=(char* str)
{
	if (this->_polinom.empty()) { return *this; } // если пустой

	char* ptr = str;
	Polinom copy(*this);
	monom res;
	this->_polinom.clear();
	while (*ptr != '\0')
	{
		for (auto& iter : copy._polinom) 
		{
			res = iter / *ptr;
			if (res.getKef() != 0) { this->_polinom.push_back(res); }
		}
		ptr++;
	}

	return *this;
}

bool Polinom::operator==(const Polinom& obj)
{
	bool flag = false;

	if (this->_polinom.size() == obj._polinom.size())
	{
		for (auto& iter1 : this->_polinom)
		{
			for (auto& iter2 : obj._polinom)
			{
				if (iter1 == iter2) { flag = true; break; }
			}

			if (!flag) { return false; }
		}
	}
	else { return false; }

	return true;
}

bool Polinom::operator!=(const Polinom& obj)
{
	return !(*this == obj);
}

Polinom::~Polinom()
{
	this->_polinom.clear();
}

string Polinom::convert() const
{
	if (_polinom.empty()) { return "0"; }

	string str;
	str.reserve(SIZE);

	str += "$($\\( ";

	auto iter = _polinom.begin();
	while (iter != _polinom.end())
	{
		if (iter->getKef() >= 0 && iter != _polinom.begin()) { str.push_back('+'); }
		str += iter->convert();
		iter++;
	}

	str += " \\)$)$";
	return str;
}

//MyException
MyException::MyException(const char* msg, int value)
{
	this->_dataState = value;
	GetString(msg);
}

MyException::MyException(const char* msg, string str, int value)
{
	this->_dataState = value;
	GetString(msg);
	this->_message += ": " + str;
}

int MyException::code()
{
	return this->_dataState;
}

string MyException::what()
{
	return this->_message;
}

void MyException::GetString(const char* msg)
{
	int i = 0;
	this->_message.reserve(sizeof(msg));
	while (msg[i] != '\0')
		this->_message.push_back(msg[i++]);
}