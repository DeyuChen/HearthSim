#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "cardbook.pb.h"

#define HAND_SIZE_LIMIT 10
#define FIELD_SIZE_LIMIT 7
#define INIT_DECK_SIZE 30

using namespace std;

class Player;
class Game;

enum Target {
    Any_character = 0,
    Own_hero
};

class Player {
public:
    Player(cardbook::Card* hero, vector<cardbook::Card*> &cards);
    void print();
    void show();
    bool action();
    void draw();

    void start_turn();

    Game *game = NULL;
    cardbook::Card *hero = NULL;
    cardbook::Card *weapon = NULL;
    vector<cardbook::Card*> deck;
    vector<cardbook::Card*> hand;
    vector<cardbook::Card*> discard;
    vector<cardbook::Card*> field;
    int max_mana = 0;
    int mana = 0;
    int draw_penalty = 0;
    bool is_player_one = true;

private:
    void init_deck(vector<cardbook::Card*> &cards);
};

class Game {
public:
    Game(vector<Player> &player, cardbook::CardBook &cardbook);
    void print();
    void show();
    cardbook::Card* generate_card(int id);
    void player_turn();
    void to_field(Player &p, cardbook::Card* card, int i);
    void play();
    void damage(Player &p, int i, int v);
    void restore(Player &p, int i, int v);
    void turn_buff(Player &p, int i, int v1, int v2);
    bool check_valid_attack_target(Player &p, int i);
    void attack();
    void power();

    bool choose_target(int &i, int &j);
    bool effect_deal_damage(Target t, int v);
    bool effect_restore_health(Target t, int v);
    bool effect_gain_armor(int i, int v);
    bool effect_destroy_weapon(int i);
    bool effect_draw(int i, int v);
    bool effect_turn_buff(Target t, int v1, int v2);
    bool effect_gain_mana(int i, int v);
    bool effect_gain_max_mana(int i, int v);
    bool take_effect(cardbook::Effect *eff);

    vector<Player> &player;
    cardbook::CardBook &cardbook;
    bool end = false;
};

#endif
