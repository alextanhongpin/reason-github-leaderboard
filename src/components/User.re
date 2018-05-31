[%bs.raw {|require('./User.css')|}];

let rec summarize = (l: list(int)) =>
  switch l {
  | [] => 0
  | [head, ...tail] => head + summarize(tail)
  };

let str = ReasonReact.string;

type item = {
  name: string,
  count: int
};

type user = {
  login: string,
  score: float,
  avatarUrl: option(string)
};

type data = {
  name: string,
  createdAt: string,
  updatedAt: string,
  fetchedAt: string,
  login: string,
  location: option(string),
  bio: option(string),
  email: option(string),
  avatarUrl: option(string),
  websiteUrl: option(string),
  repositories: int,
  gists: int,
  followers: int,
  following: int,
  watchers: int,
  stargazers: int,
  forks: int,
  languages: option(list(item)),
  keywords: option(list(item)),
  matches: option(list(user))
};

type response = {data: option(data)};

module Decode = {
  let item = json =>
    Json.Decode.{
      name: json |> field("name", string),
      count: json |> field("count", int)
    };
  let user = json =>
    Json.Decode.{
      login: json |> field("login", string),
      score: json |> field("score", float),
      avatarUrl: json |> field("avatarUrl", optional(string))
    };
  let data = json =>
    Json.Decode.{
      name: json |> field("name", string),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string),
      fetchedAt: json |> field("fetchedAt", string),
      login: json |> field("login", string),
      bio: json |> optional(field("bio", string)),
      location: json |> optional(field("location", string)),
      email: json |> optional(field("email", string)),
      avatarUrl: json |> optional(field("avatarUrl", string)),
      websiteUrl: json |> optional(field("websiteUrl", string)),
      repositories: json |> field("repositories", int),
      gists: json |> field("gists", int),
      followers: json |> field("followers", int),
      following: json |> field("following", int),
      watchers: json |> field("watchers", int),
      stargazers: json |> field("stargazers", int),
      forks: json |> field("forks", int),
      languages: json |> optional(field("languages", list(item))),
      keywords: json |> optional(field("keywords", list(item))),
      matches: json |> optional(field("matches", list(user)))
    };
  let response = json =>
    Json.Decode.{data: json |> optional(field("data", data))};
};

type state =
  | Loading
  | Error
  | Success(response);

type action =
  | Fetch
  | FetchSuccess(response)
  | FetchError;

let component = ReasonReact.reducerComponent("User");

