open ReasonReact;

type game = {
  players: Player.player,
  deck: Deck.deck,
  market: Cards.cards,
};

type action =
  | ClickDeck
  | ClickMarketCard(Card.card)
  | ClickCardInHand(Card.card, Player.player);

let deck = [
  {
    Card.id: "1",
    name: "Tithe Priest",
    cost: Some(2),
    cardType: Card.Champion,
    defense: None,
    faction: Some(Card.Imperial),
    primaryAbility: None,
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-015-tithe-priest.jpg",
  },
  {
    Card.id: "2",
    name: "Taxation",
    cost: Some(1),
    cardType: Card.Action,
    defense: None,
    faction: Some(Card.Imperial),
    primaryAbility: Some(Card.AddCoins(2)),
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-017-taxation.jpg",
  },
  {
    Card.id: "3",
    name: "Varrick, the Necromancer",
    cost: Some(5),
    cardType: Card.Champion,
    defense: Some(3),
    faction: Some(Card.Necros),
    primaryAbility: None,
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-060-varrick-the-necromancer.jpg",
  },
  {
    Card.id: "10",
    name: "Elven Gift",
    cost: Some(2),
    cardType: Card.Action,
    defense: None,
    faction: Some(Card.Wild),
    primaryAbility: None,
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-066-elven-gift.jpg",
  },
  {
    Card.id: "11",
    name: "Street Thug",
    cost: Some(3),
    cardType: Card.Champion,
    defense: Some(4),
    faction: Some(Card.Guild),
    primaryAbility: None,
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-039-street-thug.jpg",
  },
  {
    Card.id: "12",
    name: "Master Weyan",
    cost: Some(4),
    cardType: Card.Champion,
    defense: Some(4),
    faction: Some(Card.Imperial),
    primaryAbility: None,
    allyAbility: None,
    image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-010-master-weyan.jpg",
  },
];

let players = {
  Player.name: "Lewis",
  hand: [
    {
      Card.id: "4",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "5",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "6",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "7",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "8",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "9",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
    {
      Card.id: "13",
      name: "Gold",
      cost: None,
      cardType: Card.Coin,
      defense: None,
      faction: None,
      primaryAbility: Some(Card.AddCoins(1)),
      allyAbility: None,
      image: "https://www.herorealms.com/wp-content/uploads/2017/09/BAS-EN-097-gold.jpg",
    },
  ],
  deck: [],
  field: [],
  discard: [],
  coins: 0,
};

let component = ReasonReact.reducerComponent("Game");

let drawFromDeck = (~market: Cards.cards, ~deck: Deck.deck) =>
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
    (~card: Card.card, ~market: Cards.cards, ~player: Player.player) =>
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

let playFromHand = (~card: Card.card, ~player: Player.player) => {
  let notIt = (c: Card.card) => c.id !== card.id;
  let newHand = List.filter(notIt, player.hand);
  switch (card.primaryAbility) {
  | Some(ability) =>
    switch (ability) {
    | AddCoins(amount) => player.coins = player.coins + amount
    }
  | None => ()
  };
  {...player, hand: newHand, field: [card, ...player.field]};
};

let make = _children => {
  ...component,
  initialState: () => {players, deck, market: []},
  reducer: (action, state) =>
    switch (action) {
    | ClickDeck =>
      switch (List.length(state.deck)) {
      | 0 => ReasonReact.NoUpdate
      | _ =>
        let (newMarket, newDeck) =
          drawFromDeck(~market=state.market, ~deck=state.deck);
        ReasonReact.Update({...state, market: newMarket, deck: newDeck});
      }
    | ClickMarketCard(card) =>
      let (newPlayer, market) =
        takeFromMarket(~card, ~market=state.market, ~player=state.players);
      let (newMarket, newDeck) = drawFromDeck(~market, ~deck=state.deck);
      ReasonReact.Update({
        ...state,
        market: newMarket,
        players: newPlayer,
        deck: newDeck,
      });
    | ClickCardInHand(card, player) =>
      let newPlayer = playFromHand(~card, ~player);
      ReasonReact.Update({...state, players: newPlayer});
    },
  render: self => {
    let {players, deck, market} = self.state;
    <div className="Game">
      <Player
        player=players
        onClickHand=(
          (~card, ~player) => self.send(ClickCardInHand(card, player))
        )
      />
      <Deck deck title="Deck" onClick=(_event => self.send(ClickDeck)) />
      <Cards
        cards=market
        title="Market"
        onClick=(card => self.send(ClickMarketCard(card)))
      />
    </div>;
  },
};
