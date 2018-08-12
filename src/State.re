type action =
  | ClickDeck
  | ClickFireGems(Player.id)
  | ClickMarketCard(Card.card, Player.id)
  | ClickCardInHand(Card.card, Player.id)
  | ClickCardInField(Card.card, Player.id)
  | PrepareChampions(Player.id)
  | CleanupField(Player.id)
  | DrawHand(Player.id, int)
  | SetStat(Player.id, string, string)
  | FocusCard(Card.card)
  | PlayAllyAbility(Card.card, Player.id)
  | PlayPrimaryAbility(Card.card, Player.id)
  | PlaySacrificeAbility(Card.card, Player.id);

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
  me: Player.id,
  focused: option(Card.card),
};

let reducer = (action: action, state: state) =>
  switch (action) {
  | FocusCard((card: Card.card)) =>
    ReasonReact.Update({...state, focused: Some(card)})
  | PrepareChampions((playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let field = Util.prepareChampions(~cards=player.field);
            {...player, field};
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | SetStat((key: string), (value: string), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true => Util.setStats(~key, ~value, ~player)
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | DrawHand((playerId: Player.id), (amount: int)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            Util.resolveAbility(
              ~ability=Some(Card.DrawCards(amount)),
              ~player,
            )
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | CleanupField((playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let (field: Cards.cards, discard: Cards.cards) =
              Util.clearField(~field=player.field);
            {
              ...player,
              field,
              combat: 0,
              coins: 0,
              hand: [],
              discard: List.concat([discard, player.hand, player.discard]),
              playedAllyAbility: [],
              playedPrimaryAbility: [],
            };
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | ClickDeck =>
    switch (List.length(state.deck)) {
    | 0 => ReasonReact.NoUpdate
    | _ =>
      let (market, deck) =
        Util.drawFromDeck(~market=state.market, ~deck=state.deck);
      ReasonReact.Update({...state, market, deck});
    }
  | ClickFireGems((playerId: Player.id)) =>
    let card: Card.card = List.hd(state.fireGems);
    let playersTuples: list((Player.player, option(Cards.cards))) =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => (player, None)
          | true =>
            switch (List.length(state.fireGems)) {
            | 0 => (player, None)
            | _ =>
              let player = Util.buyCard(~card, ~player);
              let fireGems: Cards.cards =
                List.filter(
                  (c: Card.card) => c.id !== card.id,
                  state.fireGems,
                );
              (player, Some(fireGems));
            }
          },
        state.players,
      );
    Js.log(playersTuples);
    ReasonReact.NoUpdate;
  /*| ClickMarketCard((card: Card.card), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let (player, fromMarket) =
              Util.takeFromMarket(~card, ~market=state.market, ~player);
            let (market, deck) =
              Util.drawFromDeck(~market=fromMarket, ~deck=state.deck);
            player;
          },
        state.players,
      );
    ReasonReact.Update({...state, market, players, deck});*/
  | ClickCardInHand((card: Card.card), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true => Util.playFromHand(~card, ~player)
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | PlayAllyAbility((card: Card.card), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let eql = (id: string) => id === card.id;
            let allies = (ally: Card.card) =>
              ally.faction === card.faction && ! eql(ally.id);
            let alliesExist = List.exists(allies, player.field);
            let playedAbility = List.exists(eql, player.playedAllyAbility);
            if (playedAbility === false && alliesExist === true) {
              let newPlayer =
                Util.resolveAbility(~ability=card.allyAbility, ~player);
              {
                ...newPlayer,
                playedAllyAbility: [card.id, ...player.playedAllyAbility],
              };
            } else {
              player;
            };
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | PlayPrimaryAbility((card: Card.card), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let eql = (id: string) => id === card.id;
            let playedAbility = List.exists(eql, player.playedPrimaryAbility);
            switch (card.primaryAbility) {
            | Some(Expend(_)) => card.expended = true
            | _ => ()
            };
            if (playedAbility === false) {
              let newPlayer =
                Util.resolveAbility(~ability=card.primaryAbility, ~player);
              {
                ...newPlayer,
                playedPrimaryAbility: [
                  card.id,
                  ...player.playedPrimaryAbility,
                ],
              };
            } else {
              player;
            };
          },
        state.players,
      );
    ReasonReact.Update({...state, players});
  | PlaySacrificeAbility((card: Card.card), (playerId: Player.id)) =>
    let players =
      List.map(
        (player: Player.player) =>
          switch (player.id === playerId) {
          | false => player
          | true =>
            let player =
              Util.resolveAbility(~ability=card.sacrificeAbility, ~player);
            let field =
              List.filter((c: Card.card) => c.id !== card.id, player.field);
            {...player, field};
          },
        state.players,
      );
    let sacrifice = [card, ...state.sacrifice];
    ReasonReact.Update({...state, sacrifice, players});
  | _ => ReasonReact.NoUpdate
  };
