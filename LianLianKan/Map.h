#pragma once

#include <string.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Map
{
public:
	Map();
	~Map();
	vector<vector<int>> connection(int x1, int y1, int x2, int y2);
	vector<vector<int>> makeMap();
	vector<vector<int>> rearrange();
	vector<vector<int>> prompt();
private:
	bool isLineLinkable(int x1, int y1, int x2, int y2);
	vector<vector<int>> map;
	bool full_true(vector<bool> flags);
	void recover(int & x1, int & y1, int & x2, int & y2, int tmp_x1, int tmp_y1, int tmp_x2, int tmp_y2);
};

