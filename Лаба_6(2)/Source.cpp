#include "includes.h"
#include "implementation.h"

int add_string(ifstream& fin, char symbol, char* str, int str_size)
{
	while (symbol != '}') {
		if (symbol != ' ')
			str[str_size++] = symbol;

		symbol = fin.get();
	}

	str[str_size] = '\0';
	return str_size;
}

int search_index_operations(string& operation, vector<string>& operations) // ищет индекс операции в массиве
{
	auto iter = operations.begin();
	int index = 0;
	while (iter != operations.end())
	{
		if (*iter == operation) { return index; }
		iter++; index++;
	}

	return -1;
}

void realization_operations(ofstream& out, Polinom& p1, Polinom& p2, vector<string>& operations, string& operation) // реализация операции и запись в файл
{
	Polinom res;
	char* str = new char[operation.size() + 1];
	bool check = false;
	int index = search_index_operations(operation, operations);
	int i = 0;

	switch (index)
	{
	case 0:
		res = p1 + p2;
		break;
	case 1:
		res = p1 - p2;
		break;
	case 2:
		res = p1 * p2;
		break;
	case 3:
		check = p1 == p2;
		break;
	case 4:
		check = !(p1 == p2);
		break;
	case 5:
		check = uniformity(p1);
		break;
	case 6:
		check = harmony(p1);
		break;
	default: // если операция со списком
		operation.erase(operation.begin());
		while (operation[i] != '\0') { str[i] = operation[i]; i++; }
		str[i] = '\0';
		res = p1 / str;
		break;
	}

	if (index >= 0 && index <= 2) { out << p1.convert() << " $" << operation << "$ " << p2.convert() << " $=$ " << res.convert() << "\\\\\n"; }
	else if (index == 3 || index == 4) { out << p1.convert() << " $" << operation << "$ " << p2.convert() << " $=$ " << check << "\\\\\n"; }
	else if (index == 5 || index == 6) { out << "$" << operation << "$ " << p1.convert() << " $=$ " << check << "\\\\\n"; }
	else { out << p1.convert() << "$/" << str << "$ $=$ " << res.convert() << "\\\\\n"; }

	delete[] str;
}

int main(int argc, char* argv[])
{
	ifstream fin(argv[1]);

	ofstream cerrFile("CERR_LOG.txt");
	cerr.rdbuf(cerrFile.rdbuf());

	ofstream texFile("TeX.tex");
	texFile << "\\usepackage{mathtext}\nbegin{document}\n";

	char symbol;
	vector<string> operations = { "+", "-", "*", "==", "!=", "uniformity", "harmony" };
	string operation;
	char* polinom_1;
	char* polinom_2;
	int i = 0, j = 0, index = 0;
	operation.reserve(5);
	polinom_1 = new char[SIZE];
	polinom_2 = new char[SIZE];
	polinom_1[0] = polinom_2[0] = '\0';

	if (fin.is_open())
	{
		symbol = fin.get();
		while (!fin.eof())
		{
			if (symbol == ' ')
			{
				symbol = fin.get();
				continue;
			}
			else if (i == 0 && symbol == '{') {
				symbol = fin.get(); // пропускаем скобку
				i = add_string(fin, symbol, polinom_1, i);
				symbol = fin.get(); // пропускаем }
			}
			else if (j == 0 && symbol == '{') {
				symbol = fin.get();
				j = add_string(fin, symbol, polinom_2, j);
				symbol = fin.get(); // пропускаем }
			}
			else // операция
			{
				while (symbol != ' ' && symbol != '{')
				{
					operation.push_back(symbol);
					symbol = fin.get();
				}
			}

			if ((symbol == '\n' || symbol == EOF) && (i != 0 || j != 0))
			{
				try
				{
					Polinom p1(polinom_1);
					Polinom p2(polinom_2);
					realization_operations(texFile, p1, p2, operations, operation);
				}
				catch (MyException& ex)
				{
					cerr << ex.what() << " Code: " << ex.code() << endl;
				}

				i = j = 0;
				polinom_1[i] = polinom_2[j] = '\0';
				operation.clear();
				if (symbol != EOF) { symbol = fin.get(); }
			}
			else if (symbol == '\n') { symbol = fin.get(); }
			
		}
	}
	else { cerr << "File " << argv[1] << " not exist!!!\n"; }

	fin.close();
	cerrFile.close();
	texFile << "end{document}";
	texFile.close();
	delete[] polinom_1;
	delete[] polinom_2;

	return 0;
}