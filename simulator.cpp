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
    for (int i = 3; i < 33; i++){
        cards.push_back(generate_card(cardbook, i));
    }
    Player p1(generate_card(cardbook, 1), cards);
    for (int i = 3; i < 33; i++){
        cards.push_back(generate_card(cardbook, i));
    }
    Player p2(generate_card(cardbook, 2), cards);

    vector<Player> player = {p1, p2};
    Game game(player, cardbook);
    
    while (!game.end)
        game.player_turn();

    return 0;
}
