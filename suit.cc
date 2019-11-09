#include "suit.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
Suite::Suite(){
	si1 = 0;
	si2 = 0;
	ply1.empty();
	ply2.empty();
}

void Suite::push(int a, std::shared_ptr<Card> c,int b){
	if(a == 1){
		if(b == -1){
			++si1;
			ply1.push_back(c);
//			std::cout<<"Trying to play a minion"<<std::endl;
		} else {
			r1 = c;
		}
//		std::cout<<c->getname()<<"  "<<ply1.back()->getname()<<std::endl;
	}else{
		if(b == -1){
			++si2;
			ply2.push_back(c);
		} else {
			r2 =c;
		}
	}
}

void Suite::deathcheck(int b){
	if(b == 1){
		for(int a = 0; a <si1;++a){
			if(ply1.at(a)->get_defence() == 0){
				t3->push(1,ply1.at(a));
				ply1.erase(ply1.begin()+a);
				--si1;
				receive(b,2);
			}
		}
	} else {
		for(int a = 0; a <si2;++a){
			if(ply2.at(a)->get_defence() == 0){
				t3->push(2,ply2.at(a));
				ply2.erase(ply2.begin()+a);
				--si2;
				receive(b,2);
			}
		}
	}
}

std::shared_ptr<Card> Suite::getcard(int a, int b){
//	std::cout<<"Getcard is called"<<" "<<a<<" "<<b<<std::endl;
	if(b >= 0){
		if(a == 1){
//			std::cout<<ply1.at(b)->getname()<<std::endl;
			return ply1.at(b);
		}else {
			return ply2.at(b);
		}
	} else {
//		std::cout<<"Trying to get a ritual"<<std::endl;
		if(a == 1){
			return r1;
		} else {
			return r2;
		}
	}
}


void Suite::sett(std::shared_ptr<Suite> a,std::shared_ptr<Suite> b,std::shared_ptr<Suite> c){
		t1 = a;
		t2 = b;
		t3 = c;
	
}

void Suite::ritualset(int a, std::shared_ptr<Card> r){
	if(a == 1){
		r1 = r;
	} else {
		r2 = r;
	}
}

int Suite::get_size(int a){
	if(a == 1){
		return ply1.size();
	}else {
		return ply2.size();
	}
}

void Suite::receive(int player,int b){
	if(b == 3){
		if(player == 1){
			if(ply1.back()){
				ply1.back()->buff(0,-1);
			}
		} else {
			if(ply2.back()){
				ply2.back()->buff(0,-1);
			}
		}
	} else if(b == 6){
		if(player == 1){
			for(int a = 0 ; a < ply1.size(); ++a){
				ply1.at(a)->buff(0,1);
			}
		}else {
			for(int a = 0 ; a < ply2.size(); ++a){
				ply2.at(a)->buff(0,1);
			}
		}
	} else if(b == 7){
		if(player == 1){
			if(ply1.size()> 0){
				t3->push(player,ply1.back());
				ply1.pop_back();
			}
		}else {
			if(ply2.size() > 0){
				t3->push(player,ply2.back());
				ply2.pop_back();
			}
		}
	} else if(b == 8){
		if(player == 1){
			if(ply1.back()){
				ply1.back()->buff(1,1);
			}
		}else {
			if(ply2.back()){
				ply2.back()->buff(1,1);
			}
		}
	} else if(b == 9){
		if(player == 1){
			for(int a = 0;a < ply1.size();++a){
			ply1.at(a)->change_play(1);
			}
		} else {
			for(int a = 0;a<ply2.size();++a){
			ply2.at(a)->change_play(1);
			}
		}	
	} else {
//need to set the action to 1 at the start of the game
//		std::cout<<"Condition message "<<b<<" is delivered to all subminions"<<std::endl;
		if(player == 1){
			if(r1){
				r1->receive(player,b);
			}
			for(int a = 0; a < ply1.size();++a){
				ply1.at(a)->receive(player,b);
			}
		} 
		if(r2){
			r2->receive(player,b);
		}
		for(int a = 0; a < ply2.size();++a){
			ply2.at(a)->receive(player,b);
		}
		if(player == 2){
			if(r1){
				r1->receive(player,b);
			}
			for(int a = 0; a < ply1.size();++a){
				ply1.at(a)->receive(player,b);
			}
		}
	}
}

