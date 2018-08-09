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
      ~onChangeStat,
      _children,
    ) => {
  let valueFromEvent = evt =>
    evt |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
  let onClickHand = card => onClickInHand(~card, ~player);
  let onClickField = card => onClickInField(~card, ~player);
  let onChange = event => {
    let key = valueFromEvent(event)##name;
    let value = valueFromEvent(event)##value;
    onChangeStat(~key, ~value, ~player);
  };
  let {name, hand, deck, field, discard, coins, combat, health} = player;
  {
    ...component,
    render: _self =>
      <div className="Player">
        (name |> ReasonReact.string)
        <br />
        ("Health: " |> ReasonReact.string)
        <input
          _type="number"
          value=(string_of_int(health))
          name="health"
          onChange
        />
        <br />
        ("Coins: " |> ReasonReact.string)
        <input
          _type="number"
          value=(string_of_int(coins))
          name="coins"
          onChange
        />
        <br />
        ("Combat: " |> ReasonReact.string)
        <input
          _type="number"
          value=(string_of_int(combat))
          name="combat"
          onChange
        />
        <div className="Decks">
          <Deck
            title=("Deck (" ++ (deck |> List.length |> string_of_int) ++ ")")
            deck
          />
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
