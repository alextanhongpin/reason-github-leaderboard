let component = ReasonReact.statelessComponent("Loader");

let make = _children => {
  ...component,
  render: _self =>
    <span className="loading"> (ReasonReact.string("Loading...")) </span>
};