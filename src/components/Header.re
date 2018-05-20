[%bs.raw {|require('./Header.css')|}];

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Header");

let make = (~heading="", ~subheading="", _children) => {
  ...component,
  render: _self =>
    <header className="header">
      <br/>
      <Link href="/">
        <h1 className="header-heading">(str(heading))</h1>
        <span className="header-subheading"> (str(subheading)) </span>
      </Link>
      <br/>
    </header>
};