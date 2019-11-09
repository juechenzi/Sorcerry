#include "game.h"
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

Game::Game(){
    	player1 = "";
    	player2 = "";
    	health_one = 20;
    	health_two = 20;
    	magic_one = 3;
    	magic_one = 3;
	deck = shared_ptr<Suite>(new Suite);
	hand = shared_ptr<Suite>(new Suite);
	board = shared_ptr<Suite>(new Suite);
	grave = shared_ptr<Suite>(new Suite);

    	deck->sett(hand,board,grave);
    	hand->sett(deck,board,grave); 
    	board->sett(deck,hand,grave);
    	grave->sett(deck,hand,board);
}

void Game::set_turn(int a){
	current = a;
}

void Game::set_player(string a){
	if(current == 1){
		player1 = a;
	} else {
		player2 = a;
	}
}

void Game::set_magic(int a){
	if(current == 1){
		magic_one = a;
	} else {
		magic_two = a;
	}
}


void Game::gainMagic(int a){
	if(current == 1){
		magic_one += a;
	} else {
		magic_two += a;
	}
}

void Game::attack_p(int i){
    string line1 ="No such minion on board";
    string line2 ="Minion has no more actions";
	int check;
	if(current ==1){
		check = board->get_size(current);

		if(check < i){
			throw line1;
		} else {
			shared_ptr<Card> tmp = board->getcard(current,i-1);
			check = tmp->get_attack();
//			cout<<"The play of current is"<<tmp->get_play()<<endl;
		       if(tmp->get_play() == 0){
			       throw line2;
		       } else {
		                tmp->change_play(-1);	       
				health_two -= check;
		       }
		}
	}else {
	        check = board->get_size(current);
	       	if (check < i) {
		     	throw line1;
	       	} else {
	    		shared_ptr<Card> tmp = board->getcard(current, i-1);
	    		check = tmp->get_attack();
			if(tmp->get_play() == 0){
				tmp->change_play(-1);
				health_one -= check;
			} else {
				throw line2;
			}
		}
	}
}


void Game::attack_m(int i,int m){
    string line1 ="No such minion on board";
    string line2 ="Minion has no more action";
	int check1;
	int check2;
	int one,two;
	check1 = board->get_size(1);
	check2 = board->get_size(2);
	if(current == 1){
		if(check1 < i || check2 < m){
			throw line1;
		} else {
			shared_ptr<Card> tmp1 = board->getcard(1,i-1);
			shared_ptr<Card> tmp2 = board->getcard(2,m-1);
			one = tmp1->get_attack() * -1;
			two = tmp2->get_attack() * -1;
			if(tmp1->get_play() != 0){
				tmp1->change_play(-1);
				tmp1->buff(0,two);
				tmp2->buff(0,one);
				board->deathcheck(1);
				board->deathcheck(2);
			}else {
				throw line2;
			}
		}
	} else {
		if(check1 < m || check2 < i){
			throw line1;
		} else {
			shared_ptr<Card> tmp1 = board->getcard(1,m-1);
			shared_ptr<Card> tmp2 = board->getcard(2,i-1);
			one = tmp1->get_attack();
			two = tmp2->get_attack();
			if(tmp2->get_play() != 0){
				tmp2->change_play(-1);
				tmp1->buff(0,-1*two);
				tmp2->buff(0, -1*one);
				board->deathcheck(1);
				board->deathcheck(2);
			} else {
				throw line2;
			}
		}
	}
}

//draw function only draw card do not consider with the change within the game condition.
void Game::draw(){
        string line1 = "No cards left";
		if(deck->get_size(current) == 0){
			throw line1;
		}else if(hand->get_size(current) >= 5){
			return;
//			cout<<"there are five cards on the table;"<<endl;
		} else{
			deck->draw(current);
		}
}


void Game::discard(int i){
    string line = "No such card";
		int check;
		check = hand->get_size(current);
		if(check >=  i){
			hand->discard(i-1,current);
		} else {
			throw line;
		}
	
}

void Game::play(int i){
    string line1 ="Magic is not enough" ;
    string line2 = "Need target";
    string line3 = "No such card";
	int check;
	check = hand->get_size(current);
	if(check >=  i){
		shared_ptr<Card> tmp = hand->getcard(1,i-1);
		if(!tmp->need_target()){   
			int cost = tmp->get_cost();
			if(((cost > magic_one) && (current == 1)) || ((cost > magic_two) && (current == 2))){
				throw line1;
			} else {
				if(current == 1){
					magic_one -= cost;
				} else {
					magic_two -= cost;
				}
				hand->play_to(current,i-1);//need to do something;

//					cout<<i-1<<endl;
			}
		} else {
			throw line2;
		}
	} else {
		throw line3;
	}
}


