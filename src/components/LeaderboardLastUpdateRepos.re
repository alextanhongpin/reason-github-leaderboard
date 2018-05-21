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

let component = ReasonReact.reducerComponent("LeaderboardLastUpdateRepos");

let make = (~baseUrl, _children) => {
  ...component,
  didMount: self => self.send(Fetch), /* self.onUnmount(() => Js.Global.clearInterval(intervalId)); */
  /* let intervalId = Js.Global.setInterval(() => self.send(Fetch), 1250); */
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
    | Error => <Error />
    | Success({analyticType, repos, createdAt, updatedAt}) =>
      <Repo heading="Last Updated" repos />
    }
};