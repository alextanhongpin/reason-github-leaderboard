[%bs.raw {|require('./Error.css')|}];

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Error");

let make =
    (
      ~heading="We're Sorry",
      ~subheading="There seems to be some error fetching data from our server.",
      _children
    ) => {
  ...component,
  render: _self =>
    <div className="error">
      <h1> (str(heading)) </h1>
      <p> (str(subheading)) </p>
    </div>
};