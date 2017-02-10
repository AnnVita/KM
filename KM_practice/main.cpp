#include <iostream>
#include <fstream>
#include <sstream>
//#include <string> В Visual Studio следует раскомментировать
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

struct Group
{
    size_t groupSize;
    size_t count;
};

void GetArgumentsFromStream(stringstream & file) {}

template <typename T, typename... Targs>
void GetArgumentsFromStream(stringstream & stream, T & arg, Targs&... args)
{
	stream >> arg;
	GetArgumentsFromStream(stream, args...);
}

template <typename... Targs>
void GetArgumentsFromFirstFileLine(ifstream & file, Targs&... args)
{
	stringstream stream;
	string line;
	getline(file, line);
	stream << line;
	GetArgumentsFromStream(stream, args...);
}

template <typename T>
void ReadLineFromFileToVector(ifstream & file, vector<T> & vect)
{
	stringstream stream;
	string line;
	getline(file, line);
	stream << line;
	T elem;
	while (stream >> elem)
	{
		vect.push_back(elem);
	}
}

void ReadLineFromFileToBoolVector(ifstream & file, vector<bool> & vect, char ifTrue, char ifFalse)
{
	stringstream stream;
	string line;
	getline(file, line);
	stream << line;
	char elem;
	while (stream >> elem)
	{
		vect.push_back(elem == ifTrue);
	}
}

template<typename T>
void PrintVector(const vector<T> & vect)
{
	copy(vect.begin(), vect.end(), ostream_iterator<T>(cout, " "));
}

template<typename T>
void PrintMatrix(const vector<vector<T>> & boolMatrix)
{
	for (size_t i = 0; i < boolMatrix.size(); ++i)
	{
		PrintVector(boolMatrix[i]);
		cout << "\n";
	}
}

void FindConnection(vector<vector<bool>> & matrix, size_t y, vector<size_t> & connection, vector<size_t> & connectionSizes, size_t & yToStop)
{
	for (size_t x = 0; x < matrix[y].size(); ++x)
	{
		if (matrix[y][x])
		{
            if (yToStop == SIZE_MAX)
            {
                yToStop = y;
            }
			connection.push_back(y);
            matrix[y][x] = false;
            if (x == yToStop)
            {
                yToStop = SIZE_MAX;
                connectionSizes.push_back(connection.size());
                PrintVector(connection);
                cout << "\n";
                connection = vector<size_t>();
                break;
            }
			FindConnection(matrix, x, connection, connectionSizes, yToStop);
			break;
		}
	}
}

int main()
{
	size_t matrix0Y = 0;
	size_t matrix1Y = 0;
	ifstream input0("input0.txt");
	ifstream input1("input1.txt");

	GetArgumentsFromFirstFileLine(input0, matrix0Y);
	GetArgumentsFromFirstFileLine(input1, matrix1Y);

	vector<vector<bool>> matrix0(matrix0Y);
	for (size_t i = 0; i < matrix0Y; ++i)
	{
		ReadLineFromFileToBoolVector(input0, matrix0[i], '1', '0');
	}

	vector<size_t> connection0;
    vector<size_t> connectionSizes0;

    size_t yToStop = SIZE_MAX;
	for (size_t y = 0; y < matrix0.size(); ++y)
	{
		FindConnection(matrix0, y, connection0, connectionSizes0, yToStop);
	}

    sort(connectionSizes0.begin(), connectionSizes0.end());


    vector<vector<bool>> matrix1(matrix1Y);
    for (size_t i = 0; i < matrix1Y; ++i)
    {
        ReadLineFromFileToBoolVector(input1, matrix1[i], '1', '0');
    }

    vector<size_t> connection1;
    vector<size_t> connectionSizes1;

    yToStop = SIZE_MAX;
    for (size_t y = 0; y < matrix1.size(); ++y)
    {
        FindConnection(matrix1, y, connection1, connectionSizes1, yToStop);
    }

    sort(connectionSizes1.begin(), connectionSizes1.end());


    PrintVector(connectionSizes0);
    cout << "\n";
    PrintVector(connectionSizes1);
    cout << "\n";

    cout << ((connectionSizes0 == connectionSizes1) ? "Изоморфны" : "Не изоморфны");
    cout << "\n";

	return 0;
}