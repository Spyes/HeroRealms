type action =
  | ClickDeck
  | ClickMarketCard(Card.card)
  | ClickCardInHand(Card.card, Player.player)
  | ClickCardInField(Card.card, Player.player)
  | PrepareChampions(Player.player)
  | CleanupField(Player.player)
  | DrawHand(Player.player, int)
  | SetStat(string, string, Player.player)
  | FocusCard(Card.card);

type phase =
  | SetupPhase(int)
  | MainPhase(int)
  | DiscardPhase(int)
  | DrawPhase(int);
let phaseString = (phase: phase) : string =>
  switch (phase) {
  | SetupPhase(n) => "Setup - " ++ string_of_int(n)
  | MainPhase(n) => "Main - " ++ string_of_int(n)
  | DiscardPhase(n) => "Discard - " ++ string_of_int(n)
  | DrawPhase(n) => "Draw - " ++ string_of_int(n)
  };

type state = {
  players: Player.player,
  deck: Cards.cards,
  market: Cards.cards,
  sacrifice: Cards.cards,
  currPhase: phase,
  focused: option(Card.card),
};

let reducer = (action: action, state: state) =>
  switch (action) {
  | FocusCard((card: Card.card)) =>
    ReasonReact.Update({...state, focused: Some(card)})
  | PrepareChampions((player: Player.player)) =>
    let field = Util.prepareChampions(~cards=player.field);
    ReasonReact.Update({
      ...state,
      players: {
        ...player,
        field,
      },
    });
  | SetStat((key: string), (value: string), (player: Player.player)) =>
    let players = Util.setStats(~key, ~value, ~player);
    ReasonReact.Update({...state, players});
  | DrawHand((player: Player.player), (amount: int)) =>
    let players =
      Util.resolveAbility(~ability=Some(Card.DrawCards(amount)), ~player);
    ReasonReact.Update({...state, players});
  | CleanupField((player: Player.player)) =>
    let (field: Cards.cards, discard: Cards.cards) =
      Util.clearField(~field=player.field);
    ReasonReact.Update({
      ...state,
      players: {
        ...player,
        field,
        combat: 0,
        coins: 0,
        hand: [],
        discard: List.concat([discard, player.hand, player.discard]),
      },
    });
  | ClickDeck =>
    switch (List.length(state.deck)) {
    | 0 => ReasonReact.NoUpdate
    | _ =>
      let (newMarket, newDeck) =
        Util.drawFromDeck(~market=state.market, ~deck=state.deck);
      ReasonReact.Update({...state, market: newMarket, deck: newDeck});
    }
  | ClickMarketCard((card: Card.card)) =>
    let (newPlayer, market) =
      Util.takeFromMarket(~card, ~market=state.market, ~player=state.players);
    let (newMarket, newDeck) = Util.drawFromDeck(~market, ~deck=state.deck);
    ReasonReact.Update({
      ...state,
      market: newMarket,
      players: newPlayer,
      deck: newDeck,
    });
  | ClickCardInHand((card: Card.card), (player: Player.player)) =>
    let newPlayer = Util.playFromHand(~card, ~player);
    ReasonReact.Update({...state, players: newPlayer});
  | ClickCardInField((card: Card.card), (player: Player.player)) =>
    switch (card.cardType) {
    | Champion =>
      card.expended == false ?
        {
          let expendCard = (curr: Card.card, acc: Cards.cards) =>
            card.id === curr.id ?
              [{...curr, expended: true}, ...acc] : [curr, ...acc];
          let field = List.fold_right(expendCard, player.field, []);
          let newPlayer =
            Util.resolveAbility(~ability=card.primaryAbility, ~player);
          ReasonReact.Update({
            ...state,
            players: {
              ...newPlayer,
              field,
            },
          });
        } :
        ReasonReact.NoUpdate
    | _ => ReasonReact.NoUpdate
    }
  };
