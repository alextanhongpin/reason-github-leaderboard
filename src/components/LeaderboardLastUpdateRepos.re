/* open MomentRe; */
[%bs.raw {|require('./LeaderboardLastUpdateRepos.css')|}];

let baseUrl = "http://localhost:5000/analytics?type=leaderboard_last_updated_repos";

type counter = {
  analyticType: string,
  repos: list(Repo.repo),
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      repos: json |> field("repos", list(Repo.Decode.repo)),
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

let component = ReasonReact.reducerComponent("LeaderboardLastUpdateRepos");

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
    | Success({analyticType, repos, createdAt, updatedAt}) =>
      <div className="leaderboard-last-updated-repos">
        <h2> (str("Last Updated Repos")) </h2>
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
                   }: Repo.repo
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
                       (
                         switch language {
                         | Some(language) =>
                           <span className="language-holder">
                             <span
                               className="language-icon"
                               style=(
                                 ReactDOMRe.Style.make(
                                   ~background=
                                     Color.StringMap.find(
                                       language,
                                       Color.colors
                                     ),
                                   ()
                                 )
                               )
                             />
                             (str(" "))
                             <div className="language-label">
                               (str(language))
                             </div>
                           </span>
                         | None => ReasonReact.null
                         }
                       )
                       <div className="repo__footer-date">
                         (str(Date.parseDate(updated_at)))
                       </div>
                     </div>
                   </div>
                 </div>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </div>
      </div>
    }
};