let make = (~baseUrl, ~user="", _children) => {
  ...component,
  initialState: () => Loading,
  didMount: self => self.send(Fetch),
  reducer: (action, _state) =>
    switch action {
    | Fetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Fetch.fetch(baseUrl ++ user)
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
    | Error =>
      <Error subheading="The user might not exist or has been deleted." />
    | Success(response) =>
      switch response.data {
      | Some({
          name,
          createdAt,
          login,
          bio,
          avatarUrl,
          websiteUrl,
          repositories,
          gists,
          followers,
          following,
          watchers,
          stargazers,
          forks,
          matches,
          keywords,
          languages
        }) =>
        let sumRepos =
          switch languages {
          | Some(languages) =>
            languages |> List.map(({count}) => count) |> summarize
          | None => 0
          };
        <div>
          <div className="user-holder">
            (
              switch avatarUrl {
              | Some(url) => <img src=url width="320" height="auto" />
              | None => ReasonReact.null
              }
            )
            <h1> (str(login)) </h1>
            <p> (str(name)) </p>
            <span className="tag">
              <b> (str("Created: ")) </b>
              (str(Date.parseDate(createdAt)))
            </span>
            (
              switch websiteUrl {
              | Some(websiteUrl) =>
                <span className="tag">
                  <b> (str("Website: ")) </b>
                  <a href=websiteUrl target="_blank"> (str(websiteUrl)) </a>
                </span>
              | None => ReasonReact.null
              }
            )
            <span className="tag">
              <b> (str("Github: ")) </b>
              <a href="" target="_blank">
                (str("https://github.com/" ++ login))
              </a>
            </span>
            (
              switch bio {
              | Some(bio) =>
                <span className="tag">
                  <b> (str("Bio: ")) </b>
                  (str(bio))
                </span>
              | None => ReasonReact.null
              }
            )
            <span className="tag">
              <b> (str("Followers: ")) </b>
              (str(string_of_int(followers)))
            </span>
            <span className="tag">
              <b> (str("Following: ")) </b>
              (str(string_of_int(following)))
            </span>
            <span className="tag">
              <b> (str("Public Gists: ")) </b>
              (str(string_of_int(gists)))
            </span>
            <span className="tag">
              <b> (str("Public Repos: ")) </b>
              (str(string_of_int(repositories)))
            </span>
          </div>
          /* PROFILE */
          <div className="profile-holder">
            <span
              className="tag"
              title="The total count of stars this user has for all repos">
              <b> (str("Stars: ")) </b>
              (str(string_of_int(stargazers)))
            </span>
            <span
              className="tag"
              title="The total count of watchers this user have">
              <b> (str("Watchers: ")) </b>
              (str(string_of_int(watchers)))
            </span>
            <span
              className="tag"
              title="The total count of forks this userhas for all repos">
              <b> (str("Forks: ")) </b>
              (str(string_of_int(forks)))
            </span>
            (
              switch keywords {
              | Some(keywords) =>
                <div>
                  <b> (str("Keywords: ")) </b>
                  (
                    keywords
                    |> List.map(({name, count}) =>
                         <span key=name className="keyword">
                           (str(name))
                           (str({js| × |js}))
                           (str(string_of_int(count)))
                         </span>
                       )
                    |> Array.of_list
                    |> ReasonReact.array
                  )
                </div>
              | None => ReasonReact.null
              }
            )
            <br />
            (
              switch languages {
              | Some(languages) =>
                <div>
                  <b> (str("Languages: ")) </b>
                  (
                    languages
                    |> List.map(({name, count}) => {
                         let color = Color.getColor(name);
                         let percentage =
                           int_of_float(
                             float_of_int(count)
                             /. float_of_int(sumRepos)
                             *. 100.0
                           );
                         if (percentage > 0) {
                           <span
                             key=name
                             className="keyword"
                             style=(
                               ReactDOMRe.Style.make(
                                 ~background=Color.hexToRgbA(color, 0.2),
                                 ~color,
                                 ()
                               )
                             )>
                             (
                               str(
                                 name
                                 ++ "  "
                                 ++ string_of_int(percentage)
                                 ++ "%"
                               )
                             )
                           </span>;
                         } else {
                           ReasonReact.null;
                         };
                       })
                    |> Array.of_list
                    |> ReasonReact.array
                  )
                </div>
              | None => ReasonReact.null
              }
            )
            <br />
            <h3> (str("Similar Users")) </h3>
            (
              switch matches {
              | Some(matches) =>
                <div className="matches-holder">
                  (
                    matches
                    |> List.map(({login, avatarUrl}: user) =>
                         <a
                           href=("/users/" ++ login)
                           className="matches-item link"
                           key=login>
                           <div>
                             (
                               switch avatarUrl {
                               | Some(avatarUrl) =>
                                 <img
                                   className="matches-item__img"
                                   src=avatarUrl
                                   width="80"
                                   height="auto"
                                 />
                               | None =>
                                 <img
                                   className="matches-item__img"
                                   width="80"
                                   height="auto"
                                 />
                               }
                             )
                           </div>
                           <b className="matches-item__name">
                             (ReasonReact.string(login))
                           </b>
                         </a>
                       )
                    |> Array.of_list
                    |> ReasonReact.array
                  )
                </div>
              | None => str("You have no matches at the moment")
              }
            )
            <br />
            <br />
          </div>
        </div>;
      | None =>
        <Error subheading="The user might not exist or has been deleted." />
      }
    }
};