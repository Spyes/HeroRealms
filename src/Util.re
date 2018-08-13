let drawFromDeck =
    (~market: Cards.cards, ~deck: Cards.cards)
    : (Cards.cards, Cards.cards) =>
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

let gainCard = (~card: Card.card, ~player: Player.player) : Player.player => {
  ...player,
  discard: [card, ...player.discard],
};

let buyCard =
    (~card: Card.card, ~player: Player.player)
    : (Player.player, bool) =>
  switch (card.cost) {
  | Some(cost) =>
    player.coins >= cost ?
      {
        let player = gainCard(~card, ~player);
        let newPlayer = {...player, coins: player.coins - cost};
        (newPlayer, true);
      } :
      (player, false)
  | None =>
    let player = gainCard(~card, ~player);
    (player, true);
  };

let clearField = (~field: Cards.cards) : (Cards.cards, Cards.cards) => {
  let isChampion = (card: Card.card) => card.cardType == Champion;
  let notChampion = (card: Card.card) => card.cardType != Champion;
  let champions = List.filter(isChampion, field);
  let all = List.filter(notChampion, field);
  (champions, all);
};

let prepareChampions = (~cards: Cards.cards) : Cards.cards =>
  List.map((card: Card.card) => {...card, expended: false}, cards);

let shuffleDeck = (~deck: Cards.cards) : Cards.cards => {
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

let getDrawDeck =
    (~player: Player.player, ~amount: int)
    : (array(Card.card), Cards.cards) =>
  switch (List.length(player.deck) < amount) {
  | false => (Array.of_list(player.deck), player.discard)
  | true =>
    let discard: Cards.cards = shuffleDeck(~deck=player.discard);
    (
      Array.concat([Array.of_list(player.deck), Array.of_list(discard)]),
      [],
    );
  };

let setStats = (~key: string, ~value: string, ~player: Player.player) =>
  switch (key) {
  | "coins" => {...player, coins: int_of_string(value)}
  | "health" => {...player, health: int_of_string(value)}
  | "combat" => {...player, combat: int_of_string(value)}
  | _ => player
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
    | DrawCards(amount) =>
      let (deck: array(Card.card), discard: Cards.cards) =
        getDrawDeck(~player, ~amount);
      switch (Array.length(deck)) {
      | 0 => player
      | _ =>
        let amount =
          amount > Array.length(deck) ? Array.length(deck) : amount;
        let toHand: Cards.cards = Array.to_list(Array.sub(deck, 0, amount));
        let newDeck: Cards.cards =
          Array.to_list(
            Array.sub(deck, amount, Array.length(deck) - amount),
          );
        {
          ...player,
          hand: List.concat([toHand, player.hand]),
          deck: newDeck,
          discard,
        };
      };
    | Expend(ability) => resolveAbility(~ability=Some(ability), ~player)
    | And(abilities) =>
      List.fold_right(
        (ability: Card.ability, acc: Player.player) =>
          resolveAbility(~ability=Some(ability), ~player=acc),
        abilities,
        player,
      )
    | ForEach((cardType, ability)) =>
      let isType = (card: Card.card) => card.cardType == cardType;
      let cards = List.filter(isType, player.field);
      List.fold_right(
        (_card: Card.card, acc: Player.player) =>
          resolveAbility(~ability=Some(ability), ~player=acc),
        cards,
        player,
      );
    | _ => player
    }
  | None => player
  };

let playFromHand = (~card: Card.card, ~player: Player.player) : Player.player => {
  let notIt = (c: Card.card) => c.id !== card.id;
  let newHand = List.filter(notIt, player.hand);
  {...player, hand: newHand, field: [card, ...player.field]};
};
