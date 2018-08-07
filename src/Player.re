let component = ReasonReact.statelessComponent("Player");

type player = {
  name: string,
  hand: Cards.cards,
  deck: Cards.cards,
  field: Cards.cards,
  discard: Cards.cards,
  coins: int,
  combat: int,
  health: int,
};

let make =
    (
      ~player: player,
      ~onClickInHand,
      ~onClickInField,
      ~onMouseOverCard=_card => (),
      _children,
    ) => {
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
        <div className="Decks">
          <Deck title=(name ++ " Deck") deck />
          <Deck
            title=(
              "Discard (" ++ (discard |> List.length |> string_of_int) ++ ")"
            )
            deck=discard
            faceUp=true
          />
        </div>
        <Cards
          cards=field
          title="Field"
          onClick=onClickField
          onMouseOver=onMouseOverCard
        />
        <Cards
          cards=hand
          title="Hand"
          onClick=onClickHand
          onMouseOver=onMouseOverCard
        />
      </div>,
  };
};
