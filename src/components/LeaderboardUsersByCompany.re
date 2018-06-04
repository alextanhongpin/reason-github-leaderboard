[%bs.raw {|require('./LeaderboardUsersByCompany.css')|}];

let str = ReasonReact.string;

type user = {
  login: string,
  avatarUrl: option(string)
};

type company = {
  count: int,
  company: string,
  users: option(list(user))
};

type data = {
  analyticType: string,
  companies: option(list(company)),
  createdAt: string,
  updatedAt: string
};

type response = {data: option(data)};

module Decode = {
  let user = json => 
    Json.Decode.{
      login: json |> field("login", string),
      avatarUrl: json |> optional(field("avatarUrl", string))
    }
  let company = json =>
    Json.Decode.{
      count: json |> field("count", int),
      company: json |> field("company", string),
      users: json |> optional(field("users", list(user)))
    };
  let data = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      companies: json |> optional(field("users", list(company))),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };

  let response = json => 
    Json.Decode.{
      data: json |> optional(field("data", data))
    };
};

type state =
  | Loading
  | Error
  | Success(response);

type action =
  | Fetch
  | FetchError
  | FetchSuccess(response);
let component = ReasonReact.reducerComponent("LeaderboardUsersByCompany");

let make = (~heading, ~baseUrl, _children) => {
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
      | Some(data) => 
        <div className="leaderboard-users-by-company-holder">
          <h2>(str(heading))</h2>
          <div className="company-holder">
            (
            switch data.companies {
            | Some(companies) => 
            companies
              |> List.mapi((i,
                (
                  {
                    company,
                    count,
                    users
                  }
               )) =>
               <div className="company">
                  <h4 className="company-heading">(str(string_of_int(i + 1) ++ ". " ++ company))</h4>
                  <p className="company-subheading"><small>(str(string_of_int(count) ++ " people work here"))</small></p>

                  <div className="company-user-holder">
                  (
                    switch users {
                    | Some(users) => 
                      users
                      |> List.map(
                        (
                          {
                            login,
                            avatarUrl
                          }
                        ) => 

                          (
                            switch avatarUrl {
                            | Some(avatarUrl) => <a className="company-user-image" href=("https://github.com/" ++ login) target="_blank"><img src=(avatarUrl) title=(login) width="30px" height="auto"/></a>
                            | None => ReasonReact.null
                            }
                          )
                      )
                      |> Array.of_list
                      |> ReasonReact.array
                    | None => ReasonReact.null
                    }
                  )
                  </div>
               </div>
              )
              |> Array.of_list
              |> ReasonReact.array
            | None => ReasonReact.null
            }
            )
          </div>
        </div>  
      | None => ReasonReact.null
      }
    }
};