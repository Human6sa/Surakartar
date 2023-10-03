#include "Node.h"

using namespace SingelThreadAlphaBetaPro;

Node::Node() : have_child(false), score(0) {
	
}

Node::Node(const Board& board) : bd(board), have_child(false), score(0) {}

Node::Node(int cp, int wn, int bn, int wb, int bd) : bd(cp, wn, bn, wb, bd), have_child(false), score(0) {}

Node::~Node() {
	for (auto& cm : children_move) {
		delete cm;
	}
	for (auto& ca : children_attack) {
		delete ca;
	}
}

int Node::get_score() {
	if (have_child) return score;
	have_child = true;
	int cp = bd.current_player;
	if (cp == WHITE) {
		if (bd.WHITE_num == 0) return INT_MIN;
		else if (bd.BLACK_num == 0) return INT_MAX;
	}
	else {
		if (bd.WHITE_num == 0) return INT_MAX;
		else if (bd.BLACK_num == 0) return INT_MIN;
	}
	std::vector<Board> attack, move;
	int value_white = 0, value_black = 0;
	value_white += Each_value * bd.WHITE_num;
	value_black += Each_value * bd.BLACK_num;
	for (int i = 0; i < Board_len; ++i) {
		for (int j = 0; j < Board_len; ++j) {
			if (check(bd.WHITE_board, i, j)) {
				value_white += (value[i][j] / 10);
				std::vector<Board> ac = bd.get_attack(i, j, WHITE);
				std::vector<Board> mc = bd.get_move(i, j, WHITE);
				value_white += (ac.size() * Attack_value + mc.size() * Move_value);
				if (cp == WHITE) {
					attack.insert(attack.end(), ac.begin(), ac.end());
					move.insert(move.end(), mc.begin(), mc.end());
				}
			}
			else if (check(bd.BLACK_board, i, j)) {
				value_black += (value[i][j] / 10);
				std::vector<Board> ac = bd.get_attack(i, j, BLACK);
				std::vector<Board> mc = bd.get_move(i, j, BLACK);
				value_black += (ac.size() * Attack_value + mc.size() * Move_value);
				if (cp == BLACK) {
					attack.insert(attack.end(), ac.begin(), ac.end());
					move.insert(move.end(), mc.begin(), mc.end());
				}
			}
		}
	}
	for (auto& ac : attack) {
		children_attack.push_back(new Node(ac));
	}
	for (auto& mc : move) {
		children_move.push_back(new Node(mc));
	}
	return cp == WHITE ? value_white - value_black : value_black - value_white;
}