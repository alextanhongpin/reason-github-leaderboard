[%bs.raw {|require('./Header.css')|}];

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Header");

let make = _children => {
  ...component,
  render: _self =>
    <header className="header">
      (str("engineers.my"))
      <span className="header-top"> (str("Github Malaysia")) </span>
    </header>
};