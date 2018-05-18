let baseUrl = "http://localhost:5000/analytics?type=leaderboard_most_watchers_repos";

type counter = {
  analyticType: string,
  repos: list(Repo.repo),
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      repos: json |> field("repos", list(Repo.Decode.repo)),
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

let component = ReasonReact.reducerComponent("LeaderboardMostWatchersRepos");

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
        <h2>(str("Most watched repos:"))</h2>
        (
          repos
          |> List.map(
            ({name, full_name, owner, html_url, description, fork, created_at, updated_at, stargazers_count, watchers_count, language}: Repo.repo) => 
              <div key=html_url>
                <b>(str(full_name))</b>
                (
                  switch owner.avatar_url {
                  | Some(src) => <img src width="40" height="auto"/>
                  | None => ReasonReact.null
                  }
                )

                <div>(
                  switch description {
                  | Some(description) => <div>(str(description))</div>
                  | None => ReasonReact.null
                  }
                )</div>
              </div>
          )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    }
};
 