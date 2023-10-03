#pragma once

#include <vector>
#include "Board.h"

namespace SingelThreadAlphaBetaPro {
	static const std::vector<std::vector<int>> value = {
		{ 50, 105, 105, 105, 105, 50 },
		{105, 125, 155, 155, 125, 105},
		{105, 155, 125, 125, 155, 105},
		{105, 155, 125, 125, 155, 105},
		{105, 125, 155, 155, 125, 105},
		{ 50, 105, 105, 105, 105, 50 }
	};

	static const int Each_value = 150;
	static const int Attack_value = 20;
	static const int Move_value = 10;

	class Node {
	private:
		int score;
		bool have_child;
	public:
		Board bd;
		std::vector<Node*> children_move, children_attack;

		Node();
		Node(const Board& board);
		Node(int cp, int wn, int bn, int wb, int bd);

		~Node();

		int get_score();
	};
}