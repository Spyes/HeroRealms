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
  mutable expended: bool,
};

let make =
    (
      ~card: card,
      ~onClick=(_card: card) => (),
      ~onMouseOver=(_card: card) => (),
      ~onClickPrimaryAbility=(_card: card) => (),
      ~onClickAllyAbility=(_card: card) => (),
      ~onClickSacrificeAbility=(_card: card) => (),
      ~overlays: bool=true,
      _children,
    ) => {
  let click = _event => onClick(card);
  let mouseOver = _event => onMouseOver(card);
  let clickPrimary = _event => onClickPrimaryAbility(card);
  let clickAlly = _event => onClickAllyAbility(card);
  let clickSacrifice = _event => onClickSacrificeAbility(card);
  let className = card.expended ? "expended" : "";
  let numOfAbilities =
    List.fold_left(
      (acc: int, ability: option(ability)) =>
        switch (ability) {
        | Some(_) => acc + 1
        | None => acc
        },
      0,
      [card.primaryAbility, card.allyAbility, card.sacrificeAbility],
    );
  let primaryAbilityOverlayElement =
    switch (overlays, numOfAbilities, card.primaryAbility) {
    | (true, 1, Some(_ability)) =>
      <div className="primary-ability full" onClick=clickPrimary />
    | (true, _, Some(_ability)) =>
      <div className="primary-ability" onClick=clickPrimary />
    | (true, _, None)
    | (false, _, _) => ReasonReact.null
    };
  let allyAbilityOverlayElement =
    switch (overlays, numOfAbilities, card.allyAbility) {
    | (true, 1, Some(_ability)) =>
      <div className="ally-ability full" onClick=clickAlly />
    | (true, _, Some(_ability)) =>
      <div className="ally-ability" onClick=clickAlly />
    | (true, _, None)
    | (false, _, _) => ReasonReact.null
    };
  let sacrificeAbilityOverlayElement =
    switch (overlays, numOfAbilities, card.sacrificeAbility) {
    | (true, 1, Some(_ability)) =>
      <div className="sacrifice-ability full" onClick=clickSacrifice />
    | (true, _, Some(_ability)) =>
      <div className="sacrifice-ability" onClick=clickSacrifice />
    | (true, _, None)
    | (false, _, _) => ReasonReact.null
    };
  {
    ...component,
    render: _self =>
      <div className="Card" onClick=click onMouseOver=mouseOver>
        primaryAbilityOverlayElement
        allyAbilityOverlayElement
        sacrificeAbilityOverlayElement
        <img src=card.image className />
      </div>,
  };
};
