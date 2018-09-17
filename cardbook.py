import cardbook_pb2

cardbook = cardbook_pb2.CardBook()

card = cardbook.cards.add()
card.id = 0
card.type = cardbook_pb2.Card.HERO
card.cost = 0;
card.name = "Mage"
card.hero.att = 0
card.hero.max_hp = 30
card.hero.hp = 30

card = cardbook.cards.add()
card.id = 1
card.type = cardbook_pb2.Card.HERO
card.cost = 1;
card.name = "Warrior"
card.hero.att = 0
card.hero.max_hp = 30
card.hero.hp = 30

for i in range(2, 32):
    card = cardbook.cards.add()
    card.id = i
    card.type = cardbook_pb2.Card.MINION
    card.cost = 1;
    card.name = "test"
    card.minion.att = 1
    card.minion.max_hp = 1
    card.minion.hp = 1
    if i < 10:
        effect = card.minion.battlecry.add()
        effect.type = cardbook_pb2.Effect.Deal_damage
        effect.args.append(0)
        effect.args.append(3)

f = open("cardbook", "wb")
f.write(cardbook.SerializeToString())
f.close()
