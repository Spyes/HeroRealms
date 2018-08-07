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
      market: [],
      sacrifice: [],
      currPhase: SetupPhase(0),
      focused: None,
    }: State.state
  ),
  reducer: State.reducer,
  render: self => {
    let {players, deck, market, focused, sacrifice}: State.state = self.state;
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
          <button onClick=(_event => self.send(State.DrawHand(players)))>
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
        />
        <div className="DeckAndMarket">
          <Deck deck=sacrifice title="Sacrifice" faceUp=true />
          <Deck
            deck
            title="Deck"
            onClick=(_event => self.send(State.ClickDeck))
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
