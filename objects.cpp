#include "objects.h"
#include <assert.h>

void print_card(cardbook::Card* card){
    cout << "id : " << card->id() << endl
        << "name : " << card->name() << endl
        << "type : " << card->type() << endl;

    switch (card->type()){
        case cardbook::Card::MINION:
            cout << "HP : " << card->mutable_minion()->hp() << "/" << card->mutable_minion()->max_hp() << endl;
            break;

        case cardbook::Card::HERO:
            cout << "HP : " << card->mutable_hero()->hp() << "/" << card->mutable_hero()->max_hp() << endl;
            break;
    }
}

Player::Player(cardbook::Card* hero, vector<cardbook::Card*> &cards) :
    hero(hero)
{
    init_deck(cards);
}

void Player::print(){
    cout << "Mana: " << mana << "/" << max_mana << endl;
    cout << endl << "Hero:" << endl << endl;
    print_card(hero);

    cout << endl << "Deck size: " << deck.size() << endl;

    cout << endl << "Hand:" << endl;
    for (auto c : hand){
        cout << endl;
        print_card(c);
    }

    cout << endl << "Field:" << endl;
    for (auto c : field){
        cout << endl;
        print_card(c);
    }
}

void Player::show(){
    cout << "Mana: " << mana << "/" << max_mana << endl;
    cout << endl << "Hero:" << endl << endl;
    print_card(hero);

    cout << endl << "Deck size: " << deck.size() << endl;

    cout << endl << "Hand size: " << hand.size() << endl;

    cout << endl << "Field:" << endl;
    for (auto c : field){
        cout << endl;
        print_card(c);
    }
}

bool Player::action(){
    string input;

    cout << "input action: ";
    cin >> input;

    if (input == "end")
        return false;

    if (input == "show")
        game->show();
    else if (input == "play"){
        game->play();
    }
    else if (input == "attack"){
        game->attack();
    }

    if (game->end)
        return false;
    return true;
}

void Player::start_turn(){
    mana = ++max_mana;
    draw();
    for (auto m : field){
        m->mutable_minion()->set_active(true);
    }
}

void Player::draw(){
    if (deck.empty()){
        game->damage(game->player[0], -1, draw_penalty++);
        return;
    }

    if (hand.size() < HAND_SIZE_LIMIT)
        hand.push_back(deck.back());

    deck.pop_back();
}

void Player::init_deck(vector<cardbook::Card*> &cards){
    if (cards.size() != INIT_DECK_SIZE)
        return;

    for (int i = INIT_DECK_SIZE; i > 0; i--){
        int d = rand() % i;
        deck.push_back(cards[d]);
        cards.erase(cards.begin() + d);
    }
}

Game::Game(vector<Player> &player) :
    player(player)
{
    player[0].game = player[1].game = this;
    if (rand() % 2)
        swap(player[0], player[1]);

    for (int i = 0; i < 3; i++){
        player[0].draw();
        player[1].draw();
    }
    player[1].draw();
}

void Game::print(){
    cout << "Player1:" << endl;
    player[0].print();
    cout << "--------------------" << endl;
    cout << endl << "Player2:" << endl;
    player[1].print();
}

void Game::show(){
    cout << "Player2:" << endl;
    player[1].show();
    cout << "--------------------" << endl;
    cout << endl << "Player1:" << endl;
    player[0].print();
}

void Game::player_turn(){
    if (!end){
        player[0].start_turn();
        show();
        while (player[0].action()){}
        swap(player[0], player[1]);
    }
    cout << "game end" << endl;
}

void Game::to_field(Player &p, cardbook::Card* card, int i){
    assert(i <= p.field.size() && p.field.size() != FIELD_SIZE_LIMIT);
    p.field.insert(p.field.begin() + i, card);
}

void Game::play(){
    int i, j;

    cout << "which card: ";
    cin >> i;
    if (i >= player[0].hand.size()){
        cerr << "card does not exist" << endl;
        return;
    }
    if (player[0].hand[i]->cost() > player[0].mana){
        cerr << "not enough mana" << endl;
        return;
    }

    cardbook::Card *card_to_play = player[0].hand[i];

    switch (card_to_play->type()){
        case cardbook::Card::MINION:
            if (player[0].field.size() == FIELD_SIZE_LIMIT){
                cerr << "battlefield full" << endl;
                return;
            }

            cout << "battlefield position: ";
            cin >> j;
            if (j > player[0].field.size()){
                cerr << "invalid position" << endl;
                return;
            }

            for (int id = 0; id < card_to_play->mutable_minion()->battlecry_size(); id++){
                if (!take_effect(card_to_play->mutable_minion()->mutable_battlecry(id)))
                    return;
            }

            to_field(player[0], card_to_play, j);

            break;

        default:
            cerr << "unknown card type" << endl;
    }

    player[0].mana -= card_to_play->cost();
    player[0].hand.erase(player[0].hand.begin() + i);
}

