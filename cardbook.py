import cardbook_pb2

cardbook = cardbook_pb2.CardBook()

card = cardbook.cards.add()
card.id = 0
card.type = cardbook_pb2.Card.SPELL
card.cost = 0;
card.name = "The Coin"
effect = card.spell.effect.add()
effect.type = cardbook_pb2.Effect.Gain_mana
effect.args.append(0)
effect.args.append(1)

card = cardbook.cards.add()
card.id = 1
card.type = cardbook_pb2.Card.HERO
card.cost = 0;
card.name = "Mage"
card.character.att = 0
card.character.max_hp = 30
card.character.hp = 30
card.character.armor = 0
card.character.power_cost = 2
power = card.character.power.add()
power.type = cardbook_pb2.Effect.Deal_damage
power.args.append(0)
power.args.append(1)

card = cardbook.cards.add()
card.id = 2
card.type = cardbook_pb2.Card.HERO
card.cost = 1;
card.name = "Warrior"
card.character.att = 0
card.character.max_hp = 30
card.character.hp = 30
card.character.armor = 0
card.character.power_cost = 2
power = card.character.power.add()
power.type = cardbook_pb2.Effect.Gain_armor
power.args.append(0)
power.args.append(2)

for i in range(3, 33):
    card = cardbook.cards.add()
    card.id = i
    card.type = cardbook_pb2.Card.MINION
    card.cost = 1;
    card.name = "test"
    card.character.att = 1
    card.character.max_hp = 1
    card.character.hp = 1
    if i < 10:
        effect = card.character.battlecry.add()
        effect.type = cardbook_pb2.Effect.Deal_damage
        effect.args.append(0)
        effect.args.append(3)

f = open("cardbook", "wb")
f.write(cardbook.SerializeToString())
f.close()
