/* https://github.com/reasonml-community/reason-react-example/blob/master/src/fetch/FetchExample.re */

type user = {
  avatarUrl: string,
  bio: string,
  blog: string,
  createdAt: string,
  email: string,
  followers: int,
  following: int,
  htmlUrl: string,
  location: string,
  login: string,
  name: string,
  privateGists: option(int),
  publicGists: int,
  publicRepos: int,
};

type users = {data: list(user)};

type state = user;

module Decode = {
  let user = json =>
    Json.Decode.{
      avatarUrl: json |> field("avatar_url", string),
      bio: json |> field("bio", string),
      blog: json |> field("blog", string),
      createdAt: json |> field("created_at", string),
      email: json |> field("email", string),
      followers: json |> field("followers", int),
      following: json |> field("following", int),
      htmlUrl: json |> field("html_url", string),
      location: json |> field("location", string),
      login: json |> field("login", string),
      name: json |> field("name", string),
      privateGists: json |> optional(field("private_gists", int)),
      publicGists: json |> field("public_gists", int),
      publicRepos: json |> field("public_repos", int),
    };
  let users = json => Json.Decode.{data: json |> field("data", list(user))};
};

type action =
  | FetchUser(user)
  | NoOp;

let component = ReasonReact.reducerComponent("User");

let make = (~user="", _children) => {
  ...component,
  initialState: () => {
    login: "",
    avatarUrl: "",
    bio: "",
    blog: "",
    createdAt: "",
    email: "",
    followers: 0,
    following: 0,
    htmlUrl: "",
    location: "",
    name: "",
    privateGists: None,
    publicGists: 0,
    publicRepos: 0,
  },
  didMount: self => {
    Js.Promise.(
      Fetch.fetch("http://localhost:5000/users/" ++ user)
      |> then_(Fetch.Response.json)
      |> then_(json => {
            json 
            |> Decode.users
            |> (users => users.data)
            |> (([user, ..._rest]) => self.send(FetchUser(user)))
            |> resolve
          }
        )
      |> catch(err => 
        err
        |> Js.log
        |> (_ => Js.Promise.resolve(self.send(NoOp)))
      )
      |> ignore
    );
  },
  reducer: (action, state) =>
    switch (action) {
    | FetchUser(user) => ReasonReact.Update(user)
    | NoOp => ReasonReact.NoUpdate
    },
  render: self => {
    let {avatarUrl} = self.state;
    <div>
      <img src=(avatarUrl) width="320" height="auto"/>
    </div>;
  },
};