void Game::damage(Player &p, int i, int v){
    if (i == -1){
        cardbook::Hero *hero = p.hero->mutable_hero();
        hero->set_hp(hero->hp() - v);
        if (hero->hp() <= 0)
            end = true;
    }
    else {
        cardbook::Minion *minion = p.field[i]->mutable_minion();
        minion->set_hp(minion->hp() - v);
        if (minion->hp() <= 0){
            p.discard.push_back(p.field[i]);
            p.field.erase(p.field.begin() + i);
        }
    }
}

void Game::restore(Player &p, int i, int v){
    if (i == -1){
        cardbook::Hero *hero = p.hero->mutable_hero();
        hero->set_hp(min(hero->max_hp(), hero->hp() + v));
    }
    else {
        cardbook::Minion *minion = p.field[i]->mutable_minion();
        minion->set_hp(min(minion->max_hp(), minion->hp() + v));
    }
}

bool Game::check_valid_attack_target(Player &p, int i){
    if (i >= (int)p.field.size()){
        cerr << "target does not exist" << endl;
        return false;
    }

    if (i == -1 || !p.field[i]->mutable_minion()->taunt()){
        for (auto m : p.field){
            if (m->mutable_minion()->taunt()){
                cerr << "there are taunt minions" << endl;
                return false;
            }
        }
    }

    return true;
}

void Game::attack(){
    int i, j;

    cout << "who attacks: ";
    cin >> i;
    if (i >= (int)player[0].field.size()){
        cerr << "character does not exist" << endl;
        return;
    }

    cardbook::Minion *attacker = player[0].field[i]->mutable_minion();

    if (!attacker->active()){
        cerr << "character is not active" << endl;
        return;
    }

    cout << "attack whom: ";
    cin >> j;
    if (!check_valid_attack_target(player[1], j))
        return;

    int d1 = max(0, (i == -1)? player[0].hero->mutable_hero()->att() : attacker->att());
    int d2 = max(0, (j == -1)? 0 : player[1].field[j]->mutable_minion()->att());

    damage(player[0], i, d2);
    damage(player[1], j, d1);

    attacker->set_active(false);
}

bool Game::effect_deal_damage(Target t, int v){
    switch (t){
        case Any_character:
            int i, j;

            cout << "damage player (0: self, 1: enemy): ";
            cin >> i;
            if (i != 0 && i != 1){
                cerr << "invalid target" << endl;
                return false;
            }

            cout << "damage character: ";
            cin >> j;
            if (j >= (int)player[i].field.size()){
                cerr << "invalid target" << endl;
                return false;
            }

            damage(player[i], j, v);

            break;

        default:
            cerr << "unknown target" << endl;
            return false;
    }

    return true;
}

bool Game::effect_restore_health(Target t, int v){
    switch (t){
        case Any_character:
            int i, j;

            cout << "restore player (0: self, 1: enemy): ";
            cin >> i;
            if (i != 0 && i != 1){
                cerr << "invalid target" << endl;
                return false;
            }

            cout << "restore character: ";
            cin >> j;
            if (j >= (int)player[i].field.size()){
                cerr << "invalid target" << endl;
                return false;
            }

            restore(player[i], j, v);

            break;

        default:
            cerr << "unknown target" << endl;
            return false;
    }
}

bool Game::effect_destroy_weapon(int i){
    if (player[i].weapon){
        player[i].discard.push_back(player[i].weapon);
        player[i].weapon = NULL;
    }
    return true;
}

bool Game::effect_destroy_weapon(int i, int v){
    for (int n = 0; n < v; n++){
        player[i].draw();
    }
    return true;
}

bool Game::take_effect(cardbook::Effect *eff){
    switch (eff->type()){
        case cardbook::Effect::Deal_damage:
            return effect_deal_damage(static_cast<Target>(eff->args(0)), eff->args(1));
            break;

        case cardbook::Effect::Restore_health:
            return effect_restore_health(static_cast<Target>(eff->args(0)), eff->args(1));
            break;

        case cardbook::Effect::Destroy_weapon:
            return effect_destroy_weapon(eff->args(0));
            break;

        case cardbook::Effect::Summon:
            break;

        case cardbook::Effect::Draw:
            return effect_destroy_weapon(eff->args(0), eff->args(1));
            break;

        case cardbook::Effect::Buff:
            break;

        default:
            cerr << "unknown effect" << endl;
            return false;
    }

    return true;
}
