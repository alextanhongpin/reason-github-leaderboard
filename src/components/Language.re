let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Language");

let make = (~language, _children) => {
  ...component,
  render: _self =>
    switch language {
    | Some(language) =>
      <span className="language-holder">
        <span
          className="language-icon"
          style=(
            ReactDOMRe.Style.make(~background=Color.getColor(language), ())
          )
        />
        (str(" "))
        <div className="language-label"> (str(language)) </div>
      </span>
    | None => ReasonReact.null
    }
};