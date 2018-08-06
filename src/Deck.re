let component = ReasonReact.statelessComponent("Deck");

type deck = list(Card.card);

let make =
    (
      ~deck: deck,
      ~faceUp: bool=false,
      ~title: string="",
      ~onClick=?,
      _children,
    ) => {
  let click = (event, _self) =>
    switch (onClick) {
    | Some(func) => func(event)
    | None => ()
    };
  {
    ...component,
    render: self => {
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
      <div className="Deck" onClick=(self.handle(click))>
        <h4> (title |> ReasonReact.string) </h4>
        <div className="content"> deckElement </div>
      </div>;
    },
  };
};
