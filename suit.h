#ifndef SUIT_H
#define SUIT_H

#include "card.h"
#include <string>
#include "display.h"

class Card;
class Suite : public std::enable_shared_from_this<Suite>{
protected:
	int si1 = 0;
	int si2 = 0;
	std::vector<std::shared_ptr<Card>> ply1;
	std::vector<std::shared_ptr<Card>> ply2;
	std::shared_ptr<Card> r1 = nullptr;
	std::shared_ptr<Card> r2 = nullptr;
	std::shared_ptr<Suite> t1;
	std::shared_ptr<Suite> t2;
	std::shared_ptr<Suite> t3;
public:
	Suite();
	int get_size(int a);
	void push(int a,std::shared_ptr<Card> c,int b = -1);
	void deathcheck(int b);
	std::shared_ptr<Card> getcard(int a, int b);
	void sett(std::shared_ptr<Suite> a,std::shared_ptr<Suite> b,std::shared_ptr<Suite> c);
	void ritualset(int a,std::shared_ptr<Card> r);
	void receive(int player, int b);
	void shuffle();
//type of b: 1. minion enter 2. minion leave; 3. deal damage to last opponent minion 4. start of turn 5. end of turn 6.add 1 defence to all +1 7. destroy the last opponent minion   8. +1/+1 buff yout last minion
	void draw(int a);
	void discard(int num,int player);
	void play_to(int player1, int who, int player2 = 0, int des=-1);
	void use_ab(int player1, int who, int player2 = 0, int des=-1);
};	


#endif

