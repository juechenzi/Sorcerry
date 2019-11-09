#include "card.h"
#include <iostream>


Card::Card(int owner,std::string name,std::shared_ptr<Game> game):owner{owner},name{name}, game{game}{
	if(this->name == "Banish"){
		type = "Spell";
		description = "Destroy target minion or ritual";
		cost = 2;
	} else if(this->name == "Unsummon"){
		type = "Spell";
		description = "Return target minion to its owner's hand";
		cost = 1;
	} else if(this->name == "Recharge"){
		type = "Spell";
		description = "Your ritual gains 3 charges";
		cost = 1;
	} else if(this->name == "Disenchant"){
		type = "Spell";
		description = "Destroy the top enchantment on target minion";
		cost = 1;
	} else if(this->name == "Raise Dead"){
		type = "Spell";
		description = "Resurrect the top minion in your graveyard and set its defence to 1";
		cost = 1;
	} else if(this->name == "Blizzard"){
		type = "Spell";
		description = "Deal 2 damage to all minions";
		cost = 3;
	} else if(this->name == "Air Elemental"){
		type = "Minion";
		cost = 0;
		attack = 1;
		defence = 1;
	} else if(this->name == "Earth Elemental"){
		type = "Minion";
		cost = 3;
		attack = 4;
		defence = 4;
	} else if(this->name == "Bone Golem"){
		type = "Minion";
		cost = 2;
		description = "Gain +1/+1 whenever a minion leaves play.";
		ability = true;
		attack = 1;
		defence = 3;
	} else if(this->name == "Fire Elemental"){
		type = "Minion";
		cost = 2;
		description = "Whenever an opponent's minion enters play, deal 1 damage to it.";
		ability = true;
		attack = 2;
		defence = 2;
	} else if(this->name == "Potion Seller"){
		type = "Minion";
		cost = 2;
		description = "At the end of your turn, all your minions gain +0/+1.";
		ability = true;
		attack = 1;
		defence = 3;
	} else if(this->name == "Novice Pyromancer"){
		type = "Minion";
		cost = 1;
		description = "Deal 1 damage to target minion";
		ability = true;
		ability_cost = 1;
		attack = 0;
		defence = 1;
	} else if(this->name == "Apprentice Summoner"){
		type = "Minion";
		cost = 1;
		description = "Summon a 1/1 air elemental";
		ability = true;
		ability_cost = 1;
		attack = 1;
		defence = 1;
	} else if(this->name == "Master Summoner"){
		type = "Minion";
		cost = 3;
		description = "Summon up to three 1/1 air elementals";
		ability = true;
		ability_cost = 2;
		attack = 2;
		defence = 3;
	} else if(this->name == "Giant Strength"){
		type = "Enchantment";
		cost = 1;
		defence = 2;
		attack =2;
	} else if(this->name == "Enrage"){
		type = "Enchantment";
		cost = 2;
		defence = -2;
		attack = -2;
	} else if(this->name == "Haste"){
		type = "Enchantment";
		cost = 1;
		description = "Enchanted minion gains +1 action each turn";
	} else if(this->name == "Magic Fatigue"){
		type = "Enchantment";
		cost = 0;
		description = "Enchanted minion's activated ability costs 2 more";
	} else if(this->name == "Silence"){
		type = "Enchantment";
		cost = 1;
		description = "Enchanted minion cannot use abilities";
	} else if(this->name == "Dark Ritual"){
		type = "Ritual";
		cost = 0;
		description = "At the start of your turn, gain 1 magic";
		ability = true;
		defence = 5;
		ability_cost = 1;
	} else if(this->name == "Aura of Power"){
		type = "Ritual";
		cost = 1;
		ability = true;
		ability_cost = 1;
		defence = 4;
		description = "Whenever a minion enters play under your control, it gains +1/+1";
	} else if(this->name == "Standstill"){
		type = "Ritual";
		cost = 3;
		ability = true;
		ability_cost = 2;
		defence = 4;
		description = "Whenever a minion enters play, destroy it";
	}
}



int Card::get_cost(){
	return cost;
}



int Card::get_attack(){
	int tmp = attack;
	int size = enl.size();
	if(size != 0){
		tmp = enl.back()->get_attack();
	}
	return tmp;
}




int Card::get_defence(){
	int size = enl.size();
	if(size != 0){
		return enl.back()->get_defence();
	} else {
		return defence;
	}
}



int Card::get_enatt(){
	return enatt;
}



int Card::get_endef(){
	return endef;
}



