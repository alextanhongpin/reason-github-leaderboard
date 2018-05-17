[%bs.raw {|require('./App.css')|}];

/* [@bs.module] external logo : string = "./logo.svg"; */

/* [@bs.module] external Test : ReasonReact.reactClass = "./components/Test" */

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <GithubPage headline="Github Scraper"/>
      <Profile/>
      <Todo name="car"/>
    </div>,
};
