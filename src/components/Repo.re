[%bs.raw {|require('./Repo.css')|}];

let str = ReasonReact.string;

type repo = {
  name: string,
  createdAt: string,
  updatedAt: string,
  description: option(string),
  languages: option(list(string)),
  homepageUrl: option(string),
  forkCount: int,
  isFork: bool,
  nameWithOwner: string,
  login: string,
  avatarUrl: option(string),
  stargazers: int,
  watchers: int,
  url: string
};

module Decode = {
  let repo = json =>
    Json.Decode.{
      name: json |> field("name", string),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string),
      description: json |> field("description", optional(string)),
      languages: json |> field("languages", optional(list(string))),
      homepageUrl: json |> field("homepageUrl", optional(string)),
      forkCount: json |> field("forkCount", int),
      isFork: json |> field("isFork", bool),
      nameWithOwner: json |> field("nameWithOwner", string),
      login: json |> field("login", string),
      avatarUrl: json |> field("avatarUrl", optional(string)),
      stargazers: json |> field("stargazers", int),
      watchers: json |> field("watchers", int),
      url: json |> field("url", string)
    };
};

let component = ReasonReact.statelessComponent("RepoList");

let make = (~heading="", ~repos: list(repo), _children) => {
  ...component,
  render: self => {
    <div className="leaderboard-last-updated-repos">
       <h2> (str(heading)) </h2> 
      <div className="repo-holder">
           (
             repos
             |> List.map(
                  (
                    {
                     name,
                     createdAt,
                     updatedAt,
                     description,
                     languages,
                     homepageUrl,
                     forkCount,
                     isFork,
                     nameWithOwner,
                     login,
                     avatarUrl,
                     stargazers,
                     watchers,
                     url,
                    }
                  ) =>
                  <a key=url className="repo link" href=(url) target="_blank">
                    <div className="repo__image-holder">
                      (
                        switch avatarUrl {
                        | Some(src) => <img src width="30" height="auto" />
                        | None => <div className="placeholder" />
                        }
                      )
                    </div>
                    <div className="repo__info-holder">
                      <b> (str(nameWithOwner)) </b>
                      <p className="repo__description">
                        (
                          switch description {
                          | Some(description) => str(description)
                          | None => str("No description available")
                          }
                        )
                      </p>
                      <div className="repo__footer">
                        <div>
                          <span className="icon-star" />
                          (str(" "))
                          (str(string_of_int(stargazers)))
                        </div>

                        (switch languages {
                        | Some(languages) => switch languages {
                        | [] => ReasonReact.null
                        | [language,...rest] => <Language language=Some(language) />
                        }
                        | None => ReasonReact.null
                        })
                        
                        <div className="repo__footer-date">
                          (str(Date.parseDate(updatedAt)))
                        </div>
                      </div>
                    </div>
                  </a>
                )
             |> Array.of_list
             |> ReasonReact.array
           )
         </div>
         </div>
                        },
};