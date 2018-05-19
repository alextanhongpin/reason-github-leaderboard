let baseUrl = "http://localhost:5000/analytics?type=leaderboard_most_repos";

type repo = {
  count: int,
  login: string,
  avatar_url: option(string),
  html_url: string
};

type counter = {
  analyticType: string,
  repos: list(repo),
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let repo = json =>
    Json.Decode.{
      count: json |> field("count", int),
      login: json |> field("login", string),
      avatar_url: json |> field("avatar_url", optional(string)),
      html_url: json |> field("html_url", string)
    };
  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      repos: json |> field("repos", list(repo)),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
};

let str = ReasonReact.string;

type state =
  | Loading
  | Error
  | Success(counter);

type action =
  | Fetch
  | FetchSuccess(counter)
  | FetchError;

let component = ReasonReact.reducerComponent("LeaderboardLastUpdateRepos");

let make = _children => {
  ...component,
  didMount: self => self.send(Fetch),
  initialState: () => Loading,
  reducer: (action, _state) =>
    switch action {
    | Fetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Fetch.fetch(baseUrl)
              |> then_(Fetch.Response.json)
              |> then_(json =>
                   json
                   |> Decode.counter
                   |> (counter => self.send(FetchSuccess(counter)))
                   |> resolve
                 )
              |> catch(err =>
                   err |> Js.log |> ((_) => self.send(FetchError)) |> resolve
                 )
              |> ignore
            )
        )
      )
    | FetchSuccess(counter) => ReasonReact.Update(Success(counter))
    | FetchError => ReasonReact.Update(Error)
    },
  render: self =>
    switch self.state {
    | Loading => <Loader />
    | Error => <div> (str("Error")) </div>
    | Success({analyticType, repos, createdAt, updatedAt}) =>
      <div>
        <h2> (str("Most repos:")) </h2>
        (
          repos
          |> List.map(({count, login, avatar_url, html_url}) =>
               <div key=login>
                 <b> (str(login)) </b>
                 (
                   switch avatar_url {
                   | Some(src) => <img src width="40" height="auto" />
                   | None => ReasonReact.null
                   }
                 )
                 <div> (str(string_of_int(count))) </div>
               </div>
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    }
};