let drawFromDeck =
    (~market: Cards.cards, ~deck: Deck.deck)
    : (Cards.cards, Deck.deck) =>
  List.length(market) < 5 ?
    switch (List.length(deck)) {
    | 0 => (market, deck)
    | _ =>
      let card = List.hd(deck);
      let market = [card, ...market];
      let deck = List.tl(deck);
      (market, deck);
    } :
    (market, deck);

let takeFromMarket =
    (~card: Card.card, ~market: Cards.cards, ~player: Player.player)
    : (Player.player, Cards.cards) =>
  switch (card.cost) {
  | Some(cost) =>
    player.coins >= cost ?
      {
        let player = {
          ...player,
          discard: [card, ...player.discard],
          coins: player.coins - cost,
        };
        let isEqual = (c: Card.card) => c.id !== card.id;
        let market = List.filter(isEqual, market);
        (player, market);
      } :
      (player, market)
  | None =>
    let player = {...player, discard: [card, ...player.discard]};
    let noIt = (c: Card.card) => c.id !== card.id;
    let market = List.filter(noIt, market);
    (player, market);
  };

let rec resolveAbility =
        (~ability: option(Card.ability), ~player: Player.player)
        : Player.player =>
  switch (ability) {
  | Some(ability) =>
    switch (ability) {
    | AddCoins(amount) => {...player, coins: player.coins + amount}
    | AddCombat(amount) => {...player, combat: player.combat + amount}
    | AddHealth(amount) => {...player, health: player.health + amount}
    | Expend(ability) =>
      let newPlayer = resolveAbility(~ability=Some(ability), ~player);
      newPlayer;
    | And(abilities) =>
      List.fold_right(
        (ability: Card.ability, acc: Player.player) =>
          resolveAbility(~ability=Some(ability), ~player=acc),
        abilities,
        player,
      )
    | _ => player
    }
  | None => player
  };

let playFromHand = (~card: Card.card, ~player: Player.player) : Player.player => {
  let notIt = (c: Card.card) => c.id !== card.id;
  let newHand = List.filter(notIt, player.hand);
  switch (card.cardType) {
  | Champion => ()
  | _ =>
    let newPlayer = resolveAbility(~ability=card.primaryAbility, ~player);
    player.coins = newPlayer.coins;
    player.combat = newPlayer.combat;
    player.health = newPlayer.health;
  };
  {...player, hand: newHand, field: [card, ...player.field]};
};

let discardHand = (~player: Player.player) : Player.player => {
  ...player,
  discard: List.concat([player.hand, player.discard]),
  hand: [],
};

let prepareChampions = (~cards: Cards.cards) =>
  List.map((card: Card.card) => {...card, expended: false}, cards);

let shuffleDeck = (~deck: Deck.deck) : Deck.deck => {
  let _ = Random.init(int_of_float(Js.Date.now()));
  let len = 100;
  deck
  |> List.map(card => (card, Random.int(len)))
  |> List.sort((a, b) => {
       let (_, n1) = a;
       let (_, n2) = b;
       n1 - n2;
     })
  |> List.map(fst);
};
