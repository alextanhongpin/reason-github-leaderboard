[%bs.raw {|require('./App.css')|}];

let str = ReasonReact.string;

type user = string;

type route = 
  | ProfilePage(user)
  | HomePage;

type action = 
  | ChangeRoute(route)
  | SearchUser(string)
  | KeyDown(int);

type state = {
  route: route,
  user: user,
  loading: bool,
};

let reducer = (action, state) =>
  switch action {
  | ChangeRoute(route) => ReasonReact.Update({ ...state, route: route, loading: false })
  | SearchUser(user) => ReasonReact.Update({...state, user: user })
  | KeyDown(13) => {
    ReasonReact.UpdateWithSideEffects(
      {...state, loading: true},
      (
        _self =>
          switch state.user {
          | "" => ReasonReact.Router.push("/")
          | user => "/users/" ++ user |> ReasonReact.Router.push
          }
      ),
    )
  }
  | _ => ReasonReact.NoUpdate
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch url.path {
  | ["users", user] => ProfilePage(user)
  | _ => HomePage
  };
let component = ReasonReact.reducerComponent("App");

module Url = {
  let baseUrl = "https://api.engineers.my/v1";
  let profile = baseUrl ++ "/analytics/profiles?login=";
  let leaderboardLanguage = baseUrl ++ "/analytics?type=leaderboard_languages";
  let leaderboardLastUpdatedRepos = baseUrl ++ "/analytics?type=leaderboard_last_updated_repos";
  let leaderboardMostRepos = baseUrl ++ "/analytics?type=leaderboard_most_repos";
  let leaderboardMostReposByLanguage = baseUrl ++ "/analytics?type=leaderboard_most_repos_by_language";

  let leaderboardMostWatchersRepos = baseUrl ++ "/analytics?type=leaderboard_most_watchers_repos";
  let leaderboardMostStarsRepos = baseUrl ++ "/analytics?type=leaderboard_most_stars_repos";
  let repoCounter = baseUrl ++ "/analytics?type=repo_count";
  let userCounter = baseUrl ++ "/analytics?type=user_count";
  let user = baseUrl ++ "/users/";
};

let make = (_children) => {
  ...component,
  reducer,
  initialState: () => { route: HomePage, user: "", loading: false },
  subscriptions: (self) => [
    Sub(
      () => ReasonReact.Router.watchUrl((url) => self.send(ChangeRoute(url |> mapUrlToRoute))),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: ({ state, send }) =>
    <div className="App">
      <Header heading="engineers.my" subheading="Github Stats"/>
      <br/>
      <br/>
      <input 
        className="search" 
        _type="search" 
        placeholder="Search your Github username"
        onChange=(
          event => 
            send(SearchUser(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value))
        )
        value=(state.user)
        onKeyDown=(
          event => 
            send(KeyDown(ReactEventRe.Keyboard.which(event)))
        )
      />
      <br/>
      <br/>
      <p className="app-subheading">
        (str("We have gathered data from over ")) 
        <b>
          <UserCounter baseUrl=(Url.userCounter)/>
        </b>
        (str(" Github users and "))
        <b>
          <RepoCounter baseUrl=(Url.repoCounter)/>
        </b>
        (str(" repos from Malaysia to come up with this summary."))
      </p>
      <br/>
      (
        if (state.loading) {
          /* Global loading indicator */
          <Loader/>
        } else {
          switch state.route {
            | HomePage =>
              <div className="home-page">          
                <LeaderboardLastUpdateRepos baseUrl=(Url.leaderboardLastUpdatedRepos)/>
                <LeaderboardMostStarsRepos baseUrl=(Url.leaderboardMostStarsRepos)/>
                <LeaderboardMostRepos baseUrl=(Url.leaderboardMostRepos)/>
                <LeaderboardLanguage baseUrl=(Url.leaderboardLanguage) heading="Top Languages" subheading="View top languages that are used in Malaysia."/>
                <LeaderboardMostReposByLanguage baseUrl=(Url.leaderboardMostReposByLanguage) heading="Most Repos by Language"/>
              </div>
            | ProfilePage(user) => 
              <div>
                <User baseUrl=(Url.user) user/>
                <Profile baseUrl=(Url.profile) user/>
              </div>
            }
        }
      )
    </div>,
};