void Game::play_to(int i,int player,string card){
	int check;
	string line1 = "NO such card";
	string line2 = "no need target";
	string line3 = "Magic is not enough";
	string line4 = "Invalid player entered";
	string line5 = "Invalid target type";
	string line6 = "No such minion";
	check = hand->get_size(current);
	if(check < i){
		throw line1;
	} else {
		shared_ptr<Card> tmp = hand->getcard(i-1,current);
		if(!tmp->need_target()){
			throw line2;
		} else {
			int cost = tmp->get_cost();
			if(((cost > magic_one) && (current == 1))||((cost > magic_two) && (current == 2))){
				throw line3;
			} else {
				if((player != 1) && (player != 2)){
					throw line4;
				} else {
					int target=-2;
					check = hand->get_size(player);
					if(card == "r"){
						target = -1;
					} else if((card >= "1") && (card <= "5")){
					       target = stoi(card);
					} else {
						throw line5;
					}
					
					if((target >= -1) && (target <= check)){
						if(current == 1){
							magic_one -= cost;
						} else {
							magic_two -= cost;
						}
						hand->play_to(current,i-1,player,target);//
					} else {
						throw line6;
					}
				}
			}
		}
	}
}


void Game::inspect(int i){
    string line = "No such minion";
	int check;
	shared_ptr<Card> tmp;
	check = board->get_size(current);
	if(check <= i){
		throw line;
	} else {
		tmp = board->getcard(current,i-1);
		tmp->inspect();//need to do
	}
}

void Game::handp(){
    string line ="Empty Hand";
	vector<card_template_t> tmp1;
	shared_ptr<Card> tmp1_card;
	int amount = hand->get_size(current);
//	cout<<amount<<endl;
	if(amount == 0){
		throw line;
	}
	string ac;
	string dc;
	for(int a = 0;a < amount;++a){
//		cout<<"About to get the card, good so far"<<current<<a<<endl;
			tmp1_card = hand->getcard(current,a);
//			cout<<"OMG here"<<endl;
//	cout<<tmp1_card->getname()<<"  "<<tmp1_card->getdes()<<endl;
			if(tmp1_card->gettype() == "Minion"){
				if(tmp1_card->getdes() != ""){
					if(tmp1_card->getabc() != 0){
		    				tmp1.push_back(display_minion_activated_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence(),tmp1_card->getabc(),tmp1_card->getdes()));
			    		} else {
					    	tmp1.push_back(display_minion_triggered_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence(),tmp1_card->getdes()));
				  	  }
			    	} else {
				    	tmp1.push_back(display_minion_no_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence()));
			    	}
        	        } else if(tmp1_card->gettype() == "Ritual"){
				    tmp1.push_back(display_ritual(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->getabc(),tmp1_card->getdes(),tmp1_card->get_defence()));
//				    cout<<"size is "<<tmp1.size()<<endl;
			} else if(tmp1_card->gettype() == "Spell"){
				    tmp1.push_back(display_spell(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->getdes()));
			} else if(tmp1_card->gettype() == "Enchantment"){
				if((tmp1_card->get_defence() == 0) && (tmp1_card->get_attack() == 0)){
					tmp1.push_back(display_enchantment(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->getdes()));
			       	} else {
					if((tmp1_card->get_defence() < 0) && (tmp1_card->get_attack() < 0)){
						ac = "*"+to_string(-1*tmp1_card->get_attack());
						dc = "*"+to_string(-1*tmp1_card->get_defence());
					}else {
						ac = "+"+to_string(tmp1_card->get_attack());
						dc = "+"+to_string(tmp1_card->get_defence());
					}
					tmp1.push_back(display_enchantment_attack_defence(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->getdes(),ac,dc));
				}
			}

	}
//	cout<<"Bomb shakalaka:"<<amount<<endl;
//	cout<<"size is "<<endl;
//	cout<<tmp1.size()<<endl;
	for(int a = 0;a < 11;++a){
//		cout<<"A " <<a<<endl;
		for(int b =0;b < amount;++b){
//			cout<<"B "<<b<<endl;
		cout<<tmp1.at(b)[a];
		}
		cout<<endl;
	}
}




