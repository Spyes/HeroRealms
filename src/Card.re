let component = ReasonReact.statelessComponent("Card");

type cardType =
  | Action
  | Champion
  | Gem
  | Coin

type faction =
  | Wild
  | Necros
  | Guild
  | Imperial

type ability =
  | AddCoins(int)

type abilityChain =
  | And(list(ability))
  | Or(list(ability))


type card = {
  id: string,
  name: string,
  cardType: cardType,
  defense: option(int),
  cost: option(int),
  faction: option(faction),
  primaryAbility: option(ability),
  allyAbility: option(ability),
  image: string
};

let make = (~card: card, ~onClick=?, _children) => {
  let click = event => {
    switch (onClick) {
      | Some(func) => func(card)
      | None => ()
    }
  };
  let costElement = {
    switch (card.cost) {
      | Some(cost) =>
        let openStr = " - (";
        let costStr = (cost |> string_of_int);
        let closeStr = ")";
        ReasonReact.string(openStr ++ costStr ++ closeStr)
      | None => ReasonReact.null
    }
  };
  let abilityElement = {
    switch (card.primaryAbility) {
      | Some(ability) => ReasonReact.string("Ability")
      | None => ReasonReact.null
    }
  };
  {
    ...component,
    render: _self => {
      <div className="Card" onClick=click>
        <img src=card.image />
      </div>;
    }
  }
};
