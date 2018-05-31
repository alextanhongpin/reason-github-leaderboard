type data = {
  analyticType: string,
  users: list(ViewSwitcher.users),
  createdAt: string,
  updatedAt: string
};

type response = {data: option(data)};

module Decode = {
  let data = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      users: json |> field("users", list(ViewSwitcher.Decode.users)),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
  let response = json =>
    Json.Decode.{data: json |> field("data", optional(data))};
};

let str = ReasonReact.string;

type state =
  | Loading
  | Error
  | Success(response);

type action =
  | Fetch
  | FetchSuccess(response)
  | FetchError;

let component = ReasonReact.reducerComponent("LeaderboardMostReposByLanguage");

let make = (~baseUrl, ~heading="", _children) => {
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
                   |> Decode.response
                   |> (response => self.send(FetchSuccess(response)))
                   |> resolve
                 )
              |> catch(err =>
                   err |> Js.log |> ((_) => self.send(FetchError)) |> resolve
                 )
              |> ignore
            )
        )
      )
    | FetchSuccess(response) => ReasonReact.Update(Success(response))
    | FetchError => ReasonReact.Update(Error)
    },
  render: self =>
    switch self.state {
    | Loading => <Loader />
    | Error => <Error />
    | Success(response) =>
      switch response.data {
      | Some({users, updatedAt}) =>
        let user = users |> List.hd;
        <div>
          <h2> (str(heading)) </h2>
          <ViewSwitcher language=user.language users />
        </div>;
      | None => ReasonReact.null
      }
    }
};