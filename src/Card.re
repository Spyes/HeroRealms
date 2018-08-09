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
  | Or(list(ability))
  | ForEach((cardType, ability));

type card = {
  id: string,
  name: string,
  cardType,
  defense: option(int),
  cost: option(int),
  faction: option(faction),
  primaryAbility: option(ability),
  allyAbility: option(ability),
  sacrificeAbility: option(ability),
  image: string,
  expended: bool,
};

let make =
    (
      ~card: card,
      ~onClick=(_card: card) => (),
      ~onMouseOver=(_card: card) => (),
      ~overlays: bool=true,
      _children,
    ) => {
  let click = _event => onClick(card);
  let mouseOver = _event => onMouseOver(card);
  let className = card.expended ? "expended" : "";
  let numOfAbilities =
    List.fold_left(
      (acc: int, ability: option(ability)) =>
        switch (ability) {
        | Some(_) => acc + 1
        | None => 0
        },
      0,
      [card.primaryAbility, card.allyAbility, card.sacrificeAbility],
    );
  Js.log(numOfAbilities);
  let primaryAbilityOverlayElement =
    switch (overlays, card.primaryAbility) {
    | (true, Some(_ability)) =>
      <div
        className="primary-ability"
        onClick=(_event => Js.log("primary ability"))>
        ("P-Ability!" |> ReasonReact.string)
      </div>
    | (true, None)
    | (false, _) => ReasonReact.null
    };
  let allyAbilityOverlayElement =
    switch (overlays, card.allyAbility) {
    | (true, Some(_ability)) =>
      <div
        className="ally-ability" onClick=(_event => Js.log("ally ability"))>
        ("A-Ability!" |> ReasonReact.string)
      </div>
    | (true, None)
    | (false, _) => ReasonReact.null
    };
  {
    ...component,
    render: _self =>
      <div className="Card" onClick=click onMouseOver=mouseOver>
        primaryAbilityOverlayElement
        allyAbilityOverlayElement
        <img src=card.image className />
      </div>,
  };
};
