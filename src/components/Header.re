[%bs.raw {|require('./Header.css')|}];

let str = ReasonReact.string;
let component = ReasonReact.statelessComponent("Header");

let make = (_children) => {
  ...component,

  render: (_self) => 
    <header className="header">
      <h1>(str("Github Malaysia"))</h1>
    </header>
};