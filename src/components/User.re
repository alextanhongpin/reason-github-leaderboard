[%bs.raw {|require('./User.css')|}];
let str = ReasonReact.string;
/* https://github.com/reasonml-community/reason-react-example/blob/master/src/fetch/FetchExample.re */

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

let make = (~baseUrl, ~user="", _children) => {
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
    | Error => <Error subheading="The user might not exist or has been deleted."/>
    | Success({login, name, avatarUrl, createdAt, bio, blog, followers, following, htmlUrl, publicGists, publicRepos}) =>
      <div className="user-holder">
        (
          switch avatarUrl {
          | Some(url) => <img src=(url) width="320" height="auto"/>
          | None => ReasonReact.null
          }
        )
        <h1>(str(login))</h1>
        <p>(str(name))</p>

        <span className="tag">
          <b>(str("Created: "))</b>
          (str(Date.parseDate(createdAt)))
        </span>
        (
          switch blog {
          | Some(blog) => 
            <span className="tag">
              <b>(str("Website: "))</b>
              <a href=(blog) target="_blank">(str(blog))</a>
            </span>
          | None => ReasonReact.null
          }
        )

        (
          switch htmlUrl {
          | Some(htmlUrl) => 
            <span className="tag">
              <b>(str("Github: "))</b>
              <a href=(htmlUrl) target="_blank">(str(htmlUrl))</a>
            </span>
          | None => ReasonReact.null
          }
        )
        
        (
          switch bio {
          | Some(bio) => 
            <span className="tag">
              <b>(str("Bio: "))</b>
              (str(bio))
            </span>
          | None => ReasonReact.null
          }
        )

        (
          switch followers {
          | Some(followers) => 
            <span className="tag">
              <b>(str("Followers: "))</b>
              (str(string_of_int(followers)))
            </span>
          | None => ReasonReact.null
          }
        )

        (
          switch following {
          | Some(following) => 
            <span className="tag">
              <b>(str("Following: "))</b>
              (str(string_of_int(following)))
            </span>
          | None => ReasonReact.null
          }
        )


        (
          switch publicGists {
          | Some(publicGists) => 
            <span className="tag">
              <b>(str("Public Gists: "))</b>
              (str(string_of_int(publicGists)))
            </span>
          | None => ReasonReact.null
          }
        )

        (
          switch publicRepos {
          | Some(publicRepos) => 
            <span className="tag">
              <b>(str("Public Repos: "))</b>
              (str(string_of_int(publicRepos)))
            </span>
          | None => ReasonReact.null
          }
        )

      </div>
    }
  },
};