void Game::boardp(){
	vector<card_template_t> tmp;
	//print the upper board;
	cout<<EXTERNAL_BORDER_CHAR_TOP_LEFT;
	for(int a = 0; a < 165;++a){
		cout<<EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	}
	cout<<EXTERNAL_BORDER_CHAR_TOP_RIGHT<<endl;
	//print the first row
	shared_ptr<Card> tmp_card =  board->getcard(1,-1); //so this one need to comfirm the existence of ritual

	if(tmp_card){
		tmp.push_back(display_ritual(tmp_card->getname(),tmp_card->get_cost(),tmp_card->getabc(),tmp_card->getdes(),tmp_card->get_defence()));
	} else {
		tmp.push_back(CARD_TEMPLATE_BORDER);
	}
	tmp.push_back(CARD_TEMPLATE_EMPTY);
	tmp.push_back(display_player_card(1,player1,health_one,magic_one));
	tmp.push_back(CARD_TEMPLATE_EMPTY);
	int grave_size = grave->get_size(1);
	if(grave_size == 0){
		tmp.push_back(CARD_TEMPLATE_BORDER);
	} else {
		shared_ptr<Card> tmp_card1 = grave->getcard(1,grave_size-1);
		if(tmp_card1->getdes() == ""){
			if(tmp_card1->getabc() != 0){
    				tmp.push_back(display_minion_activated_ability(tmp_card1->getname(),tmp_card1->get_cost(),tmp_card1->get_attack(),tmp_card1->get_defence(),tmp_card1->getabc(),tmp_card1->getdes()));
	    		} else {
			    	tmp.push_back(display_minion_triggered_ability(tmp_card1->getname(),tmp_card1->get_cost(),tmp_card1->get_attack(),tmp_card1->get_defence(),tmp_card1->getdes()));
			} 
		} else {
			tmp.push_back(display_minion_no_ability(tmp_card1->getname(),tmp_card1->get_cost(),tmp_card1->get_attack(),tmp_card1->get_defence()));
		}
	}
	for(int a = 0;a < 11;++a){
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN;
		for(int b =0;b < 5;++b){
		cout<<tmp.at(b)[a];
		}
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN<<endl;
	}
	/* display the second row */

	vector<card_template_t> tmp1;
	shared_ptr<Card> tmp1_card;
	int amount = board->get_size(1);
	for(int a = 0;a < 5;++a){
		if(a < amount){
			tmp1_card = board->getcard(1,a);
			if(tmp1_card->getdes() == ""){
				if(tmp1_card->getabc() != 0){
	    				tmp1.push_back(display_minion_activated_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence(),tmp1_card->getabc(),tmp1_card->getdes()));
		    		} else {
				    	tmp1.push_back(display_minion_triggered_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence(),tmp1_card->getdes()));
			  	  }
		    	} else {
			    	tmp1.push_back(display_minion_no_ability(tmp1_card->getname(),tmp1_card->get_cost(),tmp1_card->get_attack(),tmp1_card->get_defence()));
		    	}
		} else {
			tmp1.push_back(CARD_TEMPLATE_BORDER);
		}
	}
	for(int a = 0;a < 11;++a){
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN;
		for(int b =0;b < 5;++b){
		cout<<tmp1.at(b)[a];
		}
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN<<endl;
	}
	
	//print the middle graph
	for(int a = 0; a < 10 ; ++a){
		cout<<CENTRE_GRAPHIC[a]<<endl;
	}

	//print the third row
	vector<card_template_t> tmp2;
	shared_ptr<Card> tmp2_card;
	 amount = board->get_size(2);
	for(int a = 0;a < 5;++a){
		if(a < amount){
			tmp2_card = board->getcard(2,a);
			if(tmp2_card->getdes() == ""){
				if(tmp2_card->getabc() != 0){
	    				tmp2.push_back(display_minion_activated_ability(tmp2_card->getname(),tmp2_card->get_cost(),tmp2_card->get_attack(),tmp2_card->get_defence(),tmp2_card->getabc(),tmp2_card->getdes()));
		    		} else {
				    	tmp2.push_back(display_minion_triggered_ability(tmp2_card->getname(),tmp2_card->get_cost(),tmp2_card->get_attack(),tmp2_card->get_defence(),tmp2_card->getdes()));
			  	  }
		    	} else {
			    	tmp2.push_back(display_minion_no_ability(tmp2_card->getname(),tmp2_card->get_cost(),tmp2_card->get_attack(),tmp2_card->get_defence()));
		    	}

		} else {
			tmp2.push_back(CARD_TEMPLATE_BORDER);
		}
	}
	for(int a = 0;a < 11;++a){
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN;
		for(int b =0;b < 5;++b){
		cout<<tmp2.at(b)[a];
		}
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN<<endl;
	}

	//print the forth row
	vector<card_template_t> tmp3;
	shared_ptr<Card> tmp3_card =  board->getcard(2,-1); //so this one need to comfirm the existence of ritual
	if(tmp3_card){
		tmp3.push_back(display_ritual(tmp3_card->getname(),tmp3_card->get_cost(),tmp3_card->getabc(),tmp3_card->getdes(),tmp3_card->get_defence()));
	} else {
		tmp3.push_back(CARD_TEMPLATE_BORDER);
	}
	tmp3.push_back(CARD_TEMPLATE_EMPTY);
	tmp3.push_back(display_player_card(2,player2,health_two,magic_two));
	tmp3.push_back(CARD_TEMPLATE_EMPTY);
	 grave_size = grave->get_size(2);
	if(grave_size == 0){
		tmp3.push_back(CARD_TEMPLATE_BORDER);
	} else {
		shared_ptr<Card> tmp3_card1 = grave->getcard(2,grave_size-1);
			if(tmp3_card->getdes() == ""){
				if(tmp3_card->getabc() != 0){
	    				tmp3.push_back(display_minion_activated_ability(tmp3_card->getname(),tmp3_card->get_cost(),tmp3_card->get_attack(),tmp3_card->get_defence(),tmp3_card->getabc(),tmp3_card->getdes()));
		    		} else {
				    	tmp3.push_back(display_minion_triggered_ability(tmp3_card->getname(),tmp3_card->get_cost(),tmp3_card->get_attack(),tmp3_card->get_defence(),tmp3_card->getdes()));
			  	  }
		    	} else {
			    	tmp3.push_back(display_minion_no_ability(tmp3_card->getname(),tmp3_card->get_cost(),tmp3_card->get_attack(),tmp3_card->get_defence()));
		    	}
	} 
	for(int a = 0;a < 11;++a){
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN;
		for(int b =0;b < 5;++b){
		cout<<tmp3.at(b)[a];
		}
		cout<<EXTERNAL_BORDER_CHAR_UP_DOWN<<endl;
	}
	

	//print the bottom line
	cout<<EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
	for(int a = 0; a < 165;++a){
		cout<<EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	}
	cout<<EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT<<endl;
}





