#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <shared_mutex>
#include "SingelThreadAlphaBetaPro/Board.h"
#include "Spinlock.h"


namespace MCTS {
	static const double C = sqrt(2);
	static const int DRAW = -2;
	static const int Not_Player_win = -1;
	static const int BLACK_win = 0;
	static const int WHITE_win = 1;

	class Node {
	private:
		static unsigned long long cnt;
				
		std::shared_mutex Node_lock;
		SpinLock spinLock;

		std::atomic<unsigned long long> win_num;
		std::atomic<unsigned long long> game_num;
		//std::atomic<unsigned int> vitrual_loss;

		unsigned long long id;
		unsigned long long depth;

		Node* father;

	public:
		SingelThreadAlphaBetaPro::Board bd;

		std::vector<Node*> children;

		Node();
		Node(Node* ft, SingelThreadAlphaBetaPro::Board Bd, int Depth);

		~Node();

		bool is_leaf(bool lock = true);

		void expend();

		void back_ground(bool is_win);
		void is_Draw();

		double uct();

		Node* get_best_node();

		int who_win();

		void print();
	};

	static const int N = 10000;
	static const int thread_size = 10;
	static const int Sleep_time = 1000;

	static Node* root;

	void init();

	void simulate();
	void work(int x);
}