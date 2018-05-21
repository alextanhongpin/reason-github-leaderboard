[%bs.raw {|require('./Loader.css')|}];

let component = ReasonReact.statelessComponent("Loader");

let make = _children => {
  ...component,
  render: _self =>
    <span className="loader">
      <span className="a" />
      <span className="b" />
      <span className="c" />
      <span className="d" />
      <span className="e" />
      <span className="f" />
      <span className="g" />
      <span className="h" />
      <span className="i" />
      <span className="j" />
      <span className="k" />
      <span className="l" />
      <span className="m" />
      <span className="n" />
      <span className="o" />
      <span className="p" />
      <span className="q" />
    </span>
};