void Suite::draw(int a){
	if(a == 1){
		t1->push(1,ply1.back());
//		std::cout<<ply1.back()->getname()<<std::endl;
//		std::cout<<t1->getcard(0,1)->getname()<<std::endl;
		ply1.pop_back();
		--si1;
	} else {
		t1->push(2,ply2.back());
		ply2.pop_back();
		--si2;
	}
}

void Suite::discard(int num,int player){
	if(player == 1){
		ply1.erase(ply1.begin()+num);
	} else {
		ply2.erase(ply2.begin()+num);
	}
}


void Suite::play_to(int player1, int who, int player2, int des){
	std::string line1 = "the grave is empty";
	std::shared_ptr<Card> tmp = getcard(player1,who);
	if(player2 == 0){
//		shared_ptr<Card> tmp = get_card(player1,who);
		if(tmp->gettype()=="Ritual"){
//			std::cout<<"Playing a Ritual"<<std::endl;
//			if(!tmp){
//				std::cout<<"Fucking idiot"<<std::endl;
//			}
			t2->push(player1,tmp,0);
		} else if(tmp->gettype() == "Minion"){
				if(t2->get_size(player1) == 5){
					return;
				}
			t2->push(player1,tmp);
		} else {
			if(tmp->getname() == "Recharge"){
				t2->getcard(player1,-1)->buff(0,3);
			} else if(tmp->getname() == "Raise Dead"){
//				std::cout<<"Raise Dead is called"<<std::endl;
				if(t3->get_size(player1) == 0){
					throw line1;
				} else {
					t2->push(player1,t3->getcard(player1,t3->get_size(player1) -1 ));
					t3->discard(player1,t3->get_size(player1) -1);
				}
			} else if(tmp->getname() == "Blizzard"){
				int bs1 = t2->get_size(1);
				int bs2 = t2->get_size(2);
				for(int a = 0; a < bs1;++a){
					t2->getcard(1,a);
				}
				for(int a = 0; a < bs2; ++a){
					t2->getcard(2,a);
				}
				t2->deathcheck(player1);
				if(player1 == 1){
					t2->deathcheck(2);
				} else {
					t2->deathcheck(1);
				}
			}
		}
	} else {
		if(tmp->gettype() == "Enchantment"){
			t2->getcard(player1,des)->enchant_minion(tmp);
		} else {
			if(tmp->getname() == "Banish"){
				if(des < 0){
					t2->ritualset(player2,nullptr);
				} else {
					t2->discard(des,player2);
				}
			} else if(tmp->getname() == "Disenchant"){
				t2->getcard(player2,des)->dischant_minion();
				t2->deathcheck(player2);
			} else if(tmp->getname() == "Unsummon"){
				push(player2,t2->getcard(player2,des));
				t2->discard(des,player2);
			}
		}
	}
	discard(who,player1);
}

void Suite::use_ab(int player1, int who, int player2, int des){
	std::shared_ptr<Card> tmp = getcard(player1,who);
	if(player2 == 0){
		if(tmp->getname()=="Apprentice Summoner"){
			if(player1 == 1){
				if(ply1.size() == 5){
					return;
				}
			}else {
				if(ply2.size() == 5){
					return;
				}
			}
			std::shared_ptr<Card> temp(new Card{player1,"Air Element"});
			temp->enter_play(player1,tmp->return_game());
			if(player1 == 1){
				ply1.push_back(temp);
			}else {
				ply2.push_back(temp);
			}
		} else if(tmp->getname()=="Master Summoner"){
			int put_in = 0;
			if(player1 == 1){
				put_in = 5 - ply1.size();
			} else {
				put_in = 5 - ply2.size();
			}
			if(put_in > 3){
				put_in = 3;
			}
			std::shared_ptr<Card> temp;
			for(int a = 0;a < put_in;++a){
				temp = std::shared_ptr<Card>(new Card{player1,"Air Element"});
				temp->enter_play(player1,tmp->return_game());
				if(player1 == 1){
					ply1.push_back(temp);
				} else {
					ply2.push_back(temp);
				}
			}
		}
	}else {
		if(tmp->getname()=="Novice Pyromancer"){
			tmp=getcard(player2,des);
			tmp->buff(0,-1);
		}
	}
}


void Suite::shuffle(){
	std::random_shuffle(ply1.begin(),ply1.end());
	std::random_shuffle(ply2.begin(),ply2.end());
}
