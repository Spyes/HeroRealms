let component = ReasonReact.statelessComponent("Cards");

type cards = list(Card.card);

let make =
    (
      ~cards: cards,
      ~title: string="",
      ~onClick=(_card: Card.card) => (),
      ~onMouseOver=_card => (),
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="Cards">
      <h4> (title |> ReasonReact.string) </h4>
      <div className="content">
        (
          cards
          |> List.map((card: Card.card) =>
               <Card card onClick key=card.id onMouseOver />
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </div>
    </div>,
};
