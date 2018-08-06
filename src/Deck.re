let component = ReasonReact.statelessComponent("Deck");

type deck = list(Card.card);

let make =
    (
      ~deck: deck,
      ~faceUp: bool=false,
      ~title: string="",
      ~onClick=_event => (),
      _children,
    ) => {
  ...component,
  render: _self => {
    let deckElement =
      faceUp ?
        switch (List.length(deck)) {
        | 0 => ReasonReact.null
        | _ =>
          let card = List.hd(deck);
          <Card card />;
        } :
        (
          switch (List.length(deck)) {
          | 0 => ReasonReact.null
          | _ =>
            <img
              src="https://www.herorealms.com/wp-content/uploads/2017/09/hero_realms_back.jpg"
            />
          }
        );
    <div className="Deck" onClick>
      <h4> (title |> ReasonReact.string) </h4>
      <div className="content"> deckElement </div>
    </div>;
  },
};
