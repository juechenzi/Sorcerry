#ifndef GAME_H
#define GAME_H

#include "suit.h"
#include "display.h"

class Suite;
class Game{
	public:
		std::string player1;
		std::string player2;
		int current=1;
		int health_one=20;
		int magic_one = 3;
		int health_two=20;
		int magic_two = 3;
		std::shared_ptr<Suite> deck;
		std::shared_ptr<Suite> hand;
		std::shared_ptr<Suite> board;
		std::shared_ptr<Suite> grave;

	public:
	        Game();

      		void winning();
		void set_turn(int a);
		void set_player(std::string a);
		void set_magic(int a);
		void gainMagic(int a);
		void attack_p(int i);
		void attack_m(int i,int m);
		void draw();
		void discard(int i);
		void play(int i);
		void play_to(int i,int player,std::string card);
		void inspect(int i);
		void handp();
		void boardp();
		void start();
		void end();
		void useAb(int i);
		void useAb_to(int i,int player, std::string card);
		void shuffle();
		void init_deck(std::string name,std::shared_ptr<Game> g);
		void graphics();
		void receive(int player,int b);
};
#endif



