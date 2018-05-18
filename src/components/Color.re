module StringMap = Map.Make(String);
let colors = StringMap.(
  empty
  |> add("1C Enterprise", "#814CCC")
  |> add("ABAP", "#E8274B")
  |> add("ActionScript", "#882B0F")
  |> add("Ada", "#02f88c")
  |> add("Agda", "#315665")
  |> add("AGS Script", "#B9D9FF")
  |> add("Alloy", "#64C800")
  |> add("AMPL", "#E6EFBB")
  |> add("ANTLR", "#9DC3FF")
  |> add("API Blueprint", "#2ACCA8")
  |> add("APL", "#5A8164")
  |> add("AppleScript", "#101F1F")
  |> add("Arc", "#aa2afe")
  |> add("Arduino", "#bd79d1")
  |> add("ASP", "#6a40fd")
  |> add("AspectJ", "#a957b0")
  |> add("Assembly", "#6E4C13")
  |> add("ATS", "#1ac620")
  |> add("AutoHotkey", "#6594b9")
  |> add("AutoIt", "#1C3552")
  |> add("Ballerina", "#FF5000")
  |> add("Batchfile", "#C1F12E")
  |> add("BlitzMax", "#cd6400")
  |> add("Boo", "#d4bec1")
  |> add("Brainfuck", "#2F2530")
  |> add("C", "#555555")
  |> add("C#", "#178600")
  |> add("C++", "#f34b7d")
  |> add("Ceylon", "#dfa535")
  |> add("Chapel", "#8dc63f")
  |> add("Cirru", "#ccccff")
  |> add("Clarion", "#db901e")
  |> add("Clean", "#3F85AF")
  |> add("Click", "#E4E6F3")
  |> add("Clojure", "#db5855")
  |> add("CoffeeScript", "#244776")
  |> add("ColdFusion", "#ed2cd6")
  |> add("Common Lisp", "#3fb68b")
  |> add("Component Pascal", "#B0CE4E")
  |> add("Crystal", "#776791")
  |> add("CSS", "#563d7c")
  |> add("Cuda", "#3A4E3A")
  |> add("D", "#ba595e")
  |> add("Dart", "#00B4AB")
  |> add("DataWeave", "#003a52")
  |> add("DM", "#447265")
  |> add("Dogescript", "#cca760")
  |> add("Dylan", "#6c616e")
  |> add("E", "#ccce35")
  |> add("eC", "#913960")
  |> add("ECL", "#8a1267")
  |> add("Eiffel", "#946d57")
  |> add("Elixir", "#6e4a7e")
  |> add("Elm", "#60B5CC")
  |> add("Emacs Lisp", "#c065db")
  |> add("EmberScript", "#FFF4F3")
  |> add("EQ", "#a78649")
  |> add("Erlang", "#B83998")
  |> add("F#", "#b845fc")
  |> add("Factor", "#636746")
  |> add("Fancy", "#7b9db4")
  |> add("Fantom", "#14253c")
  |> add("FLUX", "#88ccff")
  |> add("Forth", "#341708")
  |> add("Fortran", "#4d41b1")
  |> add("FreeMarker", "#0050b2")
  |> add("Frege", "#00cafe")
  |> add("Game Maker Language", "#8fb200")
  |> add("Genie", "#fb855d")
  |> add("Gherkin", "#5B2063")
  |> add("Glyph", "#e4cc98")
  |> add("Gnuplot", "#f0a9f0")
  |> add("Go", "#375eab")
  |> add("Golo", "#88562A")
  |> add("Gosu", "#82937f")
  |> add("Grammatical Framework", "#79aa7a")
  |> add("Groovy", "#e69f56")
  |> add("Hack", "#878787")
  |> add("Harbour", "#0e60e3")
  |> add("Haskell", "#5e5086")
  |> add("Haxe", "#df7900")
  |> add("HTML", "#e34c26")
  |> add("Hy", "#7790B2")
  |> add("IDL", "#a3522f")
  |> add("Io", "#a9188d")
  |> add("Ioke", "#078193")
  |> add("Isabelle", "#FEFE00")
  |> add("J", "#9EEDFF")
  |> add("Java", "#b07219")
  |> add("JavaScript", "#f1e05a")
  |> add("Jolie", "#843179")
  |> add("JSONiq", "#40d47e")
  |> add("Julia", "#a270ba")
  |> add("Jupyter Notebook", "#DA5B0B")
  |> add("Kotlin", "#F18E33")
  |> add("KRL", "#28431f")
  |> add("Lasso", "#999999")
  |> add("Lex", "#DBCA00")
  |> add("LiveScript", "#499886")
  |> add("LLVM", "#185619")
  |> add("LOLCODE", "#cc9900")
  |> add("LookML", "#652B81")
  |> add("LSL", "#3d9970")
  |> add("Lua", "#000080")
  |> add("Makefile", "#427819")
  |> add("Mask", "#f97732")
  |> add("Matlab", "#e16737")
  |> add("Max", "#c4a79c")
  |> add("MAXScript", "#00a6a6")
  |> add("Mercury", "#ff2b2b")
  |> add("Meson", "#007800")
  |> add("Metal", "#8f14e9")
  |> add("Mirah", "#c7a938")
  |> add("MQL4", "#62A8D6")
  |> add("MQL5", "#4A76B8")
  |> add("MTML", "#b7e1f4")
  |> add("NCL", "#28431f")
  |> add("Nearley", "#990000")
  |> add("Nemerle", "#3d3c6e")
  |> add("nesC", "#94B0C7")
  |> add("NetLinx", "#0aa0ff")
  |> add("NetLinx+ERB", "#747faa")
  |> add("NetLogo", "#ff6375")
  |> add("NewLisp", "#87AED7")
  |> add("Nim", "#37775b")
  |> add("Nit", "#009917")
  |> add("Nix", "#7e7eff")
  |> add("Nu", "#c9df40")
  |> add("Objective-C", "#438eff")
  |> add("Objective-C++", "#6866fb")
  |> add("Objective-J", "#ff0c5a")
  |> add("OCaml", "#3be133")
  |> add("Omgrofl", "#cabbff")
  |> add("ooc", "#b0b77e")
  |> add("Opal", "#f7ede0")
  |> add("Oxygene", "#cdd0e3")
  |> add("Oz", "#fab738")
  |> add("P4", "#7055b5")
  |> add("Pan", "#cc0000")
  |> add("Papyrus", "#6600cc")
  |> add("Parrot", "#f3ca0a")
  |> add("Pascal", "#E3F171")
  |> add("PAWN", "#dbb284")
  |> add("Pep8", "#C76F5B")
  |> add("Perl", "#0298c3")
  |> add("Perl 6", "#0000fb")
  |> add("PHP", "#4F5D95")
  |> add("PigLatin", "#fcd7de")
  |> add("Pike", "#005390")
  |> add("PLSQL", "#dad8d8")
  |> add("PogoScript", "#d80074")
  |> add("PostScript", "#da291c")
  |> add("PowerBuilder", "#8f0f8d")
  |> add("PowerShell", "#012456")
  |> add("Processing", "#0096D8")
  |> add("Prolog", "#74283c")
  |> add("Propeller Spin", "#7fa2a7")
  |> add("Puppet", "#302B6D")
  |> add("PureBasic", "#5a6986")
  |> add("PureScript", "#1D222D")
  |> add("Python", "#3572A5")
  |> add("QML", "#44a51c")
  |> add("R", "#198CE7")
  |> add("Racket", "#22228f")
  |> add("Ragel", "#9d5200")
  |> add("RAML", "#77d9fb")
  |> add("Rascal", "#fffaa0")
  |> add("Rebol", "#358a5b")
  |> add("Red", "#f50000")
  |> add("Ren'Py", "#ff7f7f")
  |> add("Ring", "#0e60e3")
  |> add("Roff", "#ecdebe")
  |> add("Rouge", "#cc0088")
  |> add("Ruby", "#701516")
  |> add("RUNOFF", "#665a4e")
  |> add("Rust", "#dea584")
  |> add("SaltStack", "#646464")
  |> add("SAS", "#B34936")
  |> add("Scala", "#c22d40")
  |> add("Scheme", "#1e4aec")
  |> add("Self", "#0579aa")
  |> add("Shell", "#89e051")
  |> add("Shen", "#120F14")
  |> add("Slash", "#007eff")
  |> add("Smalltalk", "#596706")
  |> add("SourcePawn", "#5c7611")
  |> add("SQF", "#3F3F3F")
  |> add("Squirrel", "#800000")
  |> add("SRecode Template", "#348a34")
  |> add("Stan", "#b2011d")
  |> add("Standard ML", "#dc566d")
  |> add("SuperCollider", "#46390b")
  |> add("Swift", "#ffac45")
  |> add("SystemVerilog", "#DAE1C2")
  |> add("Tcl", "#e4cc98")
  |> add("Terra", "#00004c")
  |> add("TeX", "#3D6117")
  |> add("TI Program", "#A0AA87")
  |> add("Turing", "#cf142b")
  |> add("TypeScript", "#2b7489")
  |> add("UnrealScript", "#a54c4d")
  |> add("Vala", "#fbe5cd")
  |> add("Verilog", "#b2b7f8")
  |> add("VHDL", "#adb2cb")
  |> add("Vim script", "#199f4b")
  |> add("Visual Basic", "#945db7")
  |> add("Volt", "#1F1F1F")
  |> add("Vue", "#2c3e50")
  |> add("WebAssembly", "#04133b")
  |> add("wisp", "#7582D1")
  |> add("X10", "#4B6BEF")
  |> add("xBase", "#403a40")
  |> add("XC", "#99DA07")
  |> add("XQuery", "#5232e7")
  |> add("XSLT", "#EB8CEB")
  |> add("Yacc", "#4B6C4B")
  |> add("Zephir", "#118f9e")
);