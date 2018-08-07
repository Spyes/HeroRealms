open ReasonReact;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => (
    {
      players: Mock.players,
      deck: Util.shuffleDeck(~deck=Mock.deck),
      market: [],
      currPhase: SetupPhase(0),
      focused: None,
    }: State.state
  ),
  reducer: State.reducer,
  render: self => {
    let {players, deck, market, focused}: State.state = self.state;
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
        <div className="action-btns">
          <button
            onClick=(_event => self.send(State.PrepareChampions(players)))>
            ("Prepare" |> ReasonReact.string)
          </button>
          <button onClick=(_event => self.send(State.CleanupField(players)))>
            ("Clean-up" |> ReasonReact.string)
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
      focusedCardElement
    </div>;
  },
};
