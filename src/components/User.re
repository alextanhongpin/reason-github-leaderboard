/* https://github.com/reasonml-community/reason-react-example/blob/master/src/fetch/FetchExample.re */
let baseUrl = "http://localhost:5000/users/";

type user = {
  login: string,
  name: string,
  createdAt: string,
  avatarUrl: option(string),
  bio: option(string),
  blog: option(string),
  email: option(string),
  followers: option(int),
  following: option(int),
  htmlUrl: option(string),
  location: option(string),
  privateGists: option(int),
  publicGists: option(int),
  publicRepos: option(int),
};

type users = {data: list(user)};

module Decode = {
  let user = json =>
    Json.Decode.{
      login: json |> field("login", string),
      name: json |> field("name", string),
      createdAt: json |> field("created_at", string),
      avatarUrl: json |> optional(field("avatar_url", string)),
      bio: json |> optional(field("bio", string)),
      blog: json |> optional(field("blog", string)),
      email: json |> optional(field("email", string)),
      followers: json |> optional(field("followers", int)),
      following: json |> optional(field("following", int)),
      htmlUrl: json |> optional(field("html_url", string)),
      location: json |> optional(field("location", string)),
      privateGists: json |> optional(field("private_gists", int)),
      publicGists: json |> optional(field("public_gists", int)),
      publicRepos: json |> optional(field("public_repos", int)),
    };
  let users = json => Json.Decode.{data: json |> field("data", list(user))};
};

type state = 
  | Loading
  | Error
  | Success(user);

type action =
  | Fetch
  | FetchSuccess(user)
  | FetchError;

let component = ReasonReact.reducerComponent("User");

let make = (~user="", _children) => {
  ...component,
  initialState: () => Loading,
  didMount: self => {
    self.send(Fetch)
  },
  reducer: (action, state) =>
    switch (action) {
    | Fetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self => 
            Js.Promise.(
              Fetch.fetch(baseUrl ++ user)
              |> then_(Fetch.Response.json)
              |> then_(json => {
                    json 
                    |> Decode.users
                    |> (users => users.data)
                    |> (([user, ..._rest]) => self.send(FetchSuccess(user)))
                    |> resolve
                  }
                )
              |> catch(err => 
                err
                |> Js.log
                |> ((_) => self.send(FetchError))
                |> resolve
              )
              |> ignore
            )
        )
      )
    | FetchError => ReasonReact.Update(Error)
    | FetchSuccess(user) => ReasonReact.Update(Success(user))
    },
  render: self => {
    switch self.state {
    | Loading => <Loader/>
    | Error => <div>(ReasonReact.string("Error occured"))</div>
    | Success(user) =>
      let {avatarUrl} = user;
      switch avatarUrl {
      | Some(url) =>       
        <div>
        <img src=(url) width="320" height="auto"/>
        </div>
      | None => <div>(ReasonReact.string("no image"))</div>
      }
    }
  },
};