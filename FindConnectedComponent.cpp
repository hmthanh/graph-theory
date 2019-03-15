#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>

using namespace std;

#define MAX 100

inline bool compList(const list<int>& a, const list<int>& b) {
	if (a.size() < b.size()) {
		return true;
	}
	else
	{
		if (a.size() > b.size()) {
			return false;
		}
		//a.size == b.size
		list<int>::const_iterator j = b.begin();
		for (list<int>::const_iterator i = a.begin(); i != a.end(); i++)
		{
			if ((*i) > (*j)) {
				return false;
			}
		}

		return true;
	}
}

class Graph {
	int size;
	list<int>* adjacency;

	list<int> DepthFirstSearch(int vertex, vector<bool> &visited, list<int>& connComp);
	list<int> BreadthFirstSearch(int vertex, vector<bool> &visited);
public :
	void Init(int size);
	void addEdge(int start, int end);
	void PrintAdjacency();
	list<list<int>> connectedComponents(char option);
	list<list<int>> connectedComponentsBFS();
	list<list<int>> connectedComponentsDFS();
};

inline void Graph::Init(int size)
{
	this->size = size;
	adjacency = new list<int>[size];
}

inline void Graph::addEdge(int start, int end) {
	adjacency[end].push_back(start);
}


inline list<int> Graph::BreadthFirstSearch(int vertex, vector<bool> &visited) {
	list<int> connComp = list<int>();
	visited[vertex] = true;

	list<int> queue;
	queue.push_back(vertex);

	list<int>::iterator i;
	while (!queue.empty())
	{
		vertex = queue.front();
		connComp.push_back(vertex);
		queue.pop_front();

		for (i = this->adjacency[vertex].begin(); i != this->adjacency[vertex].end(); ++i)
		{
			if (!visited[*i])
			{
				visited[*i] = true;
				queue.push_back(*i);
			}
		}
	}
	connComp.sort();
	return connComp;
}

inline list<int> Graph::DepthFirstSearch(int vertex, vector<bool>& visited, list<int>& connComp) {
	visited[vertex] = true;

	connComp.push_back(vertex);

	list<int>::iterator i;
	for (i = this->adjacency[vertex].begin(); i != this->adjacency[vertex].end(); i++)
	{
		if (!visited[*i]) {
			DepthFirstSearch(*i, visited, connComp);
		}
	}

	return connComp;
}

inline list<list<int>> Graph::connectedComponents(char option) {
	vector<bool> visited = vector<bool>(size);
	list<list<int>> connectComp = list<list<int>>();
	for (int i = 0; i < this->size; i++)
	{
		visited[i] = false;
	}

	for (int i = 0; i < this->size; i++)
	{
		if (visited[i] == false) {
			list<int> connComp;
			if (option == 'd') {
				list<int> temp = list<int>();
				connComp = this->DepthFirstSearch(i, visited, temp);
			}
			else if (option == 'b')
			{
				connComp = this->BreadthFirstSearch(i, visited);
			}
			connectComp.push_back(connComp);
		}
	}

	connectComp.sort(compList);
	return connectComp;
}

inline list<list<int>> Graph::connectedComponentsBFS()
{
	char option = 'b';
	return this->connectedComponents(option);
}

inline list<list<int>> Graph::connectedComponentsDFS()
{
	char option = 'd';
	return this->connectedComponents(option);
}

Graph ImportFile(string inputFile)
{
	Graph graph;
	ifstream file(inputFile);

	int size = 0;
	file >> size;
	graph.Init(size);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int temp;
			file >> temp;
			if (temp == 1) {
				graph.addEdge(i, j);
			}
		}
	}

	file.close();

	return graph;
}

void ExportFile(string output, list<list<int>> connComp) {
	ofstream file(output);

	cout << connComp.size() << " " << endl;
	file << connComp.size() << endl;

	for (list<list<int>>::iterator i = connComp.begin(); i != connComp.end(); i++)
	{
		list<int> temp = (*i);
		for (list<int>::iterator j = temp.begin(); j != temp.end(); ++j) {
			file << (*j) << " ";
			cout << (*j) << " ";
		}

		file << endl;
		cout << endl;
	}
	
	file.close();
}
void Graph::PrintAdjacency() {
	for (int i = 0; i < this->size; i++)
	{
		list<int> v = this->adjacency[i];
		cout << " [" << i << "]: ";
		for (list<int>::iterator j = v.begin(); j != v.end(); j++)
		{
			cout << (*j) << " ";
		}
		cout << endl;
	}
}
void Run() {
	string inputFile = "input.txt";
	string outputFile = "output.txt";
	char s = 'b';
	Graph graph = ImportFile(inputFile);
	graph.PrintAdjacency();
	cout << "********************" << endl;
	list<list<int>> conComp = graph.connectedComponents(s);
	ExportFile(outputFile, conComp);
}

int main(int argc, char* argv[])
{
	/*Run();
	return 0;*/
	if (argc == 4) {
		char option = (*argv[1]);
		const char* inputFile = argv[2];
		const char* outputFile = argv[3];

		cout << "Input file name : " << inputFile << endl;
		cout << "Output file name : " << outputFile << endl << endl;
		Graph graph = ImportFile(inputFile);
		graph.PrintAdjacency();
		cout << "********************" << endl;
		list<list<int>> conComp;
		if (option == 'b') {
			cout << "Find connected component by Breadth First Search " << endl;
			conComp = graph.connectedComponentsBFS();
		}
		else if (option == 'd') {
			cout << "Find connected component by Depth First Search " << endl;
			conComp = graph.connectedComponentsDFS();
		}
		
		ExportFile(outputFile, conComp);
		cout << endl << "Export successful to " << outputFile << " " << endl;
	}
	else {
		cout << "Wrong commandline parameters !";
		cin.get();
	}

	return 0;
}