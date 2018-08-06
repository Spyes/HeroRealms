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
    }: State.state
  ),
  reducer: State.reducer,
  render: self => {
    let {players, deck, market}: State.state = self.state;
    <div className="Game">
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
      />
      <button
        onClick=(
          _event => self.send(State.PrepareChampions(self.state.players))
        )>
        ("Prepare" |> ReasonReact.string)
      </button>
      <Deck
        deck
        title="Deck"
        onClick=(_event => self.send(State.ClickDeck))
      />
      <Cards
        cards=market
        title="Market"
        onClick=(card => self.send(State.ClickMarketCard(card)))
      />
    </div>;
  },
};
