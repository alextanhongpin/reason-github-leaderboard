[%bs.raw {|require('./RepoCounter.css')|}];
let baseUrl = "http://localhost:5000/analytics?type=repo_count";

type counter = {
  analyticType: string,
  count: int,
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      count: json |> field("count", int),
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

let component = ReasonReact.reducerComponent("RepoCounter");

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
    | Success({analyticType, count, createdAt, updatedAt}) =>
      <div className="repo-counter">
        <h1>(str(string_of_int(count)))</h1>
        <p>(str("Total Repos"))</p>
      </div>
    }
};