open ReasonReact;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => (
    {
      players: [
        {
          ...Mock.players,
          deck: Util.shuffleDeck(~deck=Mock.players.deck),
          id: "1",
        },
        {
          ...Mock.players,
          deck: Util.shuffleDeck(~deck=Mock.players.deck),
          id: "2",
        },
      ],
      deck: Util.shuffleDeck(~deck=Mock.deck),
      fireGems: Util.shuffleDeck(~deck=Mock.fireGems),
      market: [],
      sacrifice: [],
      currPhase: SetupPhase(0),
      focused: None,
      me: "1",
    }: State.state
  ),
  reducer: State.reducer,
  render: self => {
    let {players, deck, fireGems, market, focused, sacrifice, me}: State.state =
      self.state;
    let player = List.hd(players);
    let secondPlayer = List.nth(players, 1);
    let focusedCardElement =
      switch (focused) {
      | Some((card: Card.card)) =>
        <div className="Focused">
          <Card card={...card, expended: false} />
        </div>
      | None => ReasonReact.null
      };
    <div className="Game">
      <div className="contents">
        <div className="phase-info">
          (self.state.currPhase |> State.phaseString |> ReasonReact.string)
        </div>
        <div className="action-btns">
          <button
            onClick=(_event => self.send(State.PrepareChampions(player.id)))>
            ("Prepare" |> ReasonReact.string)
          </button>
          <button
            onClick=(_event => self.send(State.CleanupField(player.id)))>
            ("Clean-up" |> ReasonReact.string)
          </button>
          <button
            onClick=(_event => self.send(State.DrawHand(player.id, 5)))>
            ("Draw Hand" |> ReasonReact.string)
          </button>
        </div>
        <Player
          player
          onClickInHand=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInHand(card, player.id))
          )
          onClickInField=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInField(card, player.id))
          )
          onMouseOverCard=(
            (card: Card.card) => self.send(State.FocusCard(card))
          )
          onChangeStat=(
            (~key: string, ~value: string, ~player: Player.player) =>
              self.send(State.SetStat(key, value, player.id))
          )
          onClickDeck=(
            (~player: Player.player) =>
              self.send(State.DrawHand(player.id, 1))
          )
          onClickPrimaryAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayPrimaryAbility(card, player.id))
          )
          onClickAllyAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayAllyAbility(card, player.id))
          )
          onClickSacrificeAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlaySacrificeAbility(card, player.id))
          )
        />
        <div className="action-btns">
          <button
            onClick=(
              _event => self.send(State.PrepareChampions(secondPlayer.id))
            )>
            ("Prepare" |> ReasonReact.string)
          </button>
          <button
            onClick=(
              _event => self.send(State.CleanupField(secondPlayer.id))
            )>
            ("Clean-up" |> ReasonReact.string)
          </button>
          <button
            onClick=(_event => self.send(State.DrawHand(secondPlayer.id, 5)))>
            ("Draw Hand" |> ReasonReact.string)
          </button>
        </div>
        <Player
          player=secondPlayer
          onClickInHand=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInHand(card, player.id))
          )
          onClickInField=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInField(card, player.id))
          )
          onMouseOverCard=(
            (card: Card.card) => self.send(State.FocusCard(card))
          )
          onChangeStat=(
            (~key: string, ~value: string, ~player: Player.player) =>
              self.send(State.SetStat(key, value, player.id))
          )
          onClickDeck=(
            (~player: Player.player) =>
              self.send(State.DrawHand(player.id, 1))
          )
          onClickPrimaryAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayPrimaryAbility(card, player.id))
          )
          onClickAllyAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayAllyAbility(card, player.id))
          )
          onClickSacrificeAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlaySacrificeAbility(card, player.id))
          )
        />
        <div className="DeckAndMarket">
          <Deck deck=sacrifice title="Sacrifice" faceUp=true />
          <Deck
            deck
            title="Deck"
            onClick=(_event => self.send(State.ClickDeck))
          />
          <Deck
            deck=fireGems
            title="Fire Gems"
            faceUp=true
            onClick=(_event => self.send(State.ClickFireGems(me)))
          />
          <Cards
            cards=market
            title="Market"
            onMouseOver=(
              (card: Card.card) => self.send(State.FocusCard(card))
            )
            onClick=(card => self.send(State.ClickMarketCard(card, me)))
          />
        </div>
      </div>
      focusedCardElement
    </div>;
  },
};