void Card::buff(int a,int b){
	int size = enl.size();
	if(size != 0){
		enl.back()->buff(a,b);
		attack += a;
	} else if(type == "Enchantment"){
		attack += a;
		defence += b;
		enatt += a;
		defence += b;
	} else {
		attack += a;
		defence += b;
	}
}	




bool Card::useab(){
	int size = enl.size();
	if(size != 0){
		for(int a = 0;a <size;++a){
			if(enl.at(a)->getname() == "Silence"){
				return false;
			}
		}
	}
	return ability;
}



std::string Card::getdes(){
	return description;
}



std::string Card::getname(){
	return name;
}



std::string Card::gettype(){
	return type;
}



int Card::getabc(){
	int size = enl.size();
	int tmp = ability_cost;
	if(size != 0){
		for(int a = 0; a< size;++a){
			if(enl.at(a)->getname() == "Magic Fatigue"){
				tmp += 2;
			}
		}
	}
	return tmp;
}




void Card::enter_play(int player,std::shared_ptr<Game> game){
	owner = player;
	this->game = game;
}


void Card::set_att_def(int a,int b){
	attack = a;
	defence = b;
}


void Card::enchant_minion(std::shared_ptr<Card> en){
	enl.push_back(en);
	int tmp1 = en->get_attack();
	int tmp2 = en->get_defence();
	int tmp3 = this->get_attack();
	int tmp4 = this->get_defence();
	if(tmp1 > 0){
		tmp3 += tmp1;
	} else if(tmp1 < 0){
		tmp3 *= tmp1;
		tmp3 *= -1;
	}
	if(tmp2 > 0){
		tmp4 += tmp2;
	} else if(tmp2 < 0){
		tmp4 *= tmp2;
		tmp4 *= -1;
	}
	en->set_att_def(tmp3,tmp4);
}



void Card::dischant_minion(){
	if(enl.size() > 1){
		int tmp1 = enl.back()->get_enatt();
		int tmp2 = enl.back()->get_endef();
		enl.at(enl.size()-2)->buff(tmp1,tmp2);
		enl.pop_back();
	} else if(enl.size() == 1){
		int tmp1 = enl.back()->get_enatt();
		int tmp2 = enl.back()->get_endef();
		buff(tmp1,tmp2);
		enl.pop_back();
	}
}


void Card::receive(int player,int b){
	if(b == 4){
		play = 1;
	}
	if(this->useab()){
		if(b == 1){
			if(name == "Fire Elemental"){
				if(player != owner){
					game->receive(player,3);
				}
			} else if(name == "Standstill"){
				if(player != owner){
					if(defence > ability_cost){
						defence -= ability_cost;
						game->receive(player, 7);
					}
				}
			} else if(name == "Aura of Power"){
				if(player == owner){
					if(defence > ability_cost){
						defence -= ability_cost;			
						game->receive(player,8);
					}
				}
			}
			
		} else if(b == 2){
			if(name == "Bone Golem"){
				buff(1,1);
			}
		} else if(b == 4){
			play = 1;
			std::cout<<"start of turn` set up is done"<<" "<<play<<std::endl;
			if(name == "Dark Ritual"){
				if(player == owner){
					if(defence > ability_cost){
						defence -= ability_cost;
						game->receive(player,9);
					}
				}
			} else {
				for(int c = 0;c < enl.size();++c){
					if(enl.at(c)->getname() == "Haste"){
						play++;
					}
				}
			}
		} else if(b == 5){
			std::cout<<"end of turn set up is done"<<std::endl;
			if(name == "Potion Seller"){
				game->receive(owner,6);
			}
		}
	}
}

void Card::set_play(int a){
	play = a;
}

void Card::change_play(int b){
	play += b;
}

int Card::get_play(){
	return play;
}

bool Card::need_target(){
	if(type == "Enchantment"){
		return true;
	}else if(type == "Spell"){
		return !(name == "Recharge" || name == "Raise Dead"|| name == "Blizzard");
	}
	return false;
}

void Card::inspect(){
	card_template_t tmp;
	if(description == ""){
		tmp = display_minion_no_ability(name,cost,attack,defence);
	} else if(ability_cost != 0){
		tmp = display_minion_activated_ability(name,cost,attack,defence,ability_cost,description);
	} else {
		tmp = display_minion_triggered_ability(name,cost,attack,defence,description);
	}
	for(int a = 0; a < 11; a++){
		std::cout<<tmp[a]<<std::endl;
	}
}

std::shared_ptr<Game> Card::return_game(){
	return game;
}





