#include <iostream>
#include <vector>
#include <fstream>

//функция, получающая на вход граф, и возвращающая флаг, полносвязный он или нет
void dfs(std::vector<std::vector<int>> m, int start, std::vector<bool>& visited, std::fstream& out) {

	std::cout << start << " ";
	visited[start] = true;
	out << start << " ";
	for (int i = 0; i < m[start].size(); i++) {

		if (m[start][i] == 1 && (!visited[i])) {
			dfs(m, i, visited, out);
		}
	}

}


std::vector<std::vector<int>> matrix_multiplication(std::vector<std::vector<int>>& m, std::vector<std::vector<int>>& m1) {

	int n = m.size();

	std::vector<std::vector<int>> result(n, std::vector<int>(n,0));

		for (int i = 0; i < result.size(); ++i) {
			for (int j = 0; j < result.size(); ++j) {
				for (int k = 0; k < result.size(); ++k) {
					result[i][j] += m[i][k] * m1[k][j];
				}
			}
		}

	return result;
}

std::vector<std::vector<int>> matrix_power(std::vector<std::vector<int>>& m, int power) {

	int n = m.size();
	std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		result[i][i] = 1;
	}

	while (power > 0) {
		if (power % 2 == 0) {
			m = matrix_multiplication(m, m);
			power /= 2;
		}
		else {
			result = matrix_multiplication(result, m);
			power--;
		}
	}

	return result;
}

bool is_fully_connected_edge_count(std::vector<std::vector<int>> graph) {

	int n = graph.size();

	int count = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (graph[i][j] == 1) {
				count++;
			}
		}
	}
	//((i != j && graph[i][j] == 1) && (i == j && graph[i][j] == 0))
	int needed = n * (n - 1) / 2;
	std::cout << "\n" << count << " " << needed << "\n";
	if (count == needed) {
		std::cout << "\nGraph is fully connected \n";
		return true;
	}
	std::cout << "Graph is not fully connected \n";
	return false;

}

bool is_fully_connected_matrix(std::vector<std::vector<int>>& graph) {

	int n = graph.size();
	std::vector<std::vector<int>> result = graph;

	result = matrix_power(result, n);

	std::cout << std::endl;
	std::cout << "Adjastency matrix in power n: \n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<int> diag;
	std::vector<int> other;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j) {
				diag.push_back(result[i][j]);
			}
			else {
				other.push_back(result[i][j]);
			}
		}
	}
	/*for (int i = 0; i < diag.size(); ++i) {
		std::cout << diag[i] << " ";
	}
	std::cout << " \n";

	for (int i = 0; i < other.size(); ++i) {
		std::cout << other[i] << " ";
	}
	std::cout << " \n";
	*/
	int left = 0;
	int right = diag.size() - 1;
	int right1 = other.size() - 1;

	while (left < right) {
		if (diag[left] == diag[right]) {
			left++;
		}
		else {
			std::cout << "Graph is not fully connected \n";
			return false;
			break;
		}
	}
	while (left < right1) {
		if (other[left] == other[right1]) {
			left++;
		}
		else {
			std::cout << "Graph is not fully connected \n";
			return false;
			break;
		}
	}
	std::cout << "\nGraph is fully connected \n";
	return true;
}



int main() {

	std::string path = "input.txt";
	std::ifstream fin;
	fin.open(path);
	int count = 0;


	if (!fin.is_open()) {
		std::cerr << "Error while opening file \n";
	}
	else {
		while (fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
			if (!fin.eof()) {
				count++;
			}
		}
	}

	fin.clear();
	fin.seekg(0, std::ios::beg);

	std::vector<std::vector<int>> adj_m(count, std::vector<int>(count));

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			fin >> adj_m[i][j];
		}
	}
	std::cout << "adjastency matrix: \n";
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			std::cout << adj_m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	int start_vertex = 0;
	fin >> start_vertex;
	std::cout << "\nStart vertex: " << start_vertex << '\n';
	if (start_vertex >= count) {
		std::cerr << "Unexisting vertex\n";
	}

	fin.close();

	std::vector<bool> visited(count, false);

	std::fstream out;
	std::string out_path = "output.txt";
	out.open(out_path);
	if (!out.is_open()) {
		std::cerr << "Error while opening file \n";
	}

	dfs(adj_m, start_vertex, visited, out);

	out.close();

	is_fully_connected_matrix(adj_m);
	is_fully_connected_edge_count(adj_m);

	return 0;
}