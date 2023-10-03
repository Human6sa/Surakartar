#pragma once

#include "Node.h"

namespace SingelThreadAlphaBetaPro {
	inline void set_zero(long long& Matrix, int& x, int& y);
	inline void set_one(long long& Matrix, int& x, int& y);

	inline void get_next_board(long long& WHITE_board, long long& BLACK_board, int& current_player, int& x1, int& x2, int& y1, int& y2, int& wn, int& bn);

	class Search {
	private:
		Node* root;
	public:
		Node* node;

		Search();

		void do_action(int x1, int y1, int x2, int y2);
	};
}