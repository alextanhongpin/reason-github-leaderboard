let component = ReasonReact.statelessComponent("Loader");

let make = _children => {
  ...component,
  render: _self =>
    <div className="loading"> (ReasonReact.string("Loading...")) </div>
};