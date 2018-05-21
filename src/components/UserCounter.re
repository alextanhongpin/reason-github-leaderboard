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

let component = ReasonReact.reducerComponent("UserCounter");

let make = (~baseUrl, _children) => {
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
    | Loading => <span> (str("0")) </span>
    | Error => <Error />
    | Success({count}) => <span> (str(Counter.toLocaleString(count))) </span>
    }
};