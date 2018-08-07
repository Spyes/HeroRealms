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
  | AddHealth(int)
  | Expend(ability)
  | DrawCards(int)
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
  expended: bool,
};

let make =
    (
      ~card: card,
      ~onClick=(_card: card) => (),
      ~onMouseOver=(_card: card) => (),
      _children,
    ) => {
  let click = _event => onClick(card);
  let mouseOver = _event => onMouseOver(card);
  let className = card.expended ? "expended" : "";
  {
    ...component,
    render: _self =>
      <div className="Card" onClick=click onMouseOver=mouseOver>
        <img src=card.image className />
      </div>,
  };
};
