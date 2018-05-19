[%bs.raw {|require('./App.css')|}];

let str = ReasonReact.string;

type user = string;

type route = 
  | ProfilePage(user)
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
  | ["profiles", user] => ProfilePage(user)
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
      <Header/>
      <br/>
      <div className="tag">(str("most starred"))</div>
      <div className="tag">(str("last updated"))</div>
      (
        switch state.route {
        | HomePage => 
          <div>
            <p>(str("Find out the latest statistics on Github Users in Malaysia."))</p>
            <UserCounter/>(str(" "))
            <RepoCounter/>
            <LeaderboardLastUpdateRepos/>
            <LeaderboardMostStarsRepos/>
            /* 
            <LeaderboardMostWatchersRepos/>
            <LeaderboardMostRepos/>
            <LeaderboardLanguage/>
            <LeaderboardMostReposByLanguage/> */
          </div>
        | ProfilePage(user) => 
          <div>
            <User user/>
            <Profile user/>
          </div>
        }
      )
      <Link href="/profiles">(str("profiles"))</Link>
    </div>,
};
