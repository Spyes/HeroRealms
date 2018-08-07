type action =
  | ClickDeck
  | ClickMarketCard(Card.card)
  | ClickCardInHand(Card.card, Player.player)
  | ClickCardInField(Card.card, Player.player)
  | PrepareChampions(Player.player)
  | CleanupField(Player.player)
  | FocusCard(Card.card);

type phase =
  | SetupPhase(int)
  | MainPhase(int)
  | DiscardPhase(int)
  | DrawPhase(int);

type state = {
  players: Player.player,
  deck: Cards.cards,
  market: Cards.cards,
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
  | CleanupField((player: Player.player)) =>
    let (field: Cards.cards, discard: Cards.cards) =
      Util.clearField(~field=player.field);
    ReasonReact.Update({
      ...state,
      players: {
        ...player,
        field,
        discard: List.concat([discard, player.discard]),
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
