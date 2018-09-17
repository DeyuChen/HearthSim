#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "objects.h"
#include "cardbook.pb.h"

using namespace std;

cardbook::Card* generate_card(cardbook::CardBook &cardbook, int id){
    cardbook::Card *card = new cardbook::Card();
    card->CopyFrom(cardbook.cards(id));
    return card;
}

int main(){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    srand(time(NULL));
    
    cardbook::CardBook cardbook;
    fstream input("cardbook", ios::in | ios::binary);
    if (!input) {
      cout << "File not found.  Creating a new file." << endl;
    } else if (!cardbook.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
    
    vector<cardbook::Card*> cards;
    for (int i = 2; i < 32; i++){
        cards.push_back(generate_card(cardbook, i));
    }
    Player p1(generate_card(cardbook, 0), cards);
    for (int i = 2; i < 32; i++){
        cards.push_back(generate_card(cardbook, i));
    }
    Player p2(generate_card(cardbook, 1), cards);

    /*
    vector<Card*> card_list;
    vector<int> args = {0, 3};

    for (int i = 0; i < 30; i++){
        card_list.push_back(new Minion(i, 1, "test", 1, 1));
        if (i < 10)
            dynamic_cast<Minion*>(card_list.back())->battlecry.push_back(Effect(Deal_damage, args));
    }
    Player p1(new Hero(0, 0, "Mage", 30), card_list);
    
    for (int i = 0; i < 30; i++){
        card_list.push_back(new Minion(i, 1, "test", 1, 1));
        if (i < 10)
            dynamic_cast<Minion*>(card_list.back())->battlecry.push_back(Effect(Deal_damage, args));
    }
    Player p2(new Hero(0, 0, "Warrior", 30), card_list);
    */

    vector<Player> player = {p1, p2};
    Game game(player);
    
    while (!game.end)
        game.player_turn();

    return 0;
}
