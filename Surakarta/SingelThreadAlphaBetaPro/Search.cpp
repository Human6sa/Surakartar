#include "Search.h"

using namespace SingelThreadAlphaBetaPro;

void SingelThreadAlphaBetaPro::set_zero(long long& Matrix, int& x, int& y) {
	Matrix &= (~(1 << (x * Board_len + y)));
}

void SingelThreadAlphaBetaPro::set_one(long long& Matrix, int& x, int& y) {
	Matrix |= (1 << (x * Board_len + y));
}

void SingelThreadAlphaBetaPro::get_next_board(long long& WHITE_board, long long& BLACK_board, int& current_player, int& x1, int& y1, int& x2, int& y2, int& wn, int& bn) {
	if (current_player == BLACK) {
		set_zero(BLACK_board, x1, y1);
		set_one(BLACK_board, x2, y2);
		if (check(WHITE_board, x2, y2)) {
			set_zero(WHITE_board, x2, y2);
			--wn;
		}
	}
	else {
		set_zero(WHITE_board, x1, y1);
		set_one(WHITE_board, x2, y2);
		if (check(BLACK_board, x2, y2)) {
			set_zero(BLACK_board, x2, y2);
			--bn;
		}
	}
}

Search::Search() {
	node = root = new Node();
}

void Search::do_action(int x1, int y1, int x2, int y2) {
	long long WHITE_board = this->node->bd.WHITE_board, BLACK_board = this->node->bd.BLACK_board;
	int current_player = this->node->bd.current_player, wn = this->node->bd.WHITE_num, bn = this->node->bd.BLACK_num;

	get_next_board(WHITE_board, BLACK_board, current_player, x1, y1, x2, y2, wn, bn);

	for (auto& ac : this->node->children_attack) {
		if ((ac->bd.WHITE_board = WHITE_board) && (ac->bd.BLACK_board == BLACK_board)) {
			node = ac;
			return;
		}
	}
	for (auto& mc : this->node->children_move) {
		if ((mc->bd.WHITE_board = WHITE_board) && (mc->bd.BLACK_board == BLACK_board)) {
			node = mc;
			return;
		}
	}

	puts("create new Board");

	node = new Node(current_player ^ 1, wn, bn, WHITE_board, BLACK_board);
}