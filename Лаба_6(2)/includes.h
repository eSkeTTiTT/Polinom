#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#define SIZE 100

using namespace std;
class monom;
class Polinom;
bool uniformity(const Polinom& ptr);
bool harmony(const Polinom& obj);

class TeX_convertible
{
public:
	virtual string convert() const = 0;
};