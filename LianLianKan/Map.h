#pragma once

#include "Lightning.h"
#include <string.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Difficulty { easy, normal, hard };
enum MYDIRECTION { U = 1, D = 2, L = 4, R = 5 };

using namespace std;
class Map
{
public:
	Map();
	~Map();
	vector<vector<int>> connection(int x1, int y1, int x2, int y2, bool promptFlag = false);
	vector<vector<int>> makeMap(Difficulty level);
	vector<vector<int>> makeMap(int order);
	vector<int> getDirections(vector<vector<int>> route);
	vector<vector<int>> rearrange();
	vector<vector<int>> prompt();
private:
	bool isLineLinkable(int x1, int y1, int x2, int y2);
	vector<vector<int>> map;
	bool full_true(vector<bool> flags);
	int find_right_number(int alpha, int beta, int x = -1, int y = -1);
	int get_direction_lightning(int pre, int post);
	int get_direction_point(vector<int> vct);

};

