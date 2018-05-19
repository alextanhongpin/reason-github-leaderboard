let baseUrl = "http://localhost:5000/analytics?type=leaderboard_most_repos_by_language";

type owner = {
  avatar_url: string,
  html_url: string,
  login: string,
  count: int
};

type repo = {
  lang: string,
  top: list(owner)
};

type counter = {
  analyticType: string,
  repos: list(repo),
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let owner = json =>
    Json.Decode.{
      avatar_url: json |> field("avatar_url", string),
      html_url: json |> field("html_url", string),
      login: json |> field("login", string),
      count: json |> field("count", int)
    };
  let repo = json =>
    Json.Decode.{
      lang: json |> field("lang", string),
      top: json |> field("top", list(owner))
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

let component = ReasonReact.reducerComponent("LeaderboardMostReposByLanguage");

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
        <h2> (str("Most repos by language:")) </h2>
        (
          repos
          |> List.map(({lang, top}) =>
               <div key=lang>
                 <b> (str(lang)) </b>
                 (
                   top
                   |> List.map(({avatar_url, html_url, login, count}) =>
                        <div key=login>
                          <img src=avatar_url width="40" height="auto" />
                          (str(login))
                          (str(string_of_int(count)))
                        </div>
                      )
                   |> Array.of_list
                   |> ReasonReact.arrayToElement
                 )
               </div>
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    }
};