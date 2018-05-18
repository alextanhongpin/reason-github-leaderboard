let component = ReasonReact.statelessComponent("GithubPage");

let str = ReasonReact.string;

let make = (~headline="Github Page", _child) => {
  ...component,

  render: (_self) => 
    <div>
      <h1>(str(headline))</h1>
      <div>(str("View all Github users in Malaysia"))</div>
    </div>
};