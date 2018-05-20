[%bs.raw {|require('./Repo.css')|}];

let str = ReasonReact.string;

type owner = {
  login: string,
  avatar_url: option(string),
  url: string,
  html_url: string
};

type repo = {
  name: string,
  full_name: string,
  owner,
  html_url: string,
  description: option(string),
  fork: bool,
  created_at: string,
  updated_at: string,
  stargazers_count: option(int),
  watchers_count: option(int),
  language: option(string)
};

module Decode = {
  let owner = json =>
    Json.Decode.{
      login: json |> field("login", string),
      avatar_url: json |> field("avatar_url", optional(string)),
      url: json |> field("url", string),
      html_url: json |> field("html_url", string)
    };
  let repo = json =>
    Json.Decode.{
      name: json |> field("name", string),
      full_name: json |> field("full_name", string),
      owner: json |> field("owner", owner),
      html_url: json |> field("html_url", string),
      description: json |> field("description", optional(string)),
      fork: json |> field("fork", bool),
      created_at: json |> field("created_at", string),
      updated_at: json |> field("updated_at", string),
      stargazers_count: json |> field("stargazers_count", optional(int)),
      watchers_count: json |> field("watchers_count", optional(int)),
      language: json |> field("language", optional(string))
    };
};

let component = ReasonReact.statelessComponent("RepoList");

let make = (~heading="", ~repos: list(repo), _children) => {
  ...component,
  render: self =>
    <div className="leaderboard-last-updated-repos">
      <h2> (str(heading)) </h2>
      <div className="repo-holder">
        (
          repos
          |> List.map(
               (
                 {
                   name,
                   full_name,
                   owner,
                   html_url,
                   description,
                   fork,
                   created_at,
                   updated_at,
                   stargazers_count,
                   watchers_count,
                   language
                 }
               ) =>
               <div key=html_url className="repo">
                 <div className="repo__image-holder">
                   (
                     switch owner.avatar_url {
                     | Some(src) => <img src width="30" height="auto" />
                     | None => <div className="placeholder" />
                     }
                   )
                 </div>
                 <div className="repo__info-holder">
                   <b> (str(full_name)) </b>
                   <p className="repo__description">
                     (
                       switch description {
                       | Some(description) => str(description)
                       | None => str("No description available")
                       }
                     )
                   </p>
                   <div className="repo__footer">
                     (
                       switch stargazers_count {
                       | Some(0) => ReasonReact.null
                       | Some(count) =>
                         <div>
                           <span className="icon-star" />
                           (str(" "))
                           (str(string_of_int(count)))
                         </div>
                       | None => ReasonReact.null
                       }
                     )
                     <Language language />
                     <div className="repo__footer-date">
                       (str(Date.parseDate(updated_at)))
                     </div>
                   </div>
                 </div>
               </div>
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </div>
    </div>
};