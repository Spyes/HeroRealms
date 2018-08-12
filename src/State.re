type action =
  | ClickDeck
  | ClickFireGems
  | ClickMarketCard(Card.card)
  | ClickCardInHand(Card.card, Player.player)
  | ClickCardInField(Card.card, Player.player)
  | PrepareChampions(Player.player)
  | CleanupField(Player.player)
  | DrawHand(Player.player, int)
  | SetStat(string, string, Player.player)
  | FocusCard(Card.card)
  | PlayAllyAbility(Card.card, Player.player)
  | PlayPrimaryAbility(Card.card, Player.player)
  | PlaySacrificeAbility(Card.card, Player.player);

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
  players: list(Player.player),
  deck: Cards.cards,
  fireGems: Cards.cards,
  market: Cards.cards,
  sacrifice: Cards.cards,
  currPhase: phase,
  focused: option(Card.card),
  playedPrimaryAbility: list(string),
  playedAllyAbility: list(string),
};

let reducer = (action: action, state: state) =>
  switch (action) {
  | FocusCard((card: Card.card)) =>
    ReasonReact.Update({...state, focused: Some(card)})
  | PrepareChampions((player: Player.player)) =>
    let field = Util.prepareChampions(~cards=player.field);
    let players =
      List.map(
        (p: Player.player) =>
          switch (p.id === player.id) {
          | false => p
          | true => {...p, field}
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | SetStat((key: string), (value: string), (player: Player.player)) =>
    let players =
      List.map(
        (p: Player.player) =>
          switch (p.id === player.id) {
          | false => p
          | true => Util.setStats(~key, ~value, ~player)
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | DrawHand((player: Player.player), (amount: int)) =>
    let players =
      List.map(
        (p: Player.player) =>
          switch (p.id === player.id) {
          | false => p
          | true =>
            Util.resolveAbility(
              ~ability=Some(Card.DrawCards(amount)),
              ~player,
            )
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | CleanupField((player: Player.player)) =>
    let players =
      List.map(
        (p: Player.player) =>
          switch (p.id === player.id) {
          | false => p
          | true =>
            let (field: Cards.cards, discard: Cards.cards) =
              Util.clearField(~field=p.field);
            {
              ...p,
              field,
              combat: 0,
              coins: 0,
              hand: [],
              discard: List.concat([discard, p.hand, p.discard]),
            };
          },
        state.players,
      );
    ReasonReact.Update({
      ...state,
      players,
      playedAllyAbility: [],
      playedPrimaryAbility: [],
    });
  | ClickDeck =>
    switch (List.length(state.deck)) {
    | 0 => ReasonReact.NoUpdate
    | _ =>
      let (market, deck) =
        Util.drawFromDeck(~market=state.market, ~deck=state.deck);
      ReasonReact.Update({...state, market, deck});
    }
  /*| ClickFireGems =>
      switch (List.length(state.fireGems)) {
      | 0 => ReasonReact.NoUpdate
      | _ =>
        let card: Card.card = List.hd(state.fireGems);
        let (players, fireGems) =
          Util.takeFromMarket(
            ~card,
            ~market=state.fireGems,
            ~player=state.players,
          );
        ReasonReact.Update({...state, fireGems, players});
      }
    | ClickMarketCard((card: Card.card)) =>
      let (players, fromMarket) =
        Util.takeFromMarket(~card, ~market=state.market, ~player=state.players);
      let (market, deck) =
        Util.drawFromDeck(~market=fromMarket, ~deck=state.deck);
      ReasonReact.Update({...state, market, players, deck});*/
  | ClickCardInHand((card: Card.card), (player: Player.player)) =>
    let players =
      List.map(
        (p: Player.player) =>
          switch (p.id === player.id) {
          | false => p
          | true => Util.playFromHand(~card, ~player=p)
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  /*| PlayAllyAbility((card: Card.card), (player: Player.player)) =>
      let eql = (id: string) => id === card.id;
      let allies = (ally: Card.card) =>
        ally.faction === card.faction && ! eql(ally.id);
      let alliesExist = List.exists(allies, player.field);
      let playedAbility = List.exists(eql, state.playedAllyAbility);
      switch (playedAbility, alliesExist) {
      | (false, false)
      | (true, _) => ReasonReact.NoUpdate
      | (false, true) =>
        let players = Util.resolveAbility(~ability=card.allyAbility, ~player);
        ReasonReact.Update({
          ...state,
          players,
          playedAllyAbility: [card.id, ...state.playedAllyAbility],
        });
      };
    | PlayPrimaryAbility((card: Card.card), (player: Player.player)) =>
      let eql = (id: string) => id === card.id;
      switch (List.exists(eql, state.playedPrimaryAbility)) {
      | false =>
        let players =
          Util.resolveAbility(~ability=card.primaryAbility, ~player);
        switch (card.primaryAbility) {
        | Some(Expend(_)) => card.expended = true
        | _ => ()
        };
        ReasonReact.Update({
          ...state,
          players,
          playedPrimaryAbility: [card.id, ...state.playedPrimaryAbility],
        });
      | true => ReasonReact.NoUpdate
      };
    | PlaySacrificeAbility((card: Card.card), (player: Player.player)) =>
      let players =
        Util.resolveAbility(~ability=card.sacrificeAbility, ~player);
      let field =
        List.filter((c: Card.card) => c.id !== card.id, player.field);
      let sacrifice = [card, ...state.sacrifice];
      ReasonReact.Update({
        ...state,
        sacrifice,
        players: {
          ...players,
          field,
        },
      });*/
  | _ => ReasonReact.NoUpdate
  };
