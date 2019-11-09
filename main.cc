#include "card.h"
#include "suit.h"
#include "game.h"
#include <fstream>
#include <memory>
#include <vector>
using namespace std;

int main(int args, char *argv[]) {
    bool test = false;
//Xwindow *xw = nullptr;
    shared_ptr<Game> playing(new Game);
    string player1 = "";
    string player2 = "";
    vector<string> extra;/* to strore the extra move from -init*/
    for (int a = 1; a < args; ++a) {
        string order = argv[a];
        if (order == "-init") {
            ++a;
            if (a != args) {
                string filename = argv[a];
                ifstream allthesource{filename};
                getline(allthesource, player1);
                getline(allthesource, player2);
                string tmp;
                while (getline(allthesource, tmp)) {
                    extra.push_back(tmp);
                }
            } else {
                cout << "Init Invalid" << endl;
            }
        }
    }
    /* to test if the player has been sucessfully loaded;*/
    if (player1 == "") {
        cout << "missing name of player1" << endl;
        cout << "Enter player1:";
        getline(cin, player1);
    }
    playing->set_turn(1);
    playing->set_player(player1);

    if (player2 == "") {
        cout << "missing name of player2" << endl;
        cout << "Enter player2:";
        getline(cin, player2);
    }
    playing->set_turn(2);
    playing->set_player(player2);
    for (int a = 1; a < args; ++a) {
        string order = argv[a];
        if (order == "-deck1") {
            ++a;
            if (a != args) {
                string deck1file = argv[a];
                ifstream deck1l{deck1file};
                playing->set_turn(1);
                while (getline(deck1l, player1)) {
                    playing->init_deck(player1,playing);
                }
            }
        } else if (order == "-deck2") {
            ++a;
            if (a != args) {
                string deck2file = argv[a];
                ifstream deck2f{deck2file};
                playing->set_turn(2);
                while (getline(deck2f, player1)) {
                    playing->init_deck(player1,playing);
                }
            }
        } else if (order == "-testing") {
            test = true;
            cout<<"Test:";
        } else if (order == "-graphic") {
            /* graph process*/
        }
    }

    if (playing->deck->get_size(1) == 0) {
//        cout<<"player1 has no cards"<<endl;
        ifstream deck1{"default.deck"};
        playing->set_turn(1);
        while (getline(deck1, player1)) {
//		cout<<player1<<endl;
		playing->init_deck(player1,playing);
        }

    }

    if (playing->deck->get_size(2) == 0) {
//        cout<<"player2 has no cards"<<endl;
        ifstream deck2{"default.deck"};
        playing->set_turn(2);
        while (getline(deck2, player1)) {
//		cout<<player1<<endl;
            playing->init_deck(player1,playing);
        }
    }

    /* time to shuffle */
    if (!test) {
        playing->set_turn(1);
        playing->shuffle();
        playing->set_turn(2);
        playing->shuffle();
        cout<<"Cards has shuffled!"<<endl;
    }
    /* time to draw cards */   //draw in game has a problem?? it should been draw cards by turn
    if(!test) {
        for (int a = 0; a < 5; ++a) {
            playing->set_turn(1);//need a function to set current
            try { playing->draw(); }
            catch (string msg) { cout << msg << endl; }


            playing->set_turn(2);;
            try { playing->draw(); }
            catch (string msg) { cout << msg << endl; }
        }
    }
    //set up for real game
    string orderlist;
    istringstream orders;
    string msg;
    bool extra_on = false;

    /* set the playing.current to 1*/
    playing->set_turn(1);
    while (true) {

        if (extra.size() > 0) {  //commit the command in the init first
            playing->set_magic(999);
            extra_on = true;
            orderlist = extra[0];
            cout << "Preplaying is going on!" << endl;
        } else {
            if (!getline(cin, orderlist)) {
                break;
            }
        }

        if (test) {
            cout << "Test Mode is going on!" << endl;
            playing->set_magic(999);
        }
        orders = istringstream(orderlist);
        orders >> orderlist;
        try {
            if (orderlist == "quit") {
                cout << "Game forcely ends!" << endl;
                break;
            } else if (orderlist == "help") {
                ifstream helpfile{"help.txt"};
                string tmp;
                while (getline(helpfile, tmp)) {
                    cout << tmp << endl;
                }
            } else if (orderlist == "end") {
                playing->end();
                playing->gainMagic(1);
                if (test) {
                    playing->start();
                } else {
                    playing->draw();
                    playing->start();
                }
//`		cout<<"end command is finished"<<endl;
            } else if (orderlist == "draw") {
                if (test) {
                    playing->draw();
                } else {
                    cout << "not in test mod" << endl;
                }
            } else if (orderlist == "inspect") {
                int var;
                if (orders >> var) {
                    playing->inspect(var);
                } else {
                    cout << "invalid nehaviour" << endl;
                }
            } else if (orderlist == "hand") {
                playing->handp();
            } else if (orderlist == "board") {
                playing->boardp();
            } else if (orderlist == "discard") {
                int var;
                if (orders >> var) {
                    playing->discard(var);
                } else {
                    cout << "Invalid discard" << endl;
                }
            } else if (orderlist == "attack") {
                int var1, var2;
                if (orders >> var1) {
                    if (orders >> var2) {
                        playing->attack_m(var1, var2);
                    } else {
                        playing->attack_p(var1);
                    }
                } else {
                    cout << "Invalid behavior" << endl;
                }
            } else if (orderlist == "play") {
                int var1, var2;
                string c;
                if (orders >> var1) {
                    if (orders >> var2 && orders >> c) {
                        playing->play_to(var1, var2, c);
                    } else {
                        playing->play(var1);
                    }
                } else {
                    cout << "invalid behavior" << endl;
                }
            } else if (orderlist == "use") {
                int var1, var2;
                string c;
                if (orders >> var1) {
                    if (orders >> var2 && orders >> c) {
                        playing->useAb_to(var1, var2, c);
                    } else {
                        playing->useAb(var1);
                    }
                } else {
                    cout << "invalid behavior" << endl;
                }
            } else if(orderlist == "gain_magic"){
		    playing->gainMagic(999);
	    
	    
	    }else if (orderlist == "taoism"){
		   cout<<"You Win!"<<endl;
		   break;
	    } else {
                cout << "NO SUCH COMMAND" << endl;
            }

            //to make a change from command from init to command from cin;
            if (extra_on) {
                extra.erase(extra.begin());
                if (extra.size() == 0) {
                    playing->set_turn(2);
                    playing->set_magic(3);
                    playing->set_turn(1);
                    playing->set_magic(3);
                }
            }


            extra_on = false;
        }
        catch (string msg) {
            cout << msg << endl;
        }

        try {
            playing->winning();
        }
        catch (string msg) {
            cout << msg << endl;
            cout << "Nice game!" << endl;
	    return 1;
        }
    }
}





