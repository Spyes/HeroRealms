let component = ReasonReact.statelessComponent("Player");

type player = {
  name: string,
  hand: Cards.cards,
  deck: Deck.deck,
  field: Cards.cards,
  discard: Deck.deck,
  mutable coins: int,
};

let make = (~player: player, ~onClickHand, _children) => {
  let onClick = card => onClickHand(~card, ~player);
  let {name, hand, deck, field, discard, coins} = player;
  {
    ...component,
    render: _self =>
      <div className="Player">
        (name |> ReasonReact.string)
        (" - " |> ReasonReact.string)
        (coins |> string_of_int |> ReasonReact.string)
        <Deck title=(name ++ " Deck") deck />
        <Deck title="Discard" deck=discard faceUp=true />
        <Cards cards=field title="Field" />
        <Cards cards=hand title="Hand" onClick />
      </div>,
  };
};
