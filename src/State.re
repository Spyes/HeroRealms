type action =
  | ClickDeck
  | ClickMarketCard(Card.card)
  | ClickCardInHand(Card.card, Player.player)
  | ClickCardInField(Card.card, Player.player)
  | PrepareChampions(Player.player);

type phase =
  | SetupPhase(int)
  | MainPhase(int)
  | DiscardPhase(int)
  | DrawPhase(int);

type state = {
  players: Player.player,
  deck: Deck.deck,
  market: Cards.cards,
  currPhase: phase,
};

let reducer = (action: action, state: state) =>
  switch (action) {
  | PrepareChampions(player) =>
    let field = Util.prepareChampions(~cards=player.field);
    ReasonReact.Update({
      ...state,
      players: {
        ...player,
        field,
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
  | ClickMarketCard(card) =>
    let (newPlayer, market) =
      Util.takeFromMarket(~card, ~market=state.market, ~player=state.players);
    let (newMarket, newDeck) = Util.drawFromDeck(~market, ~deck=state.deck);
    ReasonReact.Update({
      ...state,
      market: newMarket,
      players: newPlayer,
      deck: newDeck,
    });
  | ClickCardInHand(card, player) =>
    let newPlayer = Util.playFromHand(~card, ~player);
    ReasonReact.Update({...state, players: newPlayer});
  | ClickCardInField(card, player) =>
    switch (card.cardType) {
    | Champion =>
      card.expended == false ?
        {
          let field =
            List.fold_right(
              (curr: Card.card, acc: Cards.cards) =>
                card.id === curr.id ?
                  [{...curr, expended: true}, ...acc] : [curr, ...acc],
              player.field,
              [],
            );
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
