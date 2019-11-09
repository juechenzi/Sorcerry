#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "game.h"
#include "display.h"
class Game;
class Card : public std::enable_shared_from_this<Card>{
protected: 
	int owner; // owner can only be 1 or 2
	std::string type; // type can be ritual, enchantment, spell, minion
	std::string name;
	int cost;

	std::string description="";
	int attack = 0;
	int defence = 0;
	int play = 0;
	bool ability = false;
	int ability_cost = 0;
	int enatt = 0;
	int endef = 0;
	std::shared_ptr<Game> game=nullptr;
	std::vector<std::shared_ptr<Card>> enl;
public:
	Card(int owner,std::string name,std::shared_ptr<Game> game = nullptr);
	int get_cost();
	int get_attack();
	int get_defence();
	int get_enatt();
	int get_endef();
	void buff(int a,int b);
	bool useab();
	std::string getdes();
	std::string getname();
	std::string gettype();
	void set_att_def(int a, int b);
	int getabc();
	void enter_play(int player, std::shared_ptr<Game> game);
	void enchant_minion(std::shared_ptr<Card> en);
	void dischant_minion();
	void receive(int player, int b);//type of b: 1. minion enter 2. minion leave; 3. deal damage to last opponent minion 4. start of turn 5. end of turn 6.add 1 defence to all +1 7. destroy the last opponent minion   8. +1/+1 buff yout last minion
	void set_play(int a);
	void change_play(int b);
	int get_play();
	bool need_target();
	void inspect();
	std::shared_ptr<Game> return_game();

};
#endif

