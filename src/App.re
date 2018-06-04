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
  route,
  user,
  loading: bool
};

let reducer = (action, state) =>
  switch action {
  | ChangeRoute(route) => ReasonReact.Update({...state, route, loading: false})
  | SearchUser(user) => ReasonReact.Update({...state, user})
  | KeyDown(13) =>
    ReasonReact.UpdateWithSideEffects(
      {...state, loading: true},
      (
        _self =>
          switch state.user {
          | "" => ReasonReact.Router.push("/")
          | user => "/users/" ++ user |> ReasonReact.Router.push
          }
      )
    )
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
  /* let baseUrl = "http://localhost:8080"; */
  let leaderboardLanguage = baseUrl ++ "/stats?type=languages_most_popular";
  let leaderboardLastUpdatedRepos = baseUrl ++ "/stats?type=repos_most_recent";
  let repoCounter = baseUrl ++ "/stats?type=repo_count";
  let userCounter = baseUrl ++ "/stats?type=user_count";
  let leaderboardMostRepos = baseUrl ++ "/stats?type=repo_count_by_user";
  let leaderboardMostReposByLanguage = baseUrl ++ "/stats?type=repos_by_language";
  let leaderboardMostStarsRepos = baseUrl ++ "/stats?type=repos_most_stars";
  let leaderboardMostForks = baseUrl ++ "/stats?type=repos_most_forks";
  
  let leaderboardUsersByCompany = baseUrl ++  "/stats?type=users_by_company";
  let user = baseUrl ++ "/users/"; 
};

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: HomePage, user: "", loading: false},
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeRoute(url |> mapUrlToRoute))
        ),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: ({state, send}) => {
    <div className="App">
      <Header heading="engineers.my" subheading="Github Stats" />
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
         <Counter baseUrl=(Url.userCounter)/>
       </b>
       (str(" Github users and "))
       <b>
         <Counter baseUrl=(Url.repoCounter)/>
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
          | ProfilePage(user) =>
            <div>
              <User baseUrl=(Url.user) user/>
            </div>
          | HomePage =>
            <div className="home-page">
            <LeaderboardUsersByCompany heading="Companies Around Malaysia" baseUrl=(Url.leaderboardUsersByCompany)/>
            <LeaderboardLastUpdateRepos heading="Most Recent" baseUrl=(Url.leaderboardLastUpdatedRepos)/>
            <LeaderboardMostStarsRepos heading="Most Stars" baseUrl=(Url.leaderboardMostStarsRepos)/>
            <LeaderboardMostRepos heading="Most Repos" baseUrl=(Url.leaderboardMostRepos)/>
            <LeaderboardMostForks heading="Most Forks" baseUrl=(Url.leaderboardMostForks)/>
            <LeaderboardLanguage heading="Top Languages" subheading="View top languages that are used in Malaysia."  baseUrl=(Url.leaderboardLanguage)/>
            <LeaderboardMostReposByLanguage heading="Most Repos by Language" baseUrl=(Url.leaderboardMostReposByLanguage) />
            </div>
        }
      }
    ) 
    </div>
  },


};