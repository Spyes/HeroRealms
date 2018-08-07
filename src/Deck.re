let component = ReasonReact.statelessComponent("Deck");

let make =
    (
      ~deck: Cards.cards,
      ~faceUp: bool=false,
      ~title: string="",
      ~onClick=_event => (),
      _children,
    ) => {
  ...component,
  render: _self => {
    let deckElement =
      switch (List.length(deck)) {
      | 0 => <div className="empty" />
      | _ =>
        faceUp ?
          {
            let card = List.hd(deck);
            <Card card />;
          } :
          <img
            src="https://www.herorealms.com/wp-content/uploads/2017/09/hero_realms_back.jpg"
          />
      };
    <div className="Deck" onClick>
      <h4> (title |> ReasonReact.string) </h4>
      <div className="content"> deckElement </div>
    </div>;
  },
};
