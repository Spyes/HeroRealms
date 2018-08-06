let component = ReasonReact.statelessComponent("Player");

type player = {
  name: string,
  hand: Cards.cards,
  deck: Deck.deck,
  field: Cards.cards,
  discard: Deck.deck,
  mutable coins: int,
  mutable combat: int,
  mutable health: int,
};

let make = (~player: player, ~onClickInHand, ~onClickInField, _children) => {
  let onClickHand = card => onClickInHand(~card, ~player);
  let onClickField = card => onClickInField(~card, ~player);
  let {name, hand, deck, field, discard, coins, combat, health} = player;
  {
    ...component,
    render: _self =>
      <div className="Player">
        (name |> ReasonReact.string)
        <br />
        ("Health: " |> ReasonReact.string)
        (health |> string_of_int |> ReasonReact.string)
        <br />
        ("Coins: " |> ReasonReact.string)
        (coins |> string_of_int |> ReasonReact.string)
        <br />
        ("Combat: " |> ReasonReact.string)
        (combat |> string_of_int |> ReasonReact.string)
        <Deck title=(name ++ " Deck") deck />
        <Deck title="Discard" deck=discard faceUp=true />
        <Cards cards=field title="Field" onClick=onClickField />
        <Cards cards=hand title="Hand" onClick=onClickHand />
      </div>,
  };
};
