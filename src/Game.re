open ReasonReact;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => (
    {
      players: {
        ...Mock.players,
        deck: Util.shuffleDeck(~deck=Mock.players.deck),
      },
      deck: Util.shuffleDeck(~deck=Mock.deck),
      fireGems: Util.shuffleDeck(~deck=Mock.fireGems),
      market: [],
      sacrifice: [],
      currPhase: SetupPhase(0),
      focused: None,
      playedPrimaryAbility: [],
      playedAllyAbility: [],
    }: State.state
  ),
  reducer: State.reducer,
  render: self => {
    let {players, deck, fireGems, market, focused, sacrifice}: State.state =
      self.state;
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
            onClick=(_event => self.send(State.PrepareChampions(players)))>
            ("Prepare" |> ReasonReact.string)
          </button>
          <button onClick=(_event => self.send(State.CleanupField(players)))>
            ("Clean-up" |> ReasonReact.string)
          </button>
          <button onClick=(_event => self.send(State.DrawHand(players, 5)))>
            ("Draw Hand" |> ReasonReact.string)
          </button>
        </div>
        <Player
          player=players
          onClickInHand=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInHand(card, player))
          )
          onClickInField=(
            (~card: Card.card, ~player: Player.player) =>
              self.send(State.ClickCardInField(card, player))
          )
          onMouseOverCard=(
            (card: Card.card) => self.send(State.FocusCard(card))
          )
          onChangeStat=(
            (~key: string, ~value: string, ~player: Player.player) =>
              self.send(State.SetStat(key, value, player))
          )
          onClickDeck=(
            (~player: Player.player) => self.send(State.DrawHand(player, 1))
          )
          onClickPrimaryAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayPrimaryAbility(card, player))
          )
          onClickAllyAbility=(
            (card: Card.card, player: Player.player) =>
              self.send(State.PlayAllyAbility(card, player))
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
            onClick=(_event => self.send(State.ClickFireGems))
          />
          <Cards
            cards=market
            title="Market"
            onMouseOver=(
              (card: Card.card) => self.send(State.FocusCard(card))
            )
            onClick=(card => self.send(State.ClickMarketCard(card)))
          />
        </div>
      </div>
      focusedCardElement
    </div>;
  },
};
