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