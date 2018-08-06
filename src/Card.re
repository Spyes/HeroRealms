let component = ReasonReact.statelessComponent("Card");

type cardType =
  | Action
  | Champion
  | Gem
  | Item
  | Coin;

type faction =
  | Wild
  | Necros
  | Guild
  | Imperial;

type ability =
  | AddCoins(int)
  | AddCombat(int)
  | Expend(ability)
  | And(list(ability))
  | Or(list(ability));

type card = {
  id: string,
  name: string,
  cardType,
  defense: option(int),
  cost: option(int),
  faction: option(faction),
  primaryAbility: option(ability),
  allyAbility: option(ability),
  image: string,
};

let make = (~card: card, ~onClick=?, _children) => {
  let click = _event =>
    switch (onClick) {
    | Some(func) => func(card)
    | None => ()
    };
  {
    ...component,
    render: _self =>
      <div className="Card" onClick=click> <img src=card.image /> </div>,
  };
};