void Game::start(){
//	cout<<"start in the game is called"<<endl;
	board->receive(current,4);
}

void Game::end(){
//	cout<<"End int the game is called"<<endl;
	board->receive(current,5);
//	cout<<"all everyone receives the meesage"<<endl;
	if(current == 1){
		current = 2;
	} else {
		current = 1;
	}
//	cout<<"start should be called after this"<<endl;
}

void Game::useAb(int i){
    string line1 = "No such minion";
    string line2 = "Invalid because of the lack of target";
    string line3 = "magic is not enough";
    string line4 = "Cannot use ability because it is already banned";
	int check = board->get_size(current);
	
	if(check < i){
		throw line1;
	} else {
		shared_ptr<Card> tmp = board->getcard(current,i-1);
		if(tmp->getname() == "Novice Pyromancer"){
			throw line2;
		} else {
			check = tmp->get_cost();
			if((check > magic_one && current ==1)||(check > magic_two && current == 2)){
			throw line3;
			} else {
				if(current == 1){
					magic_one -= check;
				} else {
					magic_two -= check;
				}
				board->use_ab(current,i-1);
			}
		}
	}
}

void Game::useAb_to(int i,int player,string card){
    string line1 = "NO such card";
    string line2 = "no need target";
    string line3 = "Magic is not enough";
    string line4 = "Invalid player entered";
    string line5 = "Invalid target type";
	int check = board->get_size(current);
	shared_ptr <Card> tmp;
	if(check < i){
		throw line1;
	} else {
		tmp = board->getcard(current,i-1);
		if(tmp->getname() == "Novice Pyromancer"){
			if(card >= "1" && card <= "5"){
				int now = stoi(card);
				check = board->get_size(player);
				if(check <= now){
					throw line2;
				} else {
					check = tmp->get_cost();
					if((check > magic_one && current == 1)||(check> magic_two && current ==2 )){
						throw line3;
					} else {
						if(current == 1){
							magic_one -= check;
						} else {
							magic_two -= check;
						}
						board->use_ab(current,i-1,player,now-1); 
					}
					
				}
			} else {
				throw line4;
			}
		} else {
			throw line5;
		}
	}
}



void Game::shuffle(){
	deck->shuffle();
}

void Game::winning(){
    string line1 = player2 + " win!";
    string line2 = player1 + " win!";
    if(health_one <= 0){
        throw line1;
    } else if(health_two <= 0){
        throw line2;
    }
}




void Game::init_deck(string name,shared_ptr<Game> g){
		shared_ptr <Card> tmp (new Card{current,name});
		tmp->enter_play(current,g);
		deck->push(current,tmp);
}

void Game::receive(int player,int b){
	board->receive(player,b);
}
