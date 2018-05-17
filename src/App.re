[%bs.raw {|require('./App.css')|}];

type route = 
  | ProfilePage
  | HomePage;

type action = 
  | ChangeRoute(route);

type state = {
  route: route,
};

let reducer = (action, state) =>
  switch action {
  | ChangeRoute(route) => ReasonReact.Update({ ...state, route: route })
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch url.path {
  | [] => HomePage
  | ["profile"] => ProfilePage
  | _ => HomePage
  };

let component = ReasonReact.reducerComponent("App");

let make = (~message, _children) => {
  ...component,
  reducer,
  initialState: () => { route: HomePage },
  subscriptions: (self) => [
    Sub(
      () => ReasonReact.Router.watchUrl((url) => self.send(ChangeRoute(url |> mapUrlToRoute))),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: ({ state }) =>
    <div className="App">
      (
        switch state.route {
        | HomePage => <div>(ReasonReact.string("This is the home page"))</div>
        | ProfilePage => <Profile/>
        }
      )
      /* <GithubPage headline="Github Scraper"/> */
      
      <Link href="profile">(ReasonReact.string("profole"))</Link>
      /* <Todo name="car"/> */
    </div>,
};

/* 
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
}; */
