type language = {
  lang: string,
  score: float,
  count: int,
};

type keyword = {
  word: string,
  count: int,
};

type match = {
  login: string,
  score: float,
  avatarUrl: string,
  htmlUrl: string,
};

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
  privateGists: int,
  publicGists: int,
  publicRepos: int,
};

type users = {data: list(user)};

type profile = {
  login: string,
  totalCount: int,
  topLanguages: list(language),
  topKeywords: list(keyword),
  stargazersCount: int,
  watchersCount: int,
  forksCount: int,
  matches: list(match),
};

type state = {
  user,
  profile,
};

module Decode = {
  let language = json =>
    Json.Decode.{
      lang: json |> field("lang", string),
      score: json |> field("score", float),
      count: json |> field("count", int),
    };
  let keyword = json =>
    Json.Decode.{
      word: json |> field("word", string),
      count: json |> field("count", int),
    };
  let match = json =>
    Json.Decode.{
      login: json |> field("login", string),
      score: json |> field("score", float),
      avatarUrl: json |> field("avatarUrl", string),
      htmlUrl: json |> field("htmlUrl", string),
    };
  let profile = json =>
    Json.Decode.{
      login: json |> field("login", string),
      totalCount: json |> field("totalCount", int),
      topLanguages: json |> field("topLanguages", list(language)),
      topKeywords: json |> field("topKeywords", list(keyword)),
      stargazersCount: json |> field("stargazersCount", int),
      watchersCount: json |> field("watchersCount", int),
      forksCount: json |> field("forksCount", int),
      matches: json |> field("matches", list(match)),
    };
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
      privateGists: json |> field("private_gists", int),
      publicGists: json |> field("public_gists", int),
      publicRepos: json |> field("public_repos", int),
    };
  let users = json => Json.Decode.{data: json |> field("data", list(user))};
};

type action =
  | FetchProfile(profile)
  | FetchUser(user)
  | NoOp;

let component = ReasonReact.reducerComponent("Profile");

let make = _children => {
  ...component,
  initialState: () => {
    profile: {
      login: "",
      totalCount: 0,
      topLanguages: [],
      topKeywords: [],
      stargazersCount: 0,
      watchersCount: 0,
      forksCount: 0,
      matches: [],
    },
    user: {
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
      privateGists: 0,
      publicGists: 0,
      publicRepos: 0,
    },
  },
  didMount: self => {
    Js.Promise.(
      Fetch.fetch(
        "http://localhost:5000/analytics/profiles?login=alextanhongpin",
      )
      |> then_(Fetch.Response.json)
      |> then_(json => {
           let data = json |> Decode.profile;
           self.send(FetchProfile(data));
           resolve();
         })
    )
    |> ignore;
    Js.Promise.(
      Fetch.fetch("http://localhost:5000/users/alextanhongpin")
      |> then_(Fetch.Response.json)
      |> then_(json => {
           let users = json |> Decode.users;
           let [user] = users.data;
           self.send(FetchUser(user));
           resolve();
         })
    )
    |> ignore;
  },
  reducer: (action, state) =>
    switch (action) {
    | FetchProfile(profile) => ReasonReact.Update({...state, profile})
    | FetchUser(user) => ReasonReact.Update({...state, user})
    | NoOp => ReasonReact.NoUpdate
    },
  render: self => {
    let {login, totalCount, stargazersCount, matches} = self.state.profile;
    let {avatarUrl} = self.state.user;
    <div>
      <h3> (ReasonReact.string(login)) </h3>
      <div>
        (ReasonReact.string("totalCount:"))
        <b> (ReasonReact.string(string_of_int(totalCount))) </b>
      </div>
      <div>
        (ReasonReact.string("stargazersCount:"))
        <b> (ReasonReact.string(string_of_int(stargazersCount))) </b>
      </div>
      <img src=(avatarUrl) width="320" height="auto"/>
      (
        matches
        |> List.map(({login, score, avatarUrl, htmlUrl}) =>
             <div>
               <img src=(avatarUrl) width="30" height="auto" />
               <a href=htmlUrl> (ReasonReact.string(login)) </a>
             </div>